#include "Message.h"

char* Message::prepend_preamble(char* data) const {
    memcpy(data, &_id, sizeof(_id));
    memcpy(data + sizeof(_id), &_size, sizeof(_size));
    return data + PREAMBLE;
}

uint32_t Message::size() const {
    return (uint32_t)PREAMBLE + _size;
}