#include "TransformComponent.h"

TransformComponent::TransformComponent(Entity* entity) :
    Component ( entity ),
    _speed    ( 0.01f ),
    _moving   ( false )
{}

#include <iostream>
void TransformComponent::update() {
    if(_moving) {
        auto position = _transform.get_position();
        position = position + (_direction * _speed);
        _transform.set_position(position);
        _total = _total + abs(_direction * _speed);
        auto travelled = abs(_destination - position);
        if(_total.x >= travelled.x && _total.y >= travelled.y && _total.z >= travelled.z) {
            _moving = false;
        }
    }
}

void TransformComponent::move(glm::vec3 dest) {
    auto position = _transform.get_position();
    _start = position;
    _direction = dest - position;
    _direction = glm::normalize(_direction);
    _moving = true;
    _destination = dest;
    _total = glm::vec3(0, 0, 0);
}
