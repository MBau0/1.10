#include "UnitComponent.h"

UnitComponent::UnitComponent(Entity* entity) :
	Component		( entity )
{}

void UnitComponent::copy(Entity* const entity, const UnitComponent& rhs) {
	_entity = entity;

	// все
}

void UnitComponent::update() {

}