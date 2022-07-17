#ifndef SCENE_H
#define SCENE_H

#include <GL/gl3w.h>

#include "Mesh.h"
#include "Transform.h"
#include "Program.h"

#include <vector>
#include <memory>
#include <string_view>

struct aiNode;
struct aiScene;

class Scene {
public:
	Scene();

	void draw(int mode = GL_TRIANGLES, Transform transform = Transform());

	Scene* new_child();
	void add_child(std::unique_ptr<Scene> child);

	void attach_program(const Program* program);

	bool load_assimp(std::string_view directory, std::string_view file);

	const Mesh* get_mesh() const; // only works if there is only one mesh. used for map
private:
	void attach_program(const Program* program, Scene* child) const;
	void construct_scene_from_assimp(const aiScene* ai_scene, aiNode* node, Scene* scene, std::string_view directory, std::string depth) const;

	Scene* _parent;
	std::vector<std::unique_ptr<Scene>> _children;

	std::vector<Mesh> _meshes;
	Transform _transform;
	const Program* _program;
};

#endif