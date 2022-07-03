#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Program;

struct CameraSettings {
	float _field_of_view = 90.0f;
	float _aspect = 1.0f;
	float _z_near = .1f;
	float _z_far = 1000.0f;
	float _h_angle = 3.14f;
	float _v_angle = 0.0f;
};

class Camera {
public:
	enum Direction{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	enum Mode {
		FREE,
		LOCKED,
		TOGGLE
	};
public:
	Camera(GLFWwindow* window, const CameraSettings& settings);

	void update();

	void move(int direction, float time, float speed = DEFAULT_SPEED);
	void rotate(float x, float y);

	void set_mode(int mode);

	void attach_program(Program* program);

	glm::mat4 get_view() const;
	glm::mat4 get_projection() const;
	glm::vec3 get_position() const;

	glm::vec3 mouse_position_world() const;

	int get_mode() const;
private:
	void move_free(int direction, float time, float speed = DEFAULT_SPEED);
	void move_locked(int direction, float time, float speed = DEFAULT_SPEED);
private:
	int			   _mode;
	CameraSettings _settings;

	glm::vec3 _position;
	glm::vec3 _direction;
	glm::vec3 _right;
	glm::vec3 _up;

	glm::mat4 _projection;
	glm::mat4 _ortho;
	glm::mat4 _view;

	GLFWwindow* _window;

	std::vector<Program*> _programs;

	static constexpr float DEFAULT_SPEED = .11f;
	static constexpr float LOCKED_VERTICAL_ANGLE = -1.2f;
};

#endif