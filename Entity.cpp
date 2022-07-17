#include "Entity.h"

Entity::Entity() :
	_id			  ( -1 ),
	_index		  ( -1 ),
	_player		  (  0 ),
	_server_index ( -1 ),
	_scene		  ( -1 )
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

int Entity::get_server_index() const {
	return _server_index;
}

void Entity::set_server_index(int server_index) {
	_server_index = server_index;
}

int Entity::get_scene() const {
	return _scene;
}

void Entity::set_scene(int scene) {
	_scene = scene;
}