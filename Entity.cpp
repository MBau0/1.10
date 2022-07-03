#include "Entity.h"

Entity::Entity() :
	_id			( -1 ),
	_index		( -1 ),
	_player		(  0 )
{}

Entity::~Entity() {

}

int Entity::get_id() const {
	return _id;
}

void Entity::set_id(int id) {
	_id = id;
}

int Entity::get_index() const {
	return _index;
}

void Entity::set_index(int index) {
	_index = index;
}

int Entity::get_player() const {
	return _player;
}

void Entity::set_player(int player) {
	_player = player;
}