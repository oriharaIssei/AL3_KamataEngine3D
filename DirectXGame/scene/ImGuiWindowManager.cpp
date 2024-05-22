#include "ImGuiWindowManager.h"

#ifdef _DEBUG
#include <ImGui.h>
#endif // DEBUG

void ImGuiWindow::Init(const std::string &name) {
	name_ = name;
	debugCommand_.push_back([this]() {ImGui::Begin(name_.c_str()); });
}

void ImGuiWindow::PushBackCommand(const std::function<void()> &command) {
	debugCommand_.push_back([command]() {command(); });
}

void ImGuiWindow::Execute() {
	for(auto &command : debugCommand_) {
		command();
	}
}

void ImGuiWindowManager::Update() {
	for(auto &guiWindow : guiWindows) {
		guiWindow->Execute();
	}
}