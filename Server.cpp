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
#include "SyncMessage.h"
#include "EntityMessage.h"

#include "ServerEntity.h"

#include "Debug.h"

#include <mutex>

#define DEFAULT_PORT "23001"

namespace server 
{

    std::mutex message_pool_mutex;

    void receive_transform_message(char* buffer, uint32_t size, Server* server);

    void receive_sync_message(char* buffer, uint32_t size, Server* server);

    void receive_entity_message(char* buffer, uint32_t size, Server* server);

};

Server::Server() : 
    _listen_socket  ( INVALID_SOCKET ),
    _started        ( false ),
    _tick_timer     ( 50 ),
    _entities       ( 200 )
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
    _message_receive_events.emplace(SYS_SYNC_MESSAGE, server::receive_sync_message);
    _message_receive_events.emplace(GAME_TRANSFORM_MESSAGE, server::receive_transform_message);
    _message_receive_events.emplace(GAME_ENTITY_MESSAGE, &server::receive_entity_message);
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
    while(_started) {
        if(_tick_timer.alert()) {
            _message_pool.swap(server::message_pool_mutex);

            for(auto message : *_message_pool.get_previous()) {
                for(auto client : _clients) {
                    client->send(message);
                }

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
            std::cout << "WTFASJDKLAJSDKLJLAKSDJLKAJSDKLJALSDJAKLSDJ\n";
            sync(client);
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
                receive_message(context, bytes_transferred);
            }
            context->_client->receive();
        }
    }
} 

void Server::sync(ServerClient* client) {
    std::cout << "SYNC THIS BITCH\n";
    auto message = std::make_shared<SyncMessage>(client->_id, _tick_timer.time(), (uint8_t)0);

    client->send(std::static_pointer_cast<Message>(message));
}

__time64_t Server::get_tick_time() {
    return _tick_timer.time();
}

std::vector<std::shared_ptr<Message>>* Server::get_current_messages() const {
    return _message_pool.get_current();
}

CompactArray<ServerEntity>& Server::get_entities() {
    return _entities;
}

void Server::send_to_client(int client_id, std::shared_ptr<Message> message) {
    if (!message->valid()) {
        return;
    }

    for(auto client : _clients) {
        if(client->_id == client_id) {
            client->send(message);
            return;
        }
    }
}

void Server::receive_message(PER_IO_OPERATION* context, DWORD bytes_transferred) {
    uint8_t message_id;
    char* ptr = context->_buffer;
    memcpy(&message_id, ptr, sizeof(message_id));
    ptr += sizeof(message_id);
    std::cout << "message_id: " << (int)message_id << "\n";

    uint32_t size;
    memcpy(&size, ptr, sizeof(uint32_t));
    ptr += sizeof(uint32_t);

    if(_message_receive_events.count(message_id)) {
        std::lock_guard<std::mutex> guard(server::message_pool_mutex);
        _message_receive_events.at(message_id)( ptr, size, this );
    }
}

void server::receive_sync_message(char* buffer, uint32_t size, Server* server) {
    std::cout << "Sync Message\n";

    auto message = std::make_shared<SyncMessage>(buffer, size);

    if(message->_step == 1) {
        __time64_t delta = server->get_tick_time() - message->_sub_time;
        __time64_t mean_path_delay = ( message->_dom_time + delta ) / 2;
        message->_offset = message->_dom_time - mean_path_delay;
        message->_step = 2;

        server->send_to_client(message->_client_id, std::static_pointer_cast<Message>( message ));
    }

}

void server::receive_transform_message(char* buffer, uint32_t size, Server* server) {
    std::cout << "Transform Message\n";

    std::shared_ptr<TransformMessage> message = std::make_shared<TransformMessage>(buffer, size);

    auto messages = server->get_current_messages();
    messages->push_back(std::static_pointer_cast<Message>(message));
}

void server::receive_entity_message(char* buffer, uint32_t size, Server* server) {
    std::cout << "Entity Message\n";

    std::shared_ptr<EntityMessage> message = std::make_shared<EntityMessage>(buffer, size);

    std::shared_ptr<EntityMessage> response;
    if (message->_op == OP_CREATE) {
        auto entity = server->get_entities().get();
        entity->set_id(message->_entity_id);
        entity->set_player(message->_client_id);
        entity->set_server_index(entity->get_index());
        response = std::make_shared<EntityMessage>(OP_CREATE, entity->get_id(), message->_index, entity->get_player(), entity->get_server_index());
    }
    else if (message->_op == OP_DELETE) {
        ServerEntity entity(message->_entity_id, message->_server_index, message->_client_id, message->_server_index);
        response = std::make_shared<EntityMessage>(OP_DELETE, message->_entity_id, message->_index, message->_client_id, message->_server_index);
        server->get_entities().remove(&entity);
    }

    auto messages = server->get_current_messages();
    messages->push_back(std::static_pointer_cast<Message>(response));
}