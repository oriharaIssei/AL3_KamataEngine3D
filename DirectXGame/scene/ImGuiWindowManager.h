#pragma once

#include <functional>

#include <string>

#include <vector>
#include <list>

class ImGuiWindow {
public:
	void Init(const std::string &name);
	void PushBackCommand(const std::function<void()> &command);
	void Execute();
private:
	std::vector < std::function<void()>> debugCommand_;
	std::string name_;
};

class ImGuiWindowManager {
public:
	void Update();
private:
	std::list<ImGuiWindow *> guiWindows;
public:
	void PushBackGuiWindow(ImGuiWindow *guiWindow) { guiWindows.push_back(guiWindow); };
};