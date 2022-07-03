#ifndef ENTITY_MESSAGE_H
#define ENTITY_MESSAGE_H

#include "Message.h"

enum {
    OP_CREATE,
    OP_DELETE
};

struct EntityMessage : public Message {
    EntityMessage(uint8_t op, int entity_id, int index, int client_id);

    EntityMessage(char* buffer, int size);

    uint8_t _op;

    int _entity_id;

    int _index;

    int _client_id;

    char* data();

    bool valid() const;

    const uint8_t type() const { return GAME_ENTITY_MESSAGE; }

    static constexpr uint8_t _ID = GAME_ENTITY_MESSAGE;
};

#endif