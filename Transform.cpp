#include "Transform.h"

Transform::Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) :
	_position		( position ),
	_scale			( scale    ),
	_rotation		( rotation )
{}

Transform::Transform(const Transform& rhs) :
	_position			( rhs._position				),
	_scale				( rhs._scale				),
	_rotation			( rhs._rotation				)
{}

Transform& Transform::operator=(const Transform& rhs) {
	_position = rhs._position;
	_scale = rhs._scale;
	_rotation = rhs._rotation;

	return *this;
}

Transform& Transform::operator+=(const Transform& rhs) {
	_position += rhs._position;
	_scale.x = 1 - (_scale.x + rhs._scale.x);
	_scale.y = 1 - (_scale.y + rhs._scale.y);
	_scale.z = 1 - (_scale.z + rhs._scale.z);
	_rotation.x = 1 - (_rotation.x + rhs._rotation.x);
	_rotation.y = 1 - (_rotation.y + rhs._rotation.y);
	_rotation.z = 1 - (_rotation.z + rhs._rotation.z);

	return *this;
}

glm::mat4 Transform::get_model() const {
	const glm::mat4 a = glm::mat4(1);
	return
		glm::translate(a, _position) *
		(glm::rotate(a, glm::radians(_rotation.x), glm::vec3(1, 0, 0)) *
		glm::rotate(a, glm::radians(_rotation.y), glm::vec3(0, 1, 0)) *
		glm::rotate(a, glm::radians(_rotation.z), glm::vec3(0, 0, 1))) *
		glm::scale(a, _scale);
}