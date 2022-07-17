#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#include <stdint.h>
#include <vector>
#include <memory>

class Action;

struct ActionComponent : public Component {
    ActionComponent(Entity* entity = nullptr);

    void copy(Entity* const entity, const ActionComponent& rhs);

    void update();

    static constexpr uint8_t _component_type = ACTION_COMPONENT;

    std::vector<Action*> _actions;
};

#endif