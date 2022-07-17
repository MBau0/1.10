#ifndef UNIT_PANEL_H
#define UNIT_PANEL_H

#include <GL/gl3w.h>

#include <memory>
#include <vector>

class Action;
class ImageManager;

class UnitPanel {
public:
	UnitPanel(ImageManager* image_manager);

	void update();

	void update_action_frame();

	void set_actions(std::vector<Action*>* actions);
private:
	ImageManager* _image_manager;

	std::vector<Action*>* _actions;
};

#endif