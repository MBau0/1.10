#ifndef GRID_H

#include <GL/gl3w.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Program;

struct TileVertex {
	GLfloat position[2];
};

struct Tile {
	int x = 0, z = 0;
	GLfloat y = 0.0f;
};

class Grid {
public:

	Grid(int width, int length);

	// should destroy buffers
	~Grid();

	void draw(glm::vec2 num_tiles, Program* program);

	void highlight_tile(glm::vec3 mouse_position_world);

	Tile get_tile(GLfloat x, GLfloat z);

	void set_tile(Tile tile);

	glm::vec2 get_highlighted_tile();

	void load_height_map();

	void save_height_map();

	//void place_building(std::shared_ptr<Entity> building);
private:
	// init vertex buffer when constructed
	void create_buffers();

	void update_height_buffer();

	static constexpr TileVertex _tile_mesh[4] = 
		{ {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
		  {1.0f, 1.0f}//, {1.0f, 1.0f}, {1.0f, 0.0f} 
	};

	GLuint _vao;
	GLuint _vertex_buffer;
	GLuint _height_buffer;

	glm::vec2 _size;

	glm::vec2 _highlighted_tile;

	std::vector<GLfloat> _height_map;

	int _width;
	int _length;
};

#endif