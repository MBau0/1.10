#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <winsock2.h>
#include "io_data.h"

struct Message;

class ServerClient {
    public:
        ServerClient();
        ~ServerClient();

        bool connect(SOCKET listen_socket);
        bool associate(HANDLE iocp);

        void send(Message* message);
        void receive();
    
        SOCKET _socket;
        PER_IO_OPERATION _receive_context;
        PER_IO_OPERATION _send_context;
};

#endif
