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
#include "SyncMessage.h"
#include "EntityMessage.h"

#include "PeriodicTimer.h"

#include "Benchmark.h"

#include <mutex>

#define DEFAULT_HOSTNAME "192.168.1.4"
#define DEFAULT_PORT "23001"

namespace client 
{

    std::mutex message_pool_mutex;

    void receive_transform_message(char* buffer, uint32_t size, Client* client);

    void receive_sync_message(char* buffer, uint32_t size, Client* client);

    void receive_entity_message(char* buffer, uint32_t size, Client* client);

    void handle_transform_message(std::shared_ptr<Message> message, Client* client);

    void handle_entity_message(std::shared_ptr<Message> message, Client* client);

};

Client::Client() :
    _socket             ( INVALID_SOCKET ),
    _tick_timer         ( 50 ),
    _id                 ( -1 )
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
    _message_receive_events.emplace(SYS_SYNC_MESSAGE, client::receive_sync_message);
    _message_receive_events.emplace(GAME_TRANSFORM_MESSAGE, client::receive_transform_message);
    _message_receive_events.emplace(GAME_ENTITY_MESSAGE, &client::receive_entity_message);

    _message_events.emplace(GAME_TRANSFORM_MESSAGE, client::handle_transform_message);
    _message_events.emplace(GAME_ENTITY_MESSAGE, &client::handle_entity_message);
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

void Client::n_send(std::shared_ptr<Message> message) {
    if (!message->valid()) {
        return;
    }

    char* data = message->data();
    size_t total_sent = 0;
    size_t bytes_left = message->size();

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

        if(r_recv > Message::PREAMBLE || bytes_remaining > Message::PREAMBLE) {
            ptr = buf;
            bytes_remaining += r_recv;

            uint8_t id;
            memcpy(&id, ptr, sizeof(uint8_t));
            ptr += sizeof(uint8_t);

            uint32_t size;
            memcpy(&size, ptr, sizeof(uint32_t));

            ptr += sizeof(uint32_t);

            while(bytes_remaining >= Message::PREAMBLE + size) {

                receive_message(ptr, id, size);

                bytes_handled += Message::PREAMBLE + size;
                bytes_remaining -= Message::PREAMBLE + size;
                ptr = buf + bytes_handled;
               
                memcpy(&id, ptr, sizeof(uint8_t));
                ptr += sizeof(uint8_t);

                memcpy(&size, ptr, sizeof(uint32_t));
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

void Client::tick_update() {
    while(1) {
        if(_tick_timer.alert()) {
            _message_pool.swap(client::message_pool_mutex);

            for(auto message : *_message_pool.get_previous()) {
                 if(_message_events.count(message->type())) {
                     _message_events.at(message->type()) ( message, this );
                 }
            }

            _message_pool.clear_previous();
        }
        else {
            Sleep(_tick_timer.remaining());
        }
    }
}

PeriodicTimer& Client::get_tick_timer() {
    return _tick_timer;
}

void Client::receive_message(char* data, uint8_t id, uint32_t size) {
    char* ptr = data;
    std::cout << "id: " << (int) id << "\n";

    if(_message_receive_events.count(id)) {
        std::lock_guard<std::mutex> guard(client::message_pool_mutex);
        _message_receive_events.at(id)( ptr, size, this );
    }
}

std::vector<std::shared_ptr<Message>>* Client::get_current_messages() const {
    return _message_pool.get_current();
}

void Client::set_message_receiver(std::vector<std::shared_ptr<Message>>* message_receiver) {
    _message_receiver = message_receiver;
}

std::vector<std::shared_ptr<Message>>* Client::get_message_receiver() {
    return _message_receiver;
}

void client::receive_transform_message(char* buffer, uint32_t size, Client* client) {
    std::cout << "Transform Message\n";

    std::shared_ptr<TransformMessage> message = std::make_shared<TransformMessage>(buffer, size);

    auto messages = client->get_current_messages();
    messages->push_back(std::static_pointer_cast<Message>( message ));
}

void client::handle_transform_message(std::shared_ptr<Message> message, Client* client) {
    client->get_message_receiver()->push_back(message);
}

void client::receive_sync_message(char* buffer, uint32_t size, Client* client) {
    std::cout << "Sync Message\n";

    auto message = std::make_shared<SyncMessage>(buffer, size);

    if(message->_step == 0) {
        client->set_id(message->_client_id);
        __time64_t time = client->get_tick_timer().time();
        __time64_t delta = time - message->_dom_time;
        message->_dom_time = delta;
        message->_sub_time = time;
        message->_step = 1;

        client->n_send(std::static_pointer_cast<Message>( message ));
    }
    else if(message->_step == 2) {
        client->get_tick_timer().offset(message->_offset);
    }
}

void client::receive_entity_message(char* buffer, uint32_t size, Client* client) {
    std::cout << "Entity Message\n";
    auto message = std::make_shared<EntityMessage>(buffer, size);

    auto messages = client->get_current_messages();
    messages->push_back(std::static_pointer_cast<Message>(message));
}

void client::handle_entity_message(std::shared_ptr<Message> message, Client* client) {
    client->get_message_receiver()->push_back(message);
}

void Client::set_id(int id) {
    _id = id;
}

int Client::get_id() const {
    return _id;
}