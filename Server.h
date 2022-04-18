#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <vector>
#include <thread>
#include <unordered_map>

#include "ServerClient.h"
#include "io_data.h"
#include "Message.h"
#include "MessagePool.h"

#pragma comment(lib, "Ws2_32.lib")

typedef void ( *message_func_server )( char* buffer, uint32_t size, std::vector<Message*>* messages );

class Server {
    public:
        Server();
        ~Server();

        void create_message_table();

        bool start();
        bool stop();

        void main();

        void tick_update();
        void accept_clients();
        void worker_thread();

        void parse_message(PER_IO_OPERATION* context, DWORD bytes_transferred);
    private:
        SOCKET _listen_socket;

        std::vector<ServerClient*>  _clients;

        bool _started;

        HANDLE _iocp;

        std::vector<std::thread> _threads;

        std::unordered_map<uint32_t, message_func_server> _message_table;

        MessagePool _message_pool;

        std::thread _tick_updater;
};

void transform_message(char* buffer, uint32_t size, std::vector<Message*>* message);

#endif
