#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>

#include "Message.h"
#include "MessagePool.h"

#include "PeriodicTimer.h"

#include <unordered_map>

class Client;

namespace client {
    typedef void ( *message_receive_event )( char* buffer, uint32_t size, Client* client );
    typedef void ( *message_event ) ( std::shared_ptr<Message> message, Client* client );
};

class Client {
    public:
        Client();
        ~Client();

        void create_message_table();

        void n_connect();
        void n_send(std::shared_ptr<Message> message);
        void n_receieve();

        void tick_update();

        void receive_message(char* data, uint32_t size);

        std::vector<std::shared_ptr<Message>>* get_current_messages() const;

        void set_message_receiver(std::vector<std::shared_ptr<Message>>* message_receiver);
        std::vector<std::shared_ptr<Message>>* get_message_receiver();

        PeriodicTimer& get_tick_timer();
    private:
        SOCKET _socket;

        std::unordered_map<uint32_t, client::message_receive_event>  _message_receive_events;
        std::unordered_map<uint32_t, client::message_event>          _message_events;

        MessagePool _message_pool;

        std::vector<std::shared_ptr<Message>>* _message_receiver;

        PeriodicTimer _tick_timer;
};

#endif
