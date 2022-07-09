#ifndef TRANSFORM_MESSAGE_H
#define TRANSFORM_MESSAGE_H

#include "Message.h"

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

struct TransformMessage : public Message {
    TransformMessage(std::vector<uint16_t> indices, glm::vec3 position);

    TransformMessage(char* buffer, int size);

    std::vector<uint16_t> _indices;
    int _num_indices;
    glm::vec3 _position;

    char* data();

    bool valid() const;

    const uint8_t type() const { return GAME_TRANSFORM_MESSAGE; }

    static constexpr uint8_t _ID = GAME_TRANSFORM_MESSAGE;
};

#endif