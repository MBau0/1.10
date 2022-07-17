#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Transform.h"

#include <map>
#include <string_view>

class Scene;
class ProgramManager;

class SceneManager {
public:
	SceneManager(ProgramManager* program_manager);

	~SceneManager();

	Scene* get(int id) const;
private:
	void load(int id, std::string_view path);

	std::map<int, Scene*> _scenes;

	ProgramManager* _program_manager;
};

#endif