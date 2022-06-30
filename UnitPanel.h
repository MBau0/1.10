#ifndef UNIT_PANEL_H
#define UNIT_PANEL_H

#include <GL/gl3w.h>

#include <memory>
#include <vector>

class UnitPanel {
public:
	UnitPanel(const std::vector<GLuint>& abilities);

	void update();

	void update_ability_frame();

	void set_ability_image() { }
private:
	const std::vector<GLuint>& _abilities;
};

#endif