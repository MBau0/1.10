#ifndef BUILDING_COMPONENT_H
#define BUILDING_COMPONENT_H

#include "Component.h"

#include <stdint.h>

struct BuildingComponent : public Component {
    BuildingComponent(Entity* entity = nullptr);

    void copy(Entity* const entity, const BuildingComponent& rhs);

    void update();

    static constexpr uint8_t _component_type = BUILDING_COMPONENT;
};

#endif