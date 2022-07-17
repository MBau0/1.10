#include "EmptyAction.h"

EmptyAction::EmptyAction(Entity* entity, int id, std::string name, GLuint image) :
	Action(entity, id, name, image)
{}

void EmptyAction::execute() {}