#include "ActionManager.h"

#include "Action.h"
#include "BuildAction.h"
#include "EmptyAction.h"

ActionManager::ActionManager() {
	Action* empty = new EmptyAction(nullptr, 0, "Empty", 0);
	Action* build = new BuildAction(nullptr, 0, "Build", 1);
	_actions.insert(std::pair<int, Action*>(0, empty));
	_actions.insert(std::pair<int, Action*>(1, build));
}

ActionManager::~ActionManager() {
	for (auto it : _actions) {
		delete it.second;
	}
}

Action* ActionManager::get(int id) const {
	const auto it = _actions.find(id);
	return (it != _actions.end()) ? it->second : nullptr;
}