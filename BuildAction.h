#ifndef BUILD_ACTION_H
#define BUILD_ACTION_H

#include "Action.h"

class BuildAction : public Action {
public:
	BuildAction(Entity* entity, int id, std::string name, GLuint image);

	void execute();
private:
};

#endif