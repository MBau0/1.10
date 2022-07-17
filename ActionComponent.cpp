#include "ActionComponent.h"

#include "Action.h"

ActionComponent::ActionComponent(Entity* entity) :
	Component(entity)
{}

void ActionComponent::copy(Entity* const entity, const ActionComponent& rhs) {
	_entity = entity;

	_actions = rhs._actions;
	for (auto action : _actions) {
		action->set_entity(_entity);
	}

	//все
}

void ActionComponent::update() {

}