#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <cstring>

enum {
    SYS_SYNC_MESSAGE,
    GAME_TRANSFORM_MESSAGE,
    GAME_ENTITY_MESSAGE
};

struct Message {
    virtual char* data() = 0;

    virtual bool valid() const = 0;

    virtual const uint8_t type() const = 0;

    char* prepend_preamble(char* data) const;

    uint32_t size() const;

    uint32_t _size = 0;

    uint8_t _id = 0;

    static constexpr uint8_t PREAMBLE = sizeof(_id) + sizeof(_size);
};

#endif