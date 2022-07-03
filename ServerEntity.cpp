#include "ServerEntity.h"

ServerEntity::ServerEntity() :
	_id					( -1 ),
	_index				( -1 ),
	_player				( -1 )
{}

ServerEntity::ServerEntity(int id, int index, int player) :
	_id					( id ),
	_index				( index ),
	_player				( player )
{}

void ServerEntity::set_id(int id) {
	_id = id;
}

int ServerEntity::get_id() const {
	return _id;
}

void ServerEntity::set_index(int index) {
	_index = index;
}

int ServerEntity::get_index() const {
	return _index;
}

void ServerEntity::set_player(int player) {
	_player = player;
}

int ServerEntity::get_player() const {
	return _player;
}