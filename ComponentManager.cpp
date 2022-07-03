#include "ComponentManager.h"

#include <iostream>

#include "TransformComponent.h"
#include "BuildingComponent.h"
#include "UnitComponent.h"

#include "Entity.h"

ComponentManager::ComponentManager() :
    _transform_components       ( 20 ),
    _building_components        ( 20 ),
    _unit_components            ( 20 ) // this needs to be extended
{}

void ComponentManager::update() {
    for(auto it = _transform_components.begin(); it != _transform_components.end(); ++it) {
        it->update();
    }

    for (auto it = _building_components.begin(); it != _building_components.end(); ++it) {
        it->update();
    }

    for (auto it = _unit_components.begin(); it != _unit_components.end(); ++it) {
        it->update();
    }
}

TransformComponent* ComponentManager::get_transform() {
    return _transform_components.get();
}

BuildingComponent* ComponentManager::get_building() {
    return _building_components.get();
}

UnitComponent* ComponentManager::get_unit() {
    return _unit_components.get();
}

void ComponentManager::burn_entity_components(Entity* entity) {
    if (auto transform = entity->get<TransformComponent>()) {
        _transform_components.remove(transform);
    }

    if (auto building = entity->get<BuildingComponent>()) {
        _building_components.remove(building);
    }

    if (auto unit = entity->get<UnitComponent>()) {
        _unit_components.remove(unit);
    }
}

void ComponentManager::burn_transform(TransformComponent* transform) {
    _transform_components.remove(transform);
    transform = nullptr;
}

void ComponentManager::burn_building(BuildingComponent* building) {
    _building_components.remove(building);
    building = nullptr;
}

void ComponentManager::burn_unit(UnitComponent* unit) {
    _unit_components.remove(unit);
    unit = nullptr;
}

CompactArray<TransformComponent>* ComponentManager::get_transform_components() {
    return &_transform_components;
}