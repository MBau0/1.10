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

#include "PeriodicTimer.h"

#include "CompactArray.h"

#pragma comment(lib, "Ws2_32.lib")

class Server;
class ServerEntity;

namespace server {
    typedef void ( *message_receive_event )( char* buffer, uint32_t size, Server* server );
};

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

        void receive_message(PER_IO_OPERATION* context, DWORD bytes_transferred);

        void send_to_client(int client_id, std::shared_ptr<Message> messaage);

        void sync(ServerClient* client);

        __time64_t get_tick_time();

        std::vector<std::shared_ptr<Message>>* get_current_messages() const;

        CompactArray<ServerEntity>& get_entities();
    private:
        SOCKET _listen_socket;

        std::vector<ServerClient*>  _clients;

        bool _started;

        HANDLE _iocp;

        std::vector<std::thread> _threads;

        std::unordered_map<uint32_t, server::message_receive_event> _message_receive_events;

        MessagePool _message_pool;

        PeriodicTimer _tick_timer;
        std::thread _tick_updater;

        CompactArray<ServerEntity> _entities;
};

#endif
