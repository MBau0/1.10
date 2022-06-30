#ifndef TRANSFORM_MESSAGE_H
#define TRANSFORM_MESSAGE_H

#include "Message.h"

#include <glm/gtc/matrix_transform.hpp>

struct TransformMessage : public Message {
    TransformMessage();
    TransformMessage(uint32_t unit_id, glm::vec3 position);

    uint32_t _unit_id;
    glm::vec3 _position;
    //glm::vec3 _velocity;
    //glm::vec3 _direction;

    char* data();

    const uint8_t type() const { return TRANSFORM_MESSAGE; }

    static constexpr uint8_t _ID = TRANSFORM_MESSAGE;
    static constexpr uint32_t _SIZE = sizeof(_ID) + sizeof(_unit_id) + sizeof(_position);
};

#endif