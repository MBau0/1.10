#include "EntityManager.h"

#include "TransformComponent.h"
#include "BuildingComponent.h"
#include "UnitComponent.h"
#include "ActionComponent.h"

#include "EntityMessage.h"
#include "TransformMessage.h"

#include "Client.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Action.h"

static const char ENTITY_FILE[] = "Data\\Entities\\entities.txt";
static const char ENTITY_SECTION[] = "Entity";
static const char TRANSFORM_SECTION[] = "Transform";
static const char BUILDING_SECTION[] = "Building";
static const char UNIT_SECTION[] = "Unit";
static const char ACTION_SECTION[] = "Action";

EntityManager::EntityManager(Client* client, SceneManager* scene_manager) :
	_client			 ( client ),
	_scene_manager	 ( scene_manager ),
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
	entity->set_scene(cached.entity->get_scene());

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
			case ACTION_COMPONENT:
				if (attach) entity->attach(_components.get_action());
				static_cast<ActionComponent*>(entity->_components[i])->copy(entity, *static_cast<ActionComponent*>(cached.entity->_components[i]));  // пиздец
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
			case ACTION_COMPONENT:
				_components.burn_action(entity->get<ActionComponent>());
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
	file.set_section("Entity");
	
	std::string name = "";
	int scene = 0;

	file.read(&name, "name.s");
	file.read(&scene, "scene.i");

	cached.entity->set_scene(scene);

	if (file.set_section(TRANSFORM_SECTION)) {
		load_transform(file, cached.entity);
	}

	if (file.set_section(BUILDING_SECTION)) {

	}

	if (file.set_section(UNIT_SECTION)) {

	}

	if (file.set_section(ACTION_SECTION)) {
		load_action(file, cached.entity);
	}

	_entity_cache.insert(std::pair<int, Cached>(id, cached));
}

void EntityManager::load_transform(FileReader& file, Entity* entity) {
	auto transform = _components_cache.get_transform();

	file.read(&transform->_speed, "speed.f");
	float scale = 0.0f;
	file.read(&scale, "scale.f");
	transform->_transform._scale = glm::vec3(scale, scale, scale);

	entity->attach(transform);
}

void EntityManager::load_action(FileReader& file, Entity* entity) {
	auto action = _components_cache.get_action();

	int a[12];
	file.read(&a[0], "a0.i");
	file.read(&a[1], "a1.i");
	file.read(&a[2], "a2.i");
	file.read(&a[3], "a3.i");
	file.read(&a[4], "a4.i");
	file.read(&a[5], "a5.i");
	file.read(&a[6], "a6.i");
	file.read(&a[7], "a7.i");
	file.read(&a[8], "a8.i");
	file.read(&a[9], "a9.i");
	file.read(&a[10], "a10.i");
	file.read(&a[11], "a11.i");
	action->_actions = load_actions(a, entity);

	entity->attach(action);
}

std::vector<Action*> EntityManager::load_actions(int a[12], Entity* entity) {
	std::vector<Action*> actions;
	for (int i = 0; i < 12; ++i) {
		auto action = _action_manager.get(a[i]);
		if (action) {
			actions.push_back(action);
		}
		else {
			actions.push_back(_action_manager.get(0));
		}
		actions[i]->set_entity(entity);
	}

	return actions;
}

void EntityManager::entity_message(std::shared_ptr<EntityMessage> message) {
	if (message->_op == OP_CREATE) {
		// if < _back && not in _indicies is valid entity then entity.at(index).server_index = server_index !
		if (message->_client_id == _client->get_id()) {
			// we created the entity
			if (auto entity = _entities.at(message->_index)) {
				entity->set_server_index(message->_server_index);
				_server_client_index.insert(std::pair<int, int>(message->_server_index, entity->get_index()));
			}
		}
		else {
			// someone else made it, make it here
			auto entity = create(message->_entity_id, message->_client_id, message->_server_index);
			_server_client_index.insert(std::pair<int, int>(message->_server_index, entity->get_index()));
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

void EntityManager::transform_message(std::shared_ptr<TransformMessage> message) {
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

void EntityManager::draw() {
	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		if (auto scene = _scene_manager->get(it->get_scene())) {
			if (auto transform = it->get<TransformComponent>()) {
				scene->draw(4, transform->_transform);
			}
		}
	}
} 