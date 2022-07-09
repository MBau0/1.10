#include "EntityManager.h"

#include "TransformComponent.h"
#include "BuildingComponent.h"
#include "UnitComponent.h"

#include "EntityMessage.h"
#include "TransformMessage.h"

#include "Client.h"

static const char ENTITY_FILE[] = "Data\\Entities\\entities.txt";
static const char ENTITY_SECTION[] = "Entity";
static const char TRANSFORM_SECTION[] = "Transform";
static const char BUILDING_SECTION[] = "Building";
static const char UNIT_SECTION[] = "Unit";

EntityManager::EntityManager(Client* client) :
	_client			 ( client ),
	_entities		 ( 200 ),
	_entities_cached ( 200 )
{}

EntityManager::~EntityManager() {
	for (auto& pair : _entity_cache) {
		delete pair.second.entity;
	}
}

void EntityManager::update() {
	_components.update();
}

Entity* EntityManager::create(int id, int player, int server_index) {
	if (_entity_cache.find(id) == _entity_cache.end()) {
		load_cached_entity(id);
	}

	auto entity = _entities.get();
	auto& cached = _entity_cache.at(id);

	entity->set_id(cached.entity->get_id());
	entity->set_player(player);
	entity->set_server_index(server_index);

	if (server_index == -1) {
		auto message = std::make_shared<EntityMessage>(OP_CREATE, id, entity->get_index(), player, entity->get_server_index());
		_client->n_send(message);
	}

	for (int i = 0; i < entity->_components.size(); ++i) {
		if (cached.entity->_components[i] != nullptr) {
			bool attach = (entity->_components[i] == nullptr);
			switch (i) {
			case TRANSFORM_COMPONENT:
				if (attach) entity->attach(_components.get_transform());
				static_cast<TransformComponent*>(entity->_components[i])->copy(entity, *static_cast<TransformComponent*>(cached.entity->_components[i]));  // пиздец
				break;
			case BUILDING_COMPONENT:
				if (attach) entity->attach(_components.get_building());
				static_cast<BuildingComponent*>(entity->_components[i])->copy(entity, *static_cast<BuildingComponent*>(cached.entity->_components[i]));  // пиздец
				break;
			case UNIT_COMPONENT:
				if (attach) entity->attach(_components.get_unit());
				static_cast<UnitComponent*>(entity->_components[i])->copy(entity, *static_cast<UnitComponent*>(cached.entity->_components[i]));  // пиздец

				break;
			}
		}
		else if (cached.entity->_components[i] == nullptr && entity->_components[i] != nullptr) {
			switch (i) {
			case TRANSFORM_COMPONENT:
				_components.burn_transform(entity->get<TransformComponent>());
				break;
			case BUILDING_COMPONENT:
				_components.burn_building(entity->get<BuildingComponent>());
				break;
			case UNIT_COMPONENT:
				_components.burn_unit(entity->get<UnitComponent>());
				break;
			}
			entity->_components[i] = nullptr;
		}
	}

	++cached.ref_count;
	return entity;
}

void EntityManager::burn(Entity* entity) {
	auto message = std::make_shared<EntityMessage>(OP_DELETE, entity->get_id(), entity->get_index(), entity->get_player(), entity->get_server_index());
	_client->n_send(message);

	auto& cached = _entity_cache.at(entity->get_id());
	if (--cached.ref_count ==  0) {
		_components_cache.burn_entity_components(cached.entity);
		_entities_cached.remove(cached.entity);
		_entity_cache.erase(entity->get_id());
	}

	_components.burn_entity_components(entity);
	_entities.remove(entity);
	entity = nullptr;
}

std::string EntityManager::get_entity_file(int id) const {
	FileReader file(ENTITY_FILE, &FileReader::int_val);

	file.set_section(ENTITY_SECTION);
	std::string entity_file = "";
	file.read(&entity_file, id);

	return entity_file;
}

void EntityManager::load_cached_entity(int id) {
	std::string entity_file = get_entity_file(id);

	Cached cached;
	cached.entity = _entities_cached.get();
	cached.entity->set_id(id);
	cached.ref_count = 0;

	FileReader file(entity_file.c_str());
	if (file.set_section(TRANSFORM_SECTION)) {
		load_transform(file, cached.entity);
	}

	if (file.set_section(BUILDING_SECTION)) {

	}

	if (file.set_section(UNIT_SECTION)) {

	}

	_entity_cache.insert(std::pair<int, Cached>(id, cached));
}

void EntityManager::load_transform(FileReader& file, Entity* entity) {
	auto transform = _components_cache.get_transform();

	file.read(&transform->_speed, "speed.f");
	float scale = 0.0f;
	file.read(&scale, "scale.f");
	transform->_transform.set_scale(glm::vec3(scale, scale, scale));

	entity->attach(transform);
}

void EntityManager::entity_message(std::shared_ptr<EntityMessage> message) {
	if (message->_op == OP_CREATE) {
		// if < _back && not in _indicies is valid entity then entity.at(index).server_index = server_index !
		if (message->_client_id == _client->get_id()) {
			// we created the entity
			if (auto entity = _entities.at(message->_index)) {
				entity->set_server_index(message->_server_index);
				_server_client_index.insert(std::pair<int, int>(message->_server_index, message->_index));
			}
		}
		else {
			// someone else made it, make it here
			auto entity = create(message->_entity_id, message->_client_id, message->_server_index);
		}
	}
	else if (message->_op == OP_DELETE) {
		if (message->_client_id != _client->get_id()) {
			Entity entity;
			auto index = _server_client_index.extract(message->_server_index);
			entity.set_index(index.mapped());
			entity.set_id(message->_entity_id);
			entity.set_player(message->_client_id);
			burn(&entity);
		}
	}
}

#include <iostream>
void EntityManager::transform_message(std::shared_ptr<TransformMessage> message) {
	std::cout << "move?\n";
	for (auto index : message->_indices) {
		auto it = _server_client_index.find(index);
		if (it != _server_client_index.end()) {
			if (auto transform = _entities.at(it->second)->get<TransformComponent>()) {
				transform->move(message->_position);
			}
		}
	}
}

ComponentManager* EntityManager::get_component_manager() {
	return &_components;
}