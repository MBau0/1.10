#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <vector>

#include <GL/gl3w.h>

// loads all GLuint textures into memory
// assigns id's to images for getting
// reads from images file that assings ids to file paths
class ImageManager {
public:
	ImageManager();

	~ImageManager();

	ImageManager(const ImageManager&&) = delete;

	ImageManager& operator=(const ImageManager&) = delete;

	const std::vector<GLuint>& get_abilities() const;

private:
	std::vector<GLuint> _abilities;
};

#endif