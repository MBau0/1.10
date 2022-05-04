#include "TransformMessage.h"

#include "Debug.h"

TransformMessage::TransformMessage() {
    _size = _SIZE;
}

TransformMessage::TransformMessage(uint32_t unit_id, glm::vec3 position) :
    _unit_id        ( unit_id ),
    _position       ( position)
{
    _size = _SIZE;
}

char* TransformMessage::data() {
    char* data = new char[_SIZE];
    char* ptr = data;
    memcpy(ptr, &_ID, sizeof(_ID));
    ptr += sizeof(_ID);
    memcpy(ptr, &_unit_id, sizeof(_unit_id));
    ptr += sizeof(_unit_id);
    memcpy(ptr, &_position, sizeof(_position));
    ptr += sizeof(_position);

    return data;
}