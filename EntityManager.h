#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "ComponentManager.h"
#include "CompactArray.h"

#include "FileReader.h"

#include "Entity.h"

#include <string>
#include <map>

struct Cached {
	Entity* entity;
	uint16_t ref_count;
};

class EntityManager {
public:
	EntityManager();

	~EntityManager();

	void update();

	Entity* create(int id, int player);

	void burn(Entity*& entity);

	void load_cached_entity(int id);

	ComponentManager* get_component_manager();
private:
	std::string get_entity_file(int id) const;

	void load_transform(FileReader& file, Entity* entity);
private:
	CompactArray<Entity> _entities;
	CompactArray<Entity> _entities_cached;

	std::map<int, Cached> _entity_cache;

	ComponentManager _components;
	ComponentManager _components_cache;
};

#endif