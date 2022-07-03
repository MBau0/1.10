#include "UnitSelection.h"

#include "Program.h"

#include "TransformComponent.h"

#include "Collision.h"

#include "Entity.h"

UnitSelection::UnitSelection(int player, CompactArray<TransformComponent>* transform_components) :
	_player						( player ),
	_transform_components		( transform_components )
{}

void UnitSelection::start(glm::vec2 view, glm::vec3 world) {
	_view_end = view;
	_view_start = view;
	_world_end = world;
	_world_start = world;
	_selection.clear();
}

void UnitSelection::end(glm::vec2 view, glm::vec3 world) {
	_view_end = view;
	_world_end = world;
}

#include <iostream>
void UnitSelection::finalize() {
	glm::vec4 region(
		_world_start.x,
		_world_start.z,
		_world_end.x - _world_start.x,
		_world_end.z - _world_start.z
	);

	if (region.w < 0) {
		region.w = abs(region.w);
		region.y = region.y - region.w;
	}

	if (region.z < 0) {
		region.z = abs(region.z);
		region.x = region.x - region.z;
	}

	for (auto it = _transform_components->begin(); it != _transform_components->end(); ++it) {
		if (it->_entity->get_player() == _player && 
			Collision::within_region(it->_transform.get_position(), region)) {
			_selection.push_back(it->_entity);
		}
	}
}

void UnitSelection::draw(Program* program) {
	program->use();

	glUniform2fv(program->location("start"), 1, &_view_start[0]);
	glUniform2fv(program->location("end"), 1, &_view_end[0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1);
	glDisable(GL_BLEND);
}

const std::vector<Entity*>& UnitSelection::get() const {
	return _selection;
}

const std::vector<uint16_t> UnitSelection::get_ids() const {
	std::vector<uint16_t> ids;
	ids.reserve(_selection.size());
	for (auto ptr : _selection) {
		ids.push_back(ptr->get_id());
	}

	return ids;
}