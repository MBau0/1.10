#ifndef SYNC_MESSAGE_H
#define SYNC_MESSAGE_H

#include "Message.h"

#include "time.h"

struct SyncMessage : public Message {
public:
    SyncMessage();
    SyncMessage(int client_id, __time64_t dom_time, uint8_t step);
    char* data();

    int        _client_id;

    __time64_t _dom_time;
    __time64_t _sub_time;
    __time64_t _offset;

    uint8_t _step;  

    constexpr uint8_t type() const { return SYNC_MESSAGE; }

    static constexpr uint8_t _ID = SYNC_MESSAGE;
    static constexpr uint32_t _SIZE = sizeof(_ID) + sizeof(_client_id) +  sizeof(_dom_time) + sizeof(_sub_time) + sizeof(_offset) + sizeof(_step);
private:
};

#endif