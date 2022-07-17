#include "TransformComponent.h"

#include <Windows.h>

TransformComponent::TransformComponent(Entity* entity) :
    Component ( entity ),
    _speed    ( 0.01f ),
    _moving   ( false )
{}

void TransformComponent::copy(Entity* const entity, const TransformComponent& rhs) {
    _entity = entity;

    _transform = rhs._transform;
    _speed = rhs._speed;
    _direction = rhs._direction;
    _destination = rhs._destination;
    _start = rhs._start;
    _total_counter = rhs._total_counter;
    _total_dist = rhs._total_dist;
    _moving = rhs._moving;
    _last_frame = GetTickCount();
}

void TransformComponent::update() {
    if(_moving) {
        const float dt = (GetTickCount() - _last_frame) / 1000.0f;
        auto position = _transform._position;
        position = position + (_direction * (_speed * dt));
        _transform._position = position;
        _total_counter += abs(_direction * (_speed * dt));
        _last_frame = GetTickCount();
        if(_total_counter.x >= _total_dist.x && _total_counter.y >= _total_dist.y && _total_counter.z >= _total_dist.z) {
            position = _destination;
            _transform._position = position;
            _moving = false;
        }
    }
}

void TransformComponent::move(glm::vec3 dest) {
    auto position = _transform._position;
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
    _last_frame = GetTickCount();
}
