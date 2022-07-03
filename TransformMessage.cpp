#include "TransformMessage.h"

TransformMessage::TransformMessage(std::vector<uint16_t> unit_ids, glm::vec3 position) :
    _unit_ids        ( unit_ids ),
    _num_ids         ( unit_ids.size() ),
    _position        ( position)
{
    _id = GAME_TRANSFORM_MESSAGE;
    _size = sizeof(_num_ids) + (sizeof(uint16_t) * _unit_ids.size()) + sizeof(_position);
}

TransformMessage::TransformMessage(char* buffer, int size) {
    _id = GAME_TRANSFORM_MESSAGE;
    _size = size;

    char* ptr = buffer;
    memcpy(&_num_ids, ptr, sizeof(_num_ids));
    ptr += sizeof(_num_ids);

    _unit_ids.resize(_num_ids);
    memcpy(&_unit_ids[0], ptr, sizeof(uint16_t) * _num_ids);
    ptr += sizeof(uint16_t) * _num_ids;

    memcpy(&_position[0], ptr, sizeof(float) * 3);
}

char* TransformMessage::data() {
    char* data = new char[PREAMBLE + _size];
    char* ptr = prepend_preamble(data);

    memcpy(ptr, &_num_ids, sizeof(_num_ids));
    ptr += sizeof(_num_ids);

    memcpy(ptr, &_unit_ids[0], sizeof(uint16_t) * _unit_ids.size());
    ptr += sizeof(uint16_t) * _unit_ids.size();

    memcpy(ptr, &_position, sizeof(_position));
    ptr += sizeof(_position);

    return data;
}

bool TransformMessage::valid() const {
    return _unit_ids.size() != 0;
}