#include "Action.h"

Action::Action(Entity* entity, int id, std::string name, GLuint image) :
	_entity		( entity ),
	_id			( id ),
	_name		( name ),
	_image		( image )
{}

GLuint Action::get_image() const {
	return _image;
}

void Action::set_entity(Entity* entity) {
	_entity = entity;
}