#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include <string_view>
#include <map>

class Action;

class ActionManager {
public:
	ActionManager();

	~ActionManager();

	Action* get(int id) const;
private:
	void load(int id, std::string_view path);

	std::map<int, Action*> _actions;
};

#endif