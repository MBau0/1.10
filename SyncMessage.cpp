#include "SyncMessage.h"

#include <cstring>

SyncMessage::SyncMessage(int client_id, __time64_t dom_time, uint8_t step) :
    _client_id      ( client_id ),
    _dom_time       ( dom_time ),
    _sub_time       ( 0 ),
    _offset         ( 0 ),
    _step           ( step )
{
    _id = SYS_SYNC_MESSAGE;
    _size = sizeof(_client_id) + sizeof(_dom_time) + sizeof(_sub_time) + sizeof(_offset) + sizeof(_step);
}

SyncMessage::SyncMessage(char* buffer, int size) {
    _id = SYS_SYNC_MESSAGE;
    _size = size;

    char* ptr = buffer;
    memcpy(&_client_id, ptr, sizeof(_client_id));
    ptr += sizeof(_client_id);

    memcpy(&_dom_time, ptr, sizeof(_dom_time));
    ptr += sizeof(_dom_time);

    memcpy(&_sub_time, ptr, sizeof(_sub_time));
    ptr += sizeof(_sub_time);

    memcpy(&_offset, ptr, sizeof(_offset));
    ptr += sizeof(_offset);

    memcpy(&_step, ptr, sizeof(_step));
    ptr += sizeof(_step);
}

char* SyncMessage::data() {
    char* data = new char[PREAMBLE + _size];
    char* ptr = prepend_preamble(data);

    memcpy(ptr, &_client_id, sizeof(_client_id));
    ptr += sizeof(_client_id);

    memcpy(ptr, &_dom_time, sizeof(_dom_time));
    ptr += sizeof(_dom_time);

    memcpy(ptr, &_sub_time, sizeof(_sub_time));
    ptr += sizeof(_sub_time);

    memcpy(ptr, &_offset, sizeof(_offset));
    ptr += sizeof(_offset);

    memcpy(ptr, &_step, sizeof(_step));
    ptr += sizeof(_step);

    return data;
} 

bool SyncMessage::valid() const {
    return true;
}