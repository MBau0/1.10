#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>

enum {
    TRANSFORM_MESSAGE
};

struct Message {
    virtual char* data() = 0;

    uint32_t _size = 0;
};

#endif