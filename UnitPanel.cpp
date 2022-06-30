#include "UnitPanel.h"

#include <imgui/imgui.h>

static const ImVec2 ABILITY_ICON_SIZE = ImVec2(48, 48);
static const ImVec2 BACKGROUND_SIZE = ImVec2(800, 200);
static const ImVec2 BACKGROUND_POS = ImVec2(400, 700);

UnitPanel::UnitPanel(const std::vector<GLuint>& abilities) :
	_abilities		( abilities ) 
{}

void UnitPanel::update() {
	ImGui::SetNextWindowPos(BACKGROUND_POS);
	ImGui::SetNextWindowSize(BACKGROUND_SIZE);
	ImGui::Begin("UnitPanel", (bool*)0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	update_ability_frame();

	ImGui::End();
}

void UnitPanel::update_ability_frame() {
	//ImGui::SetNextWindowPos({ 100, 0 });
	ImGui::SetNextWindowPos({ BACKGROUND_POS.x + 540, BACKGROUND_POS.y + 10 });
	ImGui::BeginChild("Ability Frame", { 250, 180 }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	for (int row = 0; row < 3; ++row) {
		for (int i = 0; i < 4; ++i) {
			ImGui::SameLine(0.0f, 0.0f);
			ImGui::ImageButton((void*)(intptr_t)_abilities[0], ABILITY_ICON_SIZE);
		}
		ImGui::NewLine();
	}	


	ImGui::EndChild();
}