#include "Client.h"

#include <Windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <iostream>

#include "Debug.h"

#include "Message.h"
#include "TransformMessage.h"

#define DEFAULT_HOSTNAME "192.168.1.4"
#define DEFAULT_PORT "23001"

constexpr uint32_t message_size(uint8_t id) {
    switch(id) {
    case TRANSFORM_MESSAGE:
        return TransformMessage::_SIZE;
    }

    std::cout << "BAD MESSAGE ID\n";
    return 0;
}

Client::Client() :
    _socket(INVALID_SOCKET)
{
    create_message_table();

    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);
}

Client::~Client() {
    closesocket(_socket);
    WSACleanup();
}

void Client::create_message_table() {
    _message_table.emplace(TRANSFORM_MESSAGE, transform_message);
}

void Client::n_connect() {
    struct addrinfo* result = nullptr;
    struct addrinfo* ptr = nullptr;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int res = getaddrinfo(DEFAULT_HOSTNAME, DEFAULT_PORT, &hints, &result);
    ptr = result;

    for(ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        _socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(_socket == INVALID_SOCKET) {
            printf("socket failed : %d\n", WSAGetLastError());
        }
        break;
    }

    printf("Connecting...\n");
    res = connect(_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if(res == SOCKET_ERROR) {
        printf("connect failed : %d\n", WSAGetLastError());
        closesocket(_socket);
    }

    freeaddrinfo(result);

    if(_socket == INVALID_SOCKET) {
        printf("unable to connect\n");
    }

    printf("done\n");
}

void Client::n_send(Message* message) {
    char* data = message->data();
    size_t total_sent = 0;
    size_t bytes_left = message->_size;

    while(total_sent < bytes_left) {
        int sent = send(_socket, data + total_sent, bytes_left, 0);
        if(sent == SOCKET_ERROR) {
            printf("send failed : %d\n", WSAGetLastError());
        }
        total_sent += sent;
        bytes_left -= sent;
        std::cout << "bytes sent: " << total_sent << std::endl;
    }
    std::cout << "Sending: " << data << std::endl;

    delete[] data;
}

void Client::n_receieve() {
    char buf[4096];
    char* ptr = buf;
    size_t buf_length = 4096;
    size_t bytes_handled = 0;
    size_t bytes_remaining = 0;
    int r_recv = 0;
    memset(buf, 0, buf_length);

    do {
        bytes_handled = 0;
        r_recv = recv(_socket, buf + bytes_remaining, buf_length - bytes_remaining, 0);

        if(r_recv > 0 || bytes_remaining > 0) {
            ptr = buf;
            bytes_remaining += r_recv;
            uint32_t size = message_size(*ptr);

            while(bytes_remaining >= size) {

                parse_message(ptr, size);

                bytes_handled += size;
                bytes_remaining -= size;
                ptr = buf + bytes_handled;
                
                size = message_size(*ptr);
                
            }

            memcpy(buf, ptr, bytes_remaining);

        }
        else if(r_recv == 0) {
            printf("Close Connection\n");
        }
        else {
            printf("Receive Error\n");
        }

    } while(r_recv > 0);
}

void Client::parse_message(char* data, uint32_t size) {
    char* ptr = data;
    uint8_t message_id;
    memcpy(&message_id, ptr, sizeof(message_id));
    ptr += sizeof(message_id);
    size -= sizeof(message_id);
    std::cout << "message_id: " << (int) message_id << "\n";

    if(_message_table.count(message_id)) {
        _message_table.at(message_id)( ptr, size );
    }
}

void transform_message(char* buffer, uint32_t size) {
    std::cout << "I AM READING A TRANSFORM MESSSAGE COMMING FROM TEH SERVER WTF\n";
}