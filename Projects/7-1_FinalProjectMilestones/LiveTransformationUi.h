#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <memory>

class LiveTranslationUi
{
private:
	GLFWwindow* window;
	LiveTranslationUi* ltm;

	ImGuiIO io;

public:
	LiveTranslationUi(GLFWwindow* window, LiveTranslationUi* ltm);

	~LiveTranslationUi();

	void ShowUi();


};