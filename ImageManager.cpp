#include "ImageManager.h"

#include <SOIL/SOIL2.h>

#include <exception>

#include "FileReader.h"
#include <iostream>

static const char IMAGE_FILE[] = "Data\\Images\\images.txt";
static const char ABILITIES[] = "Abilities";

ImageManager::ImageManager() {
	try {
		FileReader file(IMAGE_FILE, FileReader::int_val);

		file.set_section(ABILITIES);
		auto num_abilities = file.get_num_lines(ABILITIES);
		_abilities.resize(num_abilities);

		for (auto it = file.s_begin(); it != file.s_end(); ++it) {
			for (int i = 0; i < num_abilities; ++i) {
				std::string image_path;
				if (file.read(&image_path, i)) {
					std::cout << "Loading... " << image_path << "\n";
					_abilities[i] = SOIL_load_OGL_texture(image_path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
				}
			}
		}

	}
	catch (std::exception& e) {
		std::cout << "dumb as hell\n";
	}
}

ImageManager::~ImageManager() {}

const std::vector<GLuint>& ImageManager::get_abilities() const {
	return _abilities;
}