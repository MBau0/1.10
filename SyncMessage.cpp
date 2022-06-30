#include "SyncMessage.h"

#include <cstring>

SyncMessage::SyncMessage() :
    _client_id      ( -1 ),
    _dom_time       ( 0 ),
    _sub_time       ( 0 ),
    _offset         ( 0 ),
    _step           ( 0 )
{
    _size = _SIZE;
}

SyncMessage::SyncMessage(int client_id, __time64_t dom_time, uint8_t step) :
    _client_id      ( client_id ),
    _dom_time       ( dom_time ),
    _sub_time       ( 0 ),
    _offset         ( 0 ),
    _step           ( step )
{
    _size = _SIZE;
}

char* SyncMessage::data() {
    char* data = new char[_SIZE];
    char* ptr = data;
    memcpy(ptr, &_ID, sizeof(_ID));
    ptr += sizeof(_ID);
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