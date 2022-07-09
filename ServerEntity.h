#ifndef SERVER_ENTITY_H
#define SERVER_ENTITY_H

#include <vector>

class ServerEntity {
public:
	ServerEntity();

	ServerEntity(int id, int index, int player, int server_index);

	int get_id() const;

	void set_id(int id);

	int get_index() const;

	void set_index(int index);

	int get_player() const;

	void set_player(int player);

	int get_server_index() const;

	void set_server_index(int server_index);
private:
	int _id;
	int _index;
	int _player;
	int _server_index;
};

#endif