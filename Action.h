#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <GL/gl3w.h>

class Entity;

class Action {
public:
	Action(Entity* entity, int id, std::string name, GLuint image);

	virtual void execute() = 0;

	GLuint get_image() const;

	void set_entity(Entity* entity);
private:
	Entity* _entity;
	int _id;
	std::string _name;
	GLuint _image;
};

#endif