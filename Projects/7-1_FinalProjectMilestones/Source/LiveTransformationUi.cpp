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
    const char* initiallySelected = this->ltm->getSelectedObject().c_str();

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
    ImGui::SliderFloat("sX:", &ltm->XscaleAdj, -15.0f, 15.0f);
    ImGui::SliderFloat("sY:", &ltm->YscaleAdj,  -15.0f, 15.0f);
    ImGui::SliderFloat("sZ:", &ltm->ZscaleAdj , -15.0f, 15.0f);

    // Label: "Adjust Rotation" (Bold)
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Rotation");

    // Labels and sliders for rotation adjustments
    ImGui::SliderFloat("rX:",  & ltm->XrotationAdj, -360.0f, 360.0f);
    ImGui::SliderFloat("rY:",  & ltm->YrotationAdj, -360.0f, 360.0f);
    ImGui::SliderFloat("rZ:",  & ltm->ZrotationAdj, -360.0f, 360.0f);

    // Label: "Adjust Position" (Bold)
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Position");

    // Labels and sliders for position adjustments
    ImGui::SliderFloat("pX:", &ltm->XpositionAdj, -100.0f, 100.0f);
    ImGui::SliderFloat("pY:", &ltm->YpositionAdj, -100.0f, 100.0f);
    ImGui::SliderFloat("pZ:", &ltm->ZpositionAdj, -100.0f, 100.0f);

    ImGui::End();
}