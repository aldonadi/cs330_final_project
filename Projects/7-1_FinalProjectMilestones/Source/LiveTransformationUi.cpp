#include "LiveTransformationUi.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <vector>

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
		ShowTransformationUiControls();
	}

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LiveTransformationUi::ShowTransformationUiControls() {

    ImGui::Begin("Live Transformations", nullptr, ImGuiWindowFlags_NoResize);

    // Label: "Live Transformations" (Bold, Underlined)
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Manage live transformations of objects");
    }
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Live Transformations");

    // Label: "Selected Object:"; Dropdown: (empty so far)
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Select an object to apply transformations");
    }
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Selected Object:");
    const char* initiallySelected = _strdup(this->ltm->getSelectedObject().c_str());

    std::vector<std::string> objects = { 
        "open-book-left-cover", 
        "open-book-right-cover", 
        "open-book-left-page",
        "open-book-right-page"
    };

    if (ImGui::BeginCombo("##selected_object", initiallySelected)) {
        for (const auto& object : objects) {
            if (ImGui::Selectable(object.c_str(), initiallySelected == object.c_str())) {
                this->ltm->setSelectedObject(object);
            }
        }
        ImGui::EndCombo();
    }

    // Button: "Reset Adjustments"
    if (ImGui::Button("Reset Adjustments")) {
        this->ltm->reset();
    }

    // Label: "Adjust Scale" (Bold)
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Scale");

    // Labels and sliders for scale adjustments
    // Note: The "PushID" and "PopID" wrappers are because ImGui can't abide controls with duplicate label names ("X:", etc)
    ImGui::PushID("scaleX"); ImGui::SliderFloat("X:", &ltm->XscaleAdj, -8.0f, 8.0f); ImGui::PopID();
    ImGui::PushID("scaleY"); ImGui::SliderFloat("Y:", &ltm->YscaleAdj, -8.0f, 8.0f); ImGui::PopID();
    ImGui::PushID("scaleZ"); ImGui::SliderFloat("Z:", &ltm->ZscaleAdj, -8.0f, 8.0f); ImGui::PopID();

    // Label: "Adjust Rotation" (Bold)                                                          
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Rotation");

    // Labels and sliders for rotation adjustments
    ImGui::PushID("rotationX"); ImGui::SliderFloat("X:",  & ltm->XrotationAdj, -360.0f, 360.0f); ImGui::PopID();
    ImGui::PushID("rotationY"); ImGui::SliderFloat("Y:",  & ltm->YrotationAdj, -360.0f, 360.0f); ImGui::PopID();
    ImGui::PushID("rotationZ"); ImGui::SliderFloat("Z:",  & ltm->ZrotationAdj, -360.0f, 360.0f); ImGui::PopID();

    // Label: "Adjust Position" (Bold)
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Position");

    // Labels and sliders for position adjustments
    ImGui::PushID("positionX"); ImGui::SliderFloat("X:", &ltm->XpositionAdj, -30.0f, 30.0f); ImGui::PopID();
    ImGui::PushID("positionY"); ImGui::SliderFloat("Y:", &ltm->YpositionAdj, -30.0f, 30.0f); ImGui::PopID();
    ImGui::PushID("positionZ"); ImGui::SliderFloat("Z:", &ltm->ZpositionAdj, -30.0f, 30.0f); ImGui::PopID();

    ImGui::End();
}