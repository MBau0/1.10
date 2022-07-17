#include "ImageManager.h"

#include <SOIL/SOIL2.h>

#include <exception>

#include "FileReader.h"
#include <iostream>

static const char IMAGE_FILE[] = "Data\\Images\\images.txt";
static const char ACTIONS[] = "Actions";

ImageManager::ImageManager() {
	try {
		FileReader file(IMAGE_FILE, FileReader::int_val);

		file.set_section(ACTIONS);
		auto num_actions = file.get_num_lines(ACTIONS);
		_actions.resize(num_actions);

		for (auto it = file.s_begin(); it != file.s_end(); ++it) {
			for (int i = 0; i < num_actions; ++i) {
				std::string image_path;
				if (file.read(&image_path, i)) {
					std::cout << "Loading... " << image_path << "\n";
					_actions[i] = SOIL_load_OGL_texture(image_path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
				}
			}
		}

	}
	catch (std::exception& e) {
		std::cout << "dumb as hell\n";
	}
}

ImageManager::~ImageManager() {}

const std::vector<GLuint>& ImageManager::get_actions() const {
	return _actions;
}