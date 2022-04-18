#ifndef IO_DATA_H
#define IO_DATA_H

#include <cstdint>

#define DATA_BUFFER_SIZE 4096

#define OP_READ 1
#define OP_WRITE 2

class ServerClient;

struct PER_IO_OPERATION {
    OVERLAPPED _overlapped;
    char _buffer[DATA_BUFFER_SIZE] = {0};
    uint8_t _operation;
    ServerClient* _client;
};


#endif
