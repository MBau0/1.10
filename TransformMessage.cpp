#include "TransformMessage.h"

TransformMessage::TransformMessage(std::vector<uint16_t> indices, glm::vec3 position) :
    _indices          ( indices ),
    _num_indices      ( indices.size() ),
    _position         ( position)
{
    _id = GAME_TRANSFORM_MESSAGE;
    _size = sizeof(_num_indices) + (sizeof(uint16_t) * _indices.size()) + sizeof(_position);
}

TransformMessage::TransformMessage(char* buffer, int size) {
    _id = GAME_TRANSFORM_MESSAGE;
    _size = size;

    char* ptr = buffer;
    memcpy(&_num_indices, ptr, sizeof(_num_indices));
    ptr += sizeof(_num_indices);

    _indices.resize(_num_indices);
    memcpy(&_indices[0], ptr, sizeof(uint16_t) * _num_indices);
    ptr += sizeof(uint16_t) * _num_indices;

    memcpy(&_position[0], ptr, sizeof(float) * 3);
}

char* TransformMessage::data() {
    char* data = new char[PREAMBLE + _size];
    char* ptr = prepend_preamble(data);

    memcpy(ptr, &_num_indices, sizeof(_num_indices));
    ptr += sizeof(_num_indices);

    memcpy(ptr, &_indices[0], sizeof(uint16_t) * _indices.size());
    ptr += sizeof(uint16_t) * _indices.size();

    memcpy(ptr, &_position, sizeof(_position));
    ptr += sizeof(_position);

    return data;
}

bool TransformMessage::valid() const {
    return _indices.size() != 0;
}