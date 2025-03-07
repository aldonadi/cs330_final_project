///////////////////////////////////////////////////////////////////////////////
// viewmanager.h
// ============
// manage the viewing of 3D objects within the viewport
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "ViewManager.h"

#ifdef _DEBUG
#include "LiveTransformations/LiveTransformer.h"
#endif

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>    

// declaration of the global variables and defines
namespace
{
	// Variables for window width and height
	const int WINDOW_WIDTH = 1000;
	const int WINDOW_HEIGHT = 800;
	const char* g_ViewName = "view";
	const char* g_ProjectionName = "projection";

	// camera object used for viewing and interacting with
	// the 3D scene
	Camera* g_pCamera = nullptr;

	// these variables are used for mouse movement processing
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;

	// movement and mouse sensitivity sanity checks (experimentally derived)
	float MAX_MOUSE_SENSITIVITY = 0.4f;
	float MIN_MOUSE_SENSITIVITY = 0.002f;

	float MAX_MOVEMENT_SPEED = 10.0f;
	float MIN_MOVEMENT_SPEED = 0.5f;

	// time between current frame and last frame
	float gDeltaTime = 0.0f; 
	float gLastFrame = 0.0f;

	// the following variable is false when orthographic projection
	// is off and true when it is on
	bool bOrthographicProjection = false;

#ifdef _DEBUG
	// used to stop the mouse from changing the camera when the UI is displayed
	bool disableMouseCameraMovement = false;

	// used to prevent flickering the UI shown/hidden if the user holds down the SPACE key
	bool justToggledTheUi = false;
#endif // _DEBUG
}

/***********************************************************
 *  ViewManager()
 *
 *  The constructor for the class
 ***********************************************************/
ViewManager::ViewManager(
	ShaderManager *pShaderManager)
{
	// initialize the member variables
	m_pShaderManager = pShaderManager;
	m_pWindow = NULL;
	g_pCamera = new Camera();
	// default camera view parameters
	g_pCamera->Position = glm::vec3(0.0f, 5.0f, 12.0f);
	g_pCamera->Front = glm::vec3(0.0f, -0.5f, -2.0f);
	g_pCamera->Up = glm::vec3(0.0f, 1.0f, 0.0f);
	g_pCamera->Zoom = 80;
}

/***********************************************************
 *  ~ViewManager()
 *
 *  The destructor for the class
 ***********************************************************/
ViewManager::~ViewManager()
{
	// free up allocated memory
	m_pShaderManager = NULL;
	m_pWindow = NULL;
	if (NULL != g_pCamera)
	{
		delete g_pCamera;
		g_pCamera = NULL;
	}
}

/***********************************************************
 *  CreateDisplayWindow()
 *
 *  This method is used to create the main display window.
 ***********************************************************/
GLFWwindow* ViewManager::CreateDisplayWindow(const char* windowTitle)
{
	GLFWwindow* window = nullptr;

	// try to create the displayed OpenGL window
	window = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		windowTitle,
		NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	// tell GLFW to capture all mouse events
	enableMouseInput(window);

	// enable blending for supporting tranparent rendering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_pWindow = window;

	return(window);
}

void ViewManager::enableMouseInput(GLFWwindow* window)
{
	// this callback is used to receive mouse moving events
	glfwSetCursorPosCallback(window, &ViewManager::Mouse_Position_Callback);
	glfwSetScrollCallback(window, &ViewManager::Mouse_Scrollwheel_Callback);
}

void ViewManager::disableMouseInput(GLFWwindow* window)
{	
	// this callback is used to receive mouse moving events
	glfwSetCursorPosCallback(window, nullptr);
	glfwSetScrollCallback(window, nullptr);
}


/***********************************************************
 *  Mouse_Position_Callback()
 *
 *  This method is automatically called from GLFW whenever
 *  the mouse is moved within the active GLFW display window.
 ***********************************************************/
void ViewManager::Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos)
{
#ifdef _DEBUG
	// if in the UI, then don't process changes to the camera
	if (disableMouseCameraMovement) {
		return;
	}
#endif // _DEBUG
	
	// when the first mouse move event is received, this needs to be recorded so that
	// all subsequent mouse moves can correctly calculate the X position offset and Y
	// position offset for proper operation
	if (gFirstMouse)
	{
		gLastX = xMousePos;
		gLastY = yMousePos;
		gFirstMouse = false;
	}

	// calculate the X offset and Y offset values for moving the 3D camera accordingly
	float xOffset = xMousePos - gLastX;
	float yOffset = gLastY - yMousePos; // reversed since y-coordinates go from bottom to top

	// set the current positions into the last position variables
	gLastX = xMousePos;
	gLastY = yMousePos;

	// move the 3D camera according to the calculated offsets
	g_pCamera->ProcessMouseMovement(xOffset, yOffset);
}

/***********************************************************
 *  Mouse_Scrollwheel_Callback()
 *
 *  This method is automatically called from GLFW whenever
 *  the mouse scrollwheel is scrolled. Amount of scroll is
 *  given in the `yOffset` argument. `xOffset` is usually unused.
 ***********************************************************/
void ViewManager::Mouse_Scrollwheel_Callback(GLFWwindow* window, double xOffset, double yOffset)
{
#ifdef _DEBUG
	// if in the UI, then don't process changes to the camera
	if (disableMouseCameraMovement) {
		return;
	}
#endif // _DEBUG

	// adjust mouse sensitivity
	g_pCamera->MouseSensitivity += (g_pCamera->MouseSensitivity * (0.02 * yOffset));

	// adjust movement speed
	g_pCamera->MovementSpeed += (g_pCamera->MovementSpeed * (0.04 * yOffset));

	// sanity checks: mouse sensitivity
	if (g_pCamera->MouseSensitivity > MAX_MOUSE_SENSITIVITY) {
		g_pCamera->MouseSensitivity = MAX_MOUSE_SENSITIVITY;
	}
	else if (g_pCamera->MouseSensitivity < MIN_MOUSE_SENSITIVITY) {
		g_pCamera->MouseSensitivity = MIN_MOUSE_SENSITIVITY;
	}

	// sanity checks: movement speed
	if (g_pCamera->MovementSpeed > MAX_MOVEMENT_SPEED) {
		g_pCamera->MovementSpeed = MAX_MOVEMENT_SPEED;
	}
	else if (g_pCamera->MovementSpeed < MIN_MOVEMENT_SPEED) {
		g_pCamera->MovementSpeed = MIN_MOVEMENT_SPEED;
	}
}

/***********************************************************
 *  ProcessKeyboardEvents()
 *
 *  This method is called to process any keyboard events
 *  that may be waiting in the event queue.
 ***********************************************************/
void ViewManager::ProcessKeyboardEvents()
{
	// close the window if the escape key has been pressed
	if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_pWindow, true);
	}

#ifdef _DEBUG
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!justToggledTheUi) {
			// toggle the "show UI" flag
			this->showTransformerUi = !(this->showTransformerUi);

			// set the "don't flicker the UI when user holds down the toggle UI key" flag
			justToggledTheUi = true;
		}
	}
	else  // no longer pressing SPACE; clear the flag
	{
		justToggledTheUi = false;
	}

	// toggle whether mouse should affect camers
	// TODO: clean up this mess
	disableMouseCameraMovement = this->showTransformerUi;

	// disable keyboard movement: return before processing the keyboard nav events
	if (this->showTransformerUi)
		return;
#endif

	ProcessSceneNavigationKeyboardEvents();
}

void ViewManager::ProcessSceneNavigationKeyboardEvents() {
	// if the camera object is null, then exit this method
	if (NULL == g_pCamera)
	{
		return;
	}

	// process camera zooming in and out
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(FORWARD, gDeltaTime);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(BACKWARD, gDeltaTime);
	}

	// process camera panning left and right
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(LEFT, gDeltaTime);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(RIGHT, gDeltaTime);
	}

	// process camera panning up and down
	if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(UP, gDeltaTime);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(DOWN, gDeltaTime);
	}

	// process camera projection type (perspective or orthographic)
	if (glfwGetKey(m_pWindow, GLFW_KEY_O) == GLFW_PRESS)
	{
		bOrthographicProjection = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_P) == GLFW_PRESS)
	{
		bOrthographicProjection = false;
	}
}

/***********************************************************
 *  PrepareSceneView()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void ViewManager::PrepareSceneView()
{
	glm::mat4 view;
	glm::mat4 projection;

	// per-frame timing
	float currentFrame = glfwGetTime();
	gDeltaTime = currentFrame - gLastFrame;
	gLastFrame = currentFrame;

	// process any keyboard events that may be waiting in the 
	// event queue
	ProcessKeyboardEvents();

	// get the current view matrix from the camera
	view = g_pCamera->GetViewMatrix();

	// calculate the aspect ratio
	const float aspectRatio = (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT;

	// define the current projection matrix
	if (!bOrthographicProjection) {    // perspective projection
		projection = glm::perspective(glm::radians(g_pCamera->Zoom), aspectRatio, 0.1f, 100.0f);
	}
	else    // orthographic projection
	{
		// TODO: determine this programmatically or somehow that isn't a magic number
		float viewDistance = aspectRatio * 4.0f;

		// Thanks to https://stackoverflow.com/a/38266620 for helping me understand
        // matching up the relative sizes on screen between perspective and ortho
		// so that the book's visual width doesn't change drastically when switching
		// between perspective and orthographic
		projection = 
			glm::ortho(
				-aspectRatio * viewDistance,
				 aspectRatio * viewDistance,
				-viewDistance,
				 viewDistance,
				 0.1f, 100.0f);
	}

	// if the shader manager object is valid
	if (NULL != m_pShaderManager)
	{
		// set the view matrix into the shader for proper rendering
		m_pShaderManager->setMat4Value(g_ViewName, view);
		// set the view matrix into the shader for proper rendering
		m_pShaderManager->setMat4Value(g_ProjectionName, projection);
		// set the view position of the camera into the shader for proper rendering
		m_pShaderManager->setVec3Value("viewPosition", g_pCamera->Position);
	}
}