#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <memory>

#include "LiveTransformer.h"

class LiveTransformationUi
{
private:
	GLFWwindow* window;
	LiveTransformer* ltm;

	ImGuiIO io;

	bool show_demo_window = true;

public:
	LiveTransformationUi(GLFWwindow* window, LiveTransformer* ltm);

	~LiveTransformationUi();

	void ShowUi();

	void ShowTransformationUiControls();


};