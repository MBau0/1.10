#ifndef UNIT_COMPONENT_H
#define UNIT_COMPONENT_H

#include "Component.h"

#include <cstdint>

struct UnitComponent : public Component {
	UnitComponent(Entity* entity = nullptr);

	void copy(Entity* const entity, const UnitComponent& rhs);

	void update();

	static constexpr uint8_t _component_type = UNIT_COMPONENT;

};

#endif