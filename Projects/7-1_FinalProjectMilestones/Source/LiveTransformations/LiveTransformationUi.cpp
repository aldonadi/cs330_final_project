#include "LiveTransformationUi.h"
#include "LiveTransformer.h"
#include "LiveTransformers.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

// for printing flags in binary form
#include <bitset>

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

    // start with UI hidden
    this->disableUi();
}

LiveTransformationUi::~LiveTransformationUi()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void LiveTransformationUi::enableUi()
{
 
    this->shouldShowUi = true;

    /*
    // clear the mouse/keyboard disable flags
    this->io.ConfigFlags &= ~ImGuiConfigFlags_NoKeyboard;
    this->io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    
    // print a message and binary representation of ConfigFlags
    std::bitset<sizeof(this->io.ConfigFlags) * 8> flagBits(this->io.ConfigFlags);
    std::cout << "enabled UI:  NoMouse=" << ((this->io.ConfigFlags) & ImGuiConfigFlags_NoMouse) << std::endl;
    */
}

void LiveTransformationUi::disableUi()
{
    this->shouldShowUi = false;
    /*
    // set the mouse/keyboard disable flags
    this->io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
    this->io.ConfigFlags |= ImGuiConfigFlags_NoMouse;

    // print a message and binary representation of ConfigFlags
    std::bitset<sizeof(this->io.ConfigFlags) * 8> flagBits(this->io.ConfigFlags);
    std::cout << "disabled UI: NoMouse=" << ((this->io.ConfigFlags) & ImGuiConfigFlags_NoMouse) << std::endl;
    */
}

bool LiveTransformationUi::isUiEnabled()
{
    return this->shouldShowUi;
}


void LiveTransformationUi::ShowUi()
{
    // don't draw anything if we shouldn't show the UI right now
    if (!this->shouldShowUi) {
        return;
    }

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

    // set up the UI controls
	ShowTransformationUiControls();

    // render the UI
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LiveTransformationUi::ShowTransformationUiControls() {

    ImGui::Begin("Live Transformations", nullptr);

    //std::cout << "w: " << ImGui::GetWindowWidth() << ", h: " << ImGui::GetWindowHeight() << std::endl;

    // Label: "Live Transformations" (Bold, Underlined)
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Manage live transformations of objects");
    }
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Live Transformations");

    // Label for drop-down
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Selected Object:");
    const char* initiallySelected = _strdup(this->getSelectedObject().c_str());

    // get the list of registered objects
    std::vector<std::string> objects = this->xfrms->getObjectNames();

    // ensure an object is selected
    if (this->selectedObjectName == "")
        this->selectedObjectName = objects.at(0);

    if (ImGui::BeginCombo("##selected_object", initiallySelected, ImGuiComboFlags_WidthFitPreview)) {
        for (const auto& object : objects) {
            if (ImGui::Selectable(object.c_str(), initiallySelected == object.c_str())) {
                this->selectObject(object);
            }
        }
        ImGui::EndCombo();
    }

    // dropdown tooltip
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Select which object to adjust");
    }

    LiveTransformer* selectedXfmr = this->xfrms->getObjectTransformer(this->getSelectedObject());

    // Button: "Reset Adjustments"
    if (ImGui::Button("Reset Adjustments")) {
        selectedXfmr->reset();
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Reset all transformations to the original, unadjusted values");
    }

    if (ImGui::Button("Copy Transformation Code Block")) {
        std::string copyString = selectedXfmr->getTransformAndRenderCodeString();

        ImGui::SetClipboardText(copyString.c_str());
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Copy the TransformAndRender code block with current transformations");
    }

    // Label: "Adjust Scale" (Bold)
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Scale");

    // make the sliders wider than default for finer-grained adjustments
    ImGui::PushItemWidth(200.0f);

    // Labels and sliders for scale adjustments
    // Note: The "PushID" and "PopID" wrappers are because ImGui can't abide controls with duplicate label names ("X:", etc)
    ImGui::PushID("scaleX"); ImGui::SliderFloat("X", &selectedXfmr->XscaleAdjusted, -8.0f, 8.0f); ImGui::PopID();
    ImGui::PushID("scaleY"); ImGui::SliderFloat("Y", &selectedXfmr->YscaleAdjusted, -8.0f, 8.0f); ImGui::PopID();
    ImGui::PushID("scaleZ"); ImGui::SliderFloat("Z", &selectedXfmr->ZscaleAdjusted, -8.0f, 8.0f); ImGui::PopID();

    // Label: "Adjust Rotation" (Bold)                                                          
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Rotation");

    // Labels and sliders for rotation adjustments
    ImGui::PushID("rotationX"); ImGui::SliderFloat("X", &selectedXfmr->XrotationAdjusted, -360.0f, 360.0f); ImGui::PopID();
    ImGui::PushID("rotationY"); ImGui::SliderFloat("Y", &selectedXfmr->YrotationAdjusted, -360.0f, 360.0f); ImGui::PopID();
    ImGui::PushID("rotationZ"); ImGui::SliderFloat("Z", &selectedXfmr->ZrotationAdjusted, -360.0f, 360.0f); ImGui::PopID();

    // Label: "Adjust Position" (Bold)
    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Position");

    // Labels and sliders for position adjustments
    ImGui::PushID("positionX"); ImGui::SliderFloat("X", &selectedXfmr->XpositionAdjusted, -10.0f, 10.0f); ImGui::PopID();
    ImGui::PushID("positionY"); ImGui::SliderFloat("Y", &selectedXfmr->YpositionAdjusted, -10.0f, 10.0f); ImGui::PopID();
    ImGui::PushID("positionZ"); ImGui::SliderFloat("Z", &selectedXfmr->ZpositionAdjusted, -10.0f, 10.0f); ImGui::PopID();

    // done with setting slider width
    ImGui::PopItemWidth();

    ImGui::TextColored(ImVec4(0.6f, 1.0f, 1.0f, 1.0f), "Adjust Color");

    // create a color vector from current object color
    ImVec4 color = ImVec4(
        selectedXfmr->RcolorAdjusted,
        selectedXfmr->GcolorAdjusted,
        selectedXfmr->BcolorAdjusted,
        1.0f);

    // add the color picker control which will modify the color vector
    ImGui::ColorPicker4("RGBA", (float*)&color);
    
    // apply any changes from the color picker back to the transformer object

    selectedXfmr->RcolorAdjusted = color.x;
    selectedXfmr->GcolorAdjusted = color.y;
    selectedXfmr->BcolorAdjusted = color.z;
    

    ImGui::End();
}