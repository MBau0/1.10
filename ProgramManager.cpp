#include "ProgramManager.h"

#include "FileReader.h"
#include "Program.h"
#include "Camera.h"

static const char SHADER_FILE[] = "Data/Shaders/shaders.txt";

ProgramManager::ProgramManager() {
	FileReader file(SHADER_FILE, FileReader::int_val);

	file.set_section("Shaders");
	if (file.is_read()) {
		for (auto it = file.s_begin(); it != file.s_end(); ++it) {
			load(it->key_val, it->value);
		}
	}


}

ProgramManager::~ProgramManager() {
	for (auto it : _programs) {
		delete it.second;
	}
}

void ProgramManager::load(int id, std::string_view path) {
	Program* program = new Program;
	program->load(path);
	_programs.insert(std::pair<int, Program*>(id, program));
}

const Program* ProgramManager::get(int id) const {
	const auto it = _programs.find(id);
	return (it != _programs.end()) ? it->second : nullptr;
}
 
void ProgramManager::attach_camera(Camera* camera) const {
	for (const auto it : _programs) {
		camera->attach_program(it.second);
	}
}