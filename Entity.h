#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"

#include <array>
#include <cassert>

class ComponentManager;

class Entity {
public:
    Entity();

    ~Entity();

    Entity& operator=(const Entity& rhs) = delete;

    Entity(const Entity&& rhs) = delete;

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

    int get_id() const;

    void set_id(int id);

    int get_index() const;

    void set_index(int index);

    int get_player() const;

    void set_player(int player);

    int get_server_index() const;

    void set_server_index(int server_index);

    std::array<Component*, TOTAL_COMPONENTS> _components{ nullptr };
private:
    int _id;
    int _index;
    int _player;
    int _server_index;
};

#endif