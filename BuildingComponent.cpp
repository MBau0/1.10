#include "BuildingComponent.h"

BuildingComponent::BuildingComponent(Entity* entity) :
	Component			( entity )
{}

void BuildingComponent::copy(Entity* const entity, const BuildingComponent& rhs) {
	_entity = entity;

	//все
}


void BuildingComponent::update() {

}