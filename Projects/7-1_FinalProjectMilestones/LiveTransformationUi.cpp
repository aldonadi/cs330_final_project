#include "LiveTransformationUi.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

LiveTranslationUi::LiveTranslationUi(GLFWwindow* window, LiveTranslationUi* ltm)
{
	this->window = window;
	this->ltm = ltm;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    this->io = ImGui::GetIO(); (void)io;
	this->io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	bool install_callbacks = true;
	ImGui_ImplGlfw_InitForOpenGL(this->window, install_callbacks);

	ImGui_ImplOpenGL3_Init("#version 330 core");
}
