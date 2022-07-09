#include "EntityMessage.h"

EntityMessage::EntityMessage(uint8_t op, int entity_id, int index, int client_id, int server_index):
    _op           ( op ),
    _entity_id    ( entity_id ),
    _index        ( index ),
    _client_id    ( client_id ),
    _server_index ( server_index )
{
    _id = GAME_ENTITY_MESSAGE;
    _size = sizeof(_op) + sizeof(_entity_id) + sizeof(_index) + sizeof(_client_id) + sizeof(_server_index);
}

EntityMessage::EntityMessage(char* buffer, int size) {
    _id = GAME_ENTITY_MESSAGE;
    _size = size;

    char* ptr = buffer;
    memcpy(&_op, ptr, sizeof(_op));
    ptr += sizeof(_op);

    memcpy(&_entity_id, ptr, sizeof(_entity_id));
    ptr += sizeof(_entity_id);

    memcpy(&_index, ptr, sizeof(_index));
    ptr += sizeof(_index);

    memcpy(&_client_id, ptr, sizeof(_client_id));
    ptr += sizeof(_client_id);

    memcpy(&_server_index, ptr, sizeof(_server_index));
}

char* EntityMessage::data() {
    char* data = new char[PREAMBLE + _size];
    char* ptr = prepend_preamble(data);

    memcpy(ptr, &_op, sizeof(_op));
    ptr += sizeof(_op);

    memcpy(ptr, &_entity_id, sizeof(_entity_id));
    ptr += sizeof(_entity_id);

    memcpy(ptr, &_index, sizeof(_index));
    ptr += sizeof(_index);

    memcpy(ptr, &_client_id, sizeof(_client_id));
    ptr += sizeof(_client_id);

    memcpy(ptr, &_server_index, sizeof(_server_index));
    ptr += sizeof(_server_index);

    return data;
}

bool EntityMessage::valid() const {
    return true;
}