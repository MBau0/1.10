#include "Server.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <iostream>

#include "ServerClient.h"

#include "Message.h"
#include "TransformMessage.h"
#include "Debug.h"
#include "PeriodicTimer.h"

#include <mutex>

#define DEFAULT_PORT "23001"

std::mutex message_pool_mutex;

Server::Server() : 
    _listen_socket  ( INVALID_SOCKET ),
    _started        ( false )
{
    WSAData wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if(result != 0) {
        printf("WsaStartup failed: %d\n", result);
    }

    create_message_table();

    _iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if(_iocp == NULL) {
        printf("CreateIOCompletionPort failed: %d\n", WSAGetLastError());
        assert(NULL);
    }

    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    long unsigned int threads_to_make = system_info.dwNumberOfProcessors * 2;
    for(long unsigned int i = 0; i < threads_to_make; ++i) {
        auto thread = std::thread(&Server::worker_thread, this);
        _threads.push_back(std::move(thread));
    }
    printf("Made %d threads\n", (int)_threads.size());
}

Server::~Server() {
    if(_started) {
        stop();
    }

    WSACleanup();
}

void Server::create_message_table() {
    _message_table.emplace(TRANSFORM_MESSAGE, transform_message);
}

bool Server::start() {
    if(_started) {
        return true;
    }
    
    struct addrinfo *result = nullptr;
    struct addrinfo hints;
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int error = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if(error) {
        printf("getaddrinfo error: %d\n", error);
    }

    _listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if(_listen_socket == INVALID_SOCKET) {
        printf("socket error: %1d\n", WSAGetLastError());
        return stop();
    }

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(23001);
    error = bind(_listen_socket, (PSOCKADDR)&addr, sizeof(addr));
    
    //error = bind(_listen_socket, result->ai_addr, (int)result->ai_addrlen);

    if(error == SOCKET_ERROR) {
        printf("bind failed: %d\n", error);
        return stop();
    }

    //error = listen(_listen_socket, SOMAXCONN);
    error = listen(_listen_socket, 5);
    if(error == SOCKET_ERROR) {
        printf("listen failed: %d\n", error);
        return stop();
    }

    struct sockaddr_in name;
    int length = 0;
    getpeername(_listen_socket, (sockaddr*)&name, &length);
    std::cout << "port:" << name.sin_port << '\n';

    printf("Server started...\n");
    _started = true;
    return _started;
}

bool Server::stop() {
    closesocket(_listen_socket);
    if(_started) {
        _started = false;

        _tick_updater.join();
        for(auto& thread : _threads) {
            thread.join();
        }

        for(auto client : _clients) {
            delete client;
        }
    }
    return false;
}

void Server::main() {
    _tick_updater = std::thread(&Server::tick_update, this);
    accept_clients();
}

void Server::tick_update() {
    PeriodicTimer tick(500);
    while(_started) {
        if(tick.alert()) {
            std::cout << "update\n";
            _message_pool.swap(message_pool_mutex);

            for(auto message : *_message_pool.get_previous()) {
                for(auto client : _clients) {
                    client->send(message);
                }

                delete message;
            }

            _message_pool.clear_previous();
        }
    }
}

void Server::accept_clients() {
    while(_started) {
        printf("accepting clients\n");
        ServerClient* client = new ServerClient;
        printf("Waiting Connection\n");
        if(client->connect(_listen_socket)) {
            printf("Waiting Association\n");
            client->associate(_iocp);
            printf("Waiting Receive\n");
            client->receive();
            _clients.push_back(client);
        }
    }
}


void Server::worker_thread() {
    DWORD bytes_transferred;
    DWORD_PTR completion_key;
    PER_IO_OPERATION *context = nullptr;
    LPOVERLAPPED lpoverlapped = nullptr;

    while(1) {
        int result = GetQueuedCompletionStatus(_iocp, &bytes_transferred, &completion_key, &lpoverlapped, INFINITE);
        context = (PER_IO_OPERATION*) lpoverlapped;
        //std::cout << "Worker Thread Passed Completion Status: " << std::this_thread::get_id() << std::endl;
        if(result == 0) {
            printf("GetQueuedCompletionStatus failed : %lu\n", GetLastError());
        }
            if(bytes_transferred == 0) {
            printf("Need to close socket\n");
        }
        else {
            //std::cout << "bytes transferred: " << bytes_transferred << std::endl;
            //std::cout << "Data: " << context->_buffer << std::endl;
            ////for(int i = 0; i < bytes_transferred; ++i) {
            //    print_b(context->_buffer[i]);
            //}
            std::cout << std::endl;
            std::cout << "Op: " << (int)context->_operation << std::endl;
            if((int) context->_operation == OP_READ) {
                parse_message(context, bytes_transferred);
            }
            context->_client->receive();
        }
    }
} 

void Server::parse_message(PER_IO_OPERATION* context, DWORD bytes_transferred) {
    uint8_t message_id;
    char* ptr = context->_buffer;
    memcpy(&message_id, ptr, sizeof(message_id));
    ptr += sizeof(message_id);
    bytes_transferred -= sizeof(message_id);
    std::cout << "message_id: " << (int)message_id << "\n";

    if(_message_table.count(message_id)) {
        std::lock_guard<std::mutex> guard(message_pool_mutex);
        _message_table.at(message_id)( ptr, bytes_transferred, _message_pool.get_current());
    }
}

void transform_message(char* buffer, uint32_t size, std::vector<Message*>* messages) {
    uint32_t unitid;
    float position[3];

    std::cout << "TRANSFORM MESSAGE\n";
    //for(int i = 0; i < size; ++i) {
    //    print_b(buffer[i]);
    //}

    memcpy(&unitid, buffer, sizeof(uint32_t));
    memcpy(position, buffer + sizeof(uint32_t), sizeof(float) * 3);

    std::cout << "Transform Message: unit_id: " << unitid << " position: " << position[0] << " " << position[1] << " " << position[2] << '\n';

    TransformMessage* message = new TransformMessage(unitid, glm::vec3(position[0], position[1], position[2]));

    messages->push_back(reinterpret_cast<Message*>(message));
}
