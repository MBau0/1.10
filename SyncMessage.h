#ifndef SYNC_MESSAGE_H
#define SYNC_MESSAGE_H

#include "Message.h"

#include "time.h"

struct SyncMessage : public Message {
public:
    SyncMessage(int client_id, __time64_t dom_time, uint8_t step);

    SyncMessage(char* buffer, int size);

    char* data();

    bool valid() const;

    int        _client_id;

    __time64_t _dom_time;
    __time64_t _sub_time;
    __time64_t _offset;

    uint8_t _step;  

    const uint8_t type() const { return SYS_SYNC_MESSAGE; }

    static constexpr uint8_t _ID = SYS_SYNC_MESSAGE;
};

#endif