#include "ComponentManager.h"

#include <iostream>
ComponentManager::ComponentManager() :
    _transform_components       ( 20 )
{}

void ComponentManager::update() {
    for(auto it = _transform_components.begin(); it != _transform_components.end(); ++it) {
        auto end = _transform_components.end();

        it->update();
    }
}

TransformComponent* ComponentManager::get_transform() {
    return _transform_components.get();
}