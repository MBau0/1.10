#include "UI.h"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

static const char GLSL_VERSION_STR[] = "#version 450 core";

UI::UI(GLFWwindow* window, const std::vector<GLuint>& abilities) :
	_window			( window ),
	_unit_panel		( abilities )
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION_STR);
}

UI::~UI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UI::new_frame() const {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UI::update() {
	_unit_panel.update();
}

void UI::draw() const {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UnitPanel& UI::get_unit_panel() {
	return _unit_panel;
}