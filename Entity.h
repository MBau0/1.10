#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"

#include <array>
#include <cassert>

class Entity {
public:
    Entity();
    ~Entity();

    template<typename _Component>
    void attach(_Component* component) {
        assert(component);
        component->_entity = this;
        _components[component->_component_type] = component;
    }

    template<typename _Component>
    _Component* get() {
        return (_Component*) _components[_Component::_component_type];
    }

private:
    std::array<Component*, TOTAL_COMPONENTS> _components { nullptr };
};

#endif