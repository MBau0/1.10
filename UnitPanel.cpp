#include "UnitPanel.h"

#include <imgui/imgui.h>

#include "ImageManager.h"
#include "Action.h"

static const ImVec2 ABILITY_ICON_SIZE = ImVec2(48, 48);
static const ImVec2 BACKGROUND_SIZE = ImVec2(800, 200);
static const ImVec2 BACKGROUND_POS = ImVec2(400, 700);

UnitPanel::UnitPanel(ImageManager* image_manager) :
	_actions		( nullptr ),
	_image_manager	( image_manager )
{}

void UnitPanel::update() {
	ImGui::SetNextWindowPos(BACKGROUND_POS);
	ImGui::SetNextWindowSize(BACKGROUND_SIZE);
	ImGui::Begin("UnitPanel", (bool*)0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	update_action_frame();

	ImGui::End();
}

void UnitPanel::update_action_frame() {
	//ImGui::SetNextWindowPos({ 100, 0 });
	ImGui::SetNextWindowPos({ BACKGROUND_POS.x + 540, BACKGROUND_POS.y + 10 });
	ImGui::BeginChild("Ability Frame", { 250, 180 }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	if (_actions) {
		int index = 0;
		while (index < _actions->size()) {
			if (_actions->at(index) != nullptr) {
				if (index != 0 && index % 4 != 0) {
					ImGui::SameLine(0.0f, 0.0f);
				}
				GLuint id = _actions->at(index)->get_image();
				ImGui::ImageButton((void*)(intptr_t)_image_manager->get_actions().at(id), ABILITY_ICON_SIZE);
			}
			++index;
		}
	}

	ImGui::EndChild();
}

void UnitPanel::set_actions(std::vector<Action*>* actions) {
	_actions = actions;
}