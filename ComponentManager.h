#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "ComponentArray.h"
#include "Component.h"
#include "TransformComponent.h"

class ComponentManager {
public:
    ComponentManager();

    void update();

    TransformComponent* get_transform();

private:
    ComponentArray<TransformComponent> _transform_components;
};

#endif