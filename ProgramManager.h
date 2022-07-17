#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#include <map>
#include <string_view>

class Program;
class Camera;

class ProgramManager {
public:
	ProgramManager();

	~ProgramManager();

	const Program* get(int id) const;

	void attach_camera(Camera* camera) const;
private:
	void load(int id, std::string_view path);

	std::map<int, Program*> _programs;
};

#endif