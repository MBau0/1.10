#include "Grid.h"

#include "Program.h"

#include "Entity.h"

#include <fstream>

static const char HEIGHT_MAP_FILE[] = "Data/height_map.data";

Grid::Grid(int width, int length) :
	_vao					( 0 ),
	_vertex_buffer			( 0 ),
	_height_buffer			( 0 ),
	_size					( glm::vec2(7.8125, 7.8125) ),
	_highlighted_tile		( { -1, -1 }),
	_width					( width ),
	_length					( length )
{
	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glCreateBuffers(1, &_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glNamedBufferStorage(_vertex_buffer, sizeof(_tile_mesh), _tile_mesh, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TileVertex), reinterpret_cast<void*>(offsetof(TileVertex, position)));

	_height_map.resize(_width * _length);

	glCreateBuffers(1, &_height_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _height_buffer);
	glNamedBufferStorage(_height_buffer, sizeof(GLfloat) * _height_map.size(), &_height_map[0], GL_DYNAMIC_STORAGE_BIT);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)0);
	glVertexAttribDivisor(1, 1);
}

Grid::~Grid() {
	glDeleteBuffers(1, &_vertex_buffer);
	glDeleteVertexArrays(1, &_vao);
}

void Grid::draw(glm::vec2 num_tiles, Program* program) {
	glBindVertexArray(_vao);
	program->use();

	glUniform2fv(program->location("size"), 1, &_size[0]);
	glUniform2fv(program->location("num_tiles"), 1, &num_tiles[0]);
	glUniform2fv(program->location("highlight"), 1, &_highlighted_tile[0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glLineWidth(2);

	glDrawArraysInstanced(GL_QUADS, 0, sizeof(_tile_mesh) / sizeof(TileVertex), int(num_tiles.x * num_tiles.y));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1);
	glDisable(GL_CULL_FACE);
}

glm::vec2 Grid::get_highlighted_tile() {
	return _highlighted_tile;
}

void Grid::highlight_tile(glm::vec3 mouse_position_world) {
	_highlighted_tile = { int(mouse_position_world.x / _size.x) , int(mouse_position_world.z / _size.y) };
}

Tile Grid::get_tile(GLfloat x, GLfloat z) {
	if (x < 0 || z < 0 ||
		x >= _width * _size.x ||
		z >= _length * _size.y) {
		return Tile();
	}

	x = ceil(x / _size.x);
	z = ceil(z / _size.y);

	Tile tile;
	tile.x = (int)(x - 1.0f);
	tile.z = (int)(z - 1.0f);
	tile.y = _height_map[tile.z * _width + tile.x];

	return tile;
}

#include <iostream>
void Grid::set_tile(Tile tile) {
	if (tile.x < 0 || tile.z < 0 ||
		tile.x >= _width ||
		tile.z >= _length) {
		return;
	}

	int index = tile.z * _width + tile.x;
	_height_map[index] = tile.y;

	glBindBuffer(GL_ARRAY_BUFFER, _height_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(GLfloat), sizeof(GLfloat), &tile.y);
}

void Grid::load_height_map() {
	std::ifstream file(HEIGHT_MAP_FILE, std::ios::binary);
	int bytes = 0;

	file.seekg(0, std::ios::end);
	bytes = file.tellg();
	file.seekg(0, std::ios::beg);
	if (bytes <= _height_map.size()) {
		file.read(reinterpret_cast<char*>(&_height_map[0]), bytes);
		update_height_buffer();
	}
}

void Grid::save_height_map() {
	std::ofstream file(HEIGHT_MAP_FILE, std::ios::binary | std::ios::trunc);
	
	file.write(reinterpret_cast<char*>(&_height_map[0]), _height_map.size() / sizeof(GLfloat));
}

/* can delete this laster just commit this incase i want to look back at it if changing height vertices*/
/*
Tile Grid::get_tile(GLfloat x, GLfloat z) {
	if (x < 0 || z < 0 || x >= (_width + 1) || z >= (_length + 1)) {
		return Tile();
	}

	x = ceil(x / _size.x);
	z = ceil(z / _size.y);

	Tile tile;
	tile.x = x;
	tile.z = z;

	z = z * (_width + 1);
	tile.y.h4 = z + x;
	tile.y.h3 = z + x - 1;
	tile.y.h2 = z + x - _width - 1;
	tile.y.h1 = z + x - _width - 2;

	tile.y.h4 = _height_map[(int)tile.y.h4];
	tile.y.h3 = _height_map[(int)tile.y.h3];
	tile.y.h2 = _height_map[(int)tile.y.h2];
	tile.y.h1 = _height_map[(int)tile.y.h1];

	return tile;
}
*/

void Grid::update_height_buffer() {
	glBindBuffer(GL_ARRAY_BUFFER, _height_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, NULL, sizeof(GLfloat) * _height_map.size(), &_height_map[0]);
}

void Grid::place_building(std::shared_ptr<Entity> building) {

}