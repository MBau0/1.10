#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"

#include "Transform.h"

struct TransformComponent : public Component {
    TransformComponent(Entity* entity = nullptr);

    void update();

    void move(glm::vec3 dest);

    static constexpr uint8_t _component_type = TRANSFORM_COMPONENT;

    Transform _transform;

    float _speed;
    glm::vec3 _direction;
    glm::vec3 _destination;
    glm::vec3 _start;
    glm::vec3 _total_counter;
    glm::vec3 _total_dist;
    bool _moving;
};

#endif
