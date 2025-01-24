#include "LiveTransformationUi.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

LiveTransformationUi::LiveTransformationUi(GLFWwindow* window, LiveTransformationManager* ltm)
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

LiveTransformationUi::~LiveTransformationUi()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void LiveTransformationUi::ShowUi()
{
	// don't draw anything if the main window is minimized ("iconified")
	if (glfwGetWindowAttrib(this->window, GLFW_ICONIFIED) != 0)
	{
		ImGui_ImplGlfw_Sleep(10);
		return;
	}

	// create the UI's frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (this->show_demo_window) {
		ImGui::ShowDemoWindow(&(this->show_demo_window));
	}

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
