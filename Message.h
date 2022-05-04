#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>

enum {
    SYNC_MESSAGE,
    TRANSFORM_MESSAGE
};

struct Message {
    virtual char* data() = 0;

    constexpr virtual uint8_t type() const = 0;

    uint32_t _size = 0;
};

#endif