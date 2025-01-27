#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <memory>
#include <string>

//#include "LiveTransformers.h"

class LiveTransformers;

class LiveTransformationUi
{
private:
	GLFWwindow* window;
	LiveTransformers* xfrms;

	std::string selectedObjectName;

	ImGuiIO io;

	bool show_demo_window = true;

public:
	LiveTransformationUi(GLFWwindow* window, LiveTransformers* xfrms);

	~LiveTransformationUi();

	void ShowUi();

	void ShowTransformationUiControls();

	void selectObject(std::string objectName) { selectedObjectName = objectName; }
	const std::string getSelectedObject() { return selectedObjectName; }

};