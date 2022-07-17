#ifndef EMPTY_ACTION_H
#define EMPTY_ACTION_H

#include "Action.h"

class EmptyAction : public Action {
public:
	EmptyAction(Entity* entity, int id, std::string name, GLuint image);

	void execute();
private:
};

#endif