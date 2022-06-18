#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>

/********************************************************************************************************************************************************/

class Program {
public:
	Program();
	Program(int key, std::string_view file_path);

	~Program();

	bool load(std::string_view file_path);

	void use() const;
	GLint location(std::string_view location) const;
private:
	bool load_shader(int type, const char* shader);
private:
	int				_key;
	std::string		_name;
	GLuint			_id;
};

/********************************************************************************************************************************************************/

#endif