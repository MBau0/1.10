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
        _total_counter += abs(_direction * _speed);
        if(_total_counter.x >= _total_dist.x && _total_counter.y >= _total_dist.y && _total_counter.z >= _total_dist.z) {
            position = _destination;
            _transform.set_position(position);
            _moving = false;
        }
    }
}

void TransformComponent::move(glm::vec3 dest) {
    auto position = _transform.get_position();
    if (dest == position) {
        return;
    }
    _start = position;
    _direction = dest - position;
    _direction = glm::normalize(_direction);
    _moving = true;
    _destination = dest;
    _total_counter = glm::vec3(0, 0, 0);
    _total_dist = abs(_destination - _start);
}
