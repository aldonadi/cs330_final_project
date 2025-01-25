#include "LiveTransformationUi.h"
#include "LiveTransformer.h"
#include "LiveTransformers.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <vector>

LiveTransformationUi::LiveTransformationUi(GLFWwindow* window, LiveTransformers* xfrms)
    : window(window), xfrms(xfrms)
{
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
    const char* initiallySelected = _strdup(this->getSelectedObject().c_str());

    // get the list of registered objects
    std::vector<std::string> objects = this->xfrms->getObjectNames();

    // ensure an object is selected
    if (this->selectedObjectName == "")
        this->selectedObjectName = objects.at(0);

    if (ImGui::BeginCombo("##selected_object", initiallySelected)) {
        for (const auto& object : objects) {
            if (ImGui::Selectable(object.c_str(), initiallySelected == object.c_str())) {
                this->selectObject(object);
            }
        }
        ImGui::EndCombo();
    }

    LiveTransformer* selectedXfmr = this->xfrms->getObjectTransformer(this->getSelectedObject());

    // Button: "Reset Adjustments"
    if (ImGui::Button("Reset Adjustments")) {
        selectedXfmr->reset();
    }

    // Label: "Adjust Scale" (Bold)
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Scale");

    // Labels and sliders for scale adjustments
    // Note: The "PushID" and "PopID" wrappers are because ImGui can't abide controls with duplicate label names ("X:", etc)
    ImGui::PushID("scaleX"); ImGui::SliderFloat("X:", &selectedXfmr->XscaleAdjusted, -8.0f, 8.0f); ImGui::PopID();
    ImGui::PushID("scaleY"); ImGui::SliderFloat("Y:", &selectedXfmr->YscaleAdjusted, -8.0f, 8.0f); ImGui::PopID();
    ImGui::PushID("scaleZ"); ImGui::SliderFloat("Z:", &selectedXfmr->ZscaleAdjusted, -8.0f, 8.0f); ImGui::PopID();

    // Label: "Adjust Rotation" (Bold)                                                          
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Rotation");

    // Labels and sliders for rotation adjustments
    ImGui::PushID("rotationX"); ImGui::SliderFloat("X:", &selectedXfmr->XrotationAdjusted, -360.0f, 360.0f); ImGui::PopID();
    ImGui::PushID("rotationY"); ImGui::SliderFloat("Y:", &selectedXfmr->YrotationAdjusted, -360.0f, 360.0f); ImGui::PopID();
    ImGui::PushID("rotationZ"); ImGui::SliderFloat("Z:", &selectedXfmr->ZrotationAdjusted, -360.0f, 360.0f); ImGui::PopID();

    // Label: "Adjust Position" (Bold)
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Position");

    // Labels and sliders for position adjustments
    ImGui::PushID("positionX"); ImGui::SliderFloat("X:", &selectedXfmr->XpositionAdjusted, -30.0f, 30.0f); ImGui::PopID();
    ImGui::PushID("positionY"); ImGui::SliderFloat("Y:", &selectedXfmr->YpositionAdjusted, -30.0f, 30.0f); ImGui::PopID();
    ImGui::PushID("positionZ"); ImGui::SliderFloat("Z:", &selectedXfmr->ZpositionAdjusted, -30.0f, 30.0f); ImGui::PopID();

    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Color");

    

    ImGui::End();
}