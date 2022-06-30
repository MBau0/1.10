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

	Entity* create(int id);

	void burn(Entity*& entity);

	void load_cached_entity(int id);
private:
	std::string get_entity_file(int id) const;

	void load_transform(FileReader& file, Entity* entity);
private:
	CompactArray<Entity> _entities;

	std::map<int, Cached> _entity_cache;

	ComponentManager _components;
};

#endif