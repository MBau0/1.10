#include "SceneManager.h"

#include "FileReader.h"
#include "Scene.h"
#include "ProgramManager.h"

static const char MODEL_FILE[] = "Data/Models/models.txt";

SceneManager::SceneManager(ProgramManager* program_manager) :
	_program_manager		( program_manager )
{
	FileReader file(MODEL_FILE, FileReader::int_val);
	
	file.set_section("Models");
	if (file.is_read()) {
		for (auto it = file.s_begin(); it != file.s_end(); ++it) {
			load(it->key_val, it->value);
		}
	}
}

SceneManager::~SceneManager() {
	for (auto it : _scenes) {
		delete it.second;
	}
}

void SceneManager::load(int id, std::string_view path) {
	FileReader model_file(path.data());

	std::string dir = "";
	std::string model = "";
	int program = 0;

	model_file.read(&dir, "dir.s");
	model_file.read(&model, "model.s");
	model_file.read(&program, "program.i");

	Scene* scene = new Scene;
	scene->load_assimp(dir, model);
	scene->attach_program(_program_manager->get(program));

	_scenes.insert(std::pair<int, Scene*>(id, scene));
}

Scene* SceneManager::get(int id) const {
	const auto it = _scenes.find(id);
	return (it != _scenes.end()) ? it->second : nullptr;
}