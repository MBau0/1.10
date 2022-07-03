#include "Collision.h"

bool Collision::within_region(glm::vec3 a, glm::vec4 b) {
	return a.x >= b.x && a.x <= a.x + a.z &&
		   a.z >= b.y && a.z <= b.y + b.w;
}