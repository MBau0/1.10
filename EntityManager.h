#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "ComponentManager.h"
#include "CompactArray.h"

#include "FileReader.h"

#include "Entity.h"

#include <string>
#include <map>
#include <memory>

class Client;

struct EntityMessage;
struct TransformMessage;

struct Cached {
	Entity* entity;
	uint16_t ref_count;
};

class EntityManager {
public:
	EntityManager(Client* client);

	~EntityManager();

	void update();

	Entity* create(int id, int player, int server_index = -1);

	void burn(Entity* entity);

	void load_cached_entity(int id);

	void entity_message(std::shared_ptr<EntityMessage> message);

	void transform_message(std::shared_ptr<TransformMessage> message);

	ComponentManager* get_component_manager();
private:
	std::string get_entity_file(int id) const;

	void load_transform(FileReader& file, Entity* entity);
private:
	CompactArray<Entity> _entities;
	CompactArray<Entity> _entities_cached;

	std::map<int, Cached> _entity_cache;
	std::map<int, int> _server_client_index;

	ComponentManager _components;
	ComponentManager _components_cache;

	Client* _client;
};

#endif