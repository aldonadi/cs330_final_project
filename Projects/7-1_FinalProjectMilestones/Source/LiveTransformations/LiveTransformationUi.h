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

	// set this to True to show the UI next render
	bool shouldShowUi = false;
public:

	LiveTransformationUi(GLFWwindow* window, LiveTransformers* xfrms);

	~LiveTransformationUi();

	// allow the UI to be shown (and capture keybd/mouse)
	void enableUI();

	// do not allow the UI to be shown (and prevent capturing keyb/mouse)
	void disableUI();

	// Returns true if enabled, otherwise false
	bool isUiEnabled();


	void ShowUi();

	void ShowTransformationUiControls();

	void selectObject(std::string objectName) { selectedObjectName = objectName; }
	const std::string getSelectedObject() { return selectedObjectName; }

};