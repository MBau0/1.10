#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>

#include "Message.h"
#include <unordered_map>

typedef void ( *message_func_client )( char* buffer, uint32_t size );

class Client {
    public:
        Client();
        ~Client();

        void create_message_table();

        void n_connect();
        void n_send(Message* message);
        void n_receieve();

        void parse_message(char* data, uint32_t size);
    private:
        SOCKET _socket;

        std::unordered_map<uint32_t, message_func_client> _message_table;
};

void transform_message(char* buffer, uint32_t size);

#endif
