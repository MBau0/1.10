#ifndef COLLISION_H
#define COLLISION_H

#include <glm/gtc/matrix_transform.hpp>

class Collision {
public:
	// a = point
	// b = rect
	static bool within_region(glm::vec3 a, glm::vec4 b);
private:
};

#endif