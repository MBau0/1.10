#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "CompactArray.h"

class Entity;

struct TransformComponent;
struct BuildingComponent;
struct UnitComponent;

class ComponentManager {
public:
    ComponentManager();

    void update();

    TransformComponent* get_transform();

    BuildingComponent* get_building();

    UnitComponent* get_unit();

    void burn_entity_components(Entity* entity);

    void burn_transform(TransformComponent* transform);

    void burn_building(BuildingComponent* building);

    void burn_unit(UnitComponent* unit);

    CompactArray<TransformComponent>* get_transform_components();
private:
    CompactArray<TransformComponent> _transform_components;
    CompactArray<BuildingComponent> _building_components;
    CompactArray<UnitComponent> _unit_components;
    // components are stored individually instead of as a Component*
    // so that they are all next to each other in cache when calling update()
    // Kinda defeats the purpose of using inheritance but o well
};

#endif