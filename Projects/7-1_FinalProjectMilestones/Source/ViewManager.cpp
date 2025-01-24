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
#include "LiveTransformer.h"
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

	// time between current frame and last frame
	float gDeltaTime = 0.0f; 
	float gLastFrame = 0.0f;

	// the following variable is false when orthographic projection
	// is off and true when it is on
	bool bOrthographicProjection = false;
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// this callback is used to receive mouse moving events
	glfwSetCursorPosCallback(window, &ViewManager::Mouse_Position_Callback);

	// enable blending for supporting tranparent rendering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_pWindow = window;

	return(window);
}

/***********************************************************
 *  Mouse_Position_Callback()
 *
 *  This method is automatically called from GLFW whenever
 *  the mouse is moved within the active GLFW display window.
 ***********************************************************/
void ViewManager::Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos)
{
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
	DbgProcessTransformationKeyboardEvents();
#endif
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

	// define the current projection matrix
	projection = glm::perspective(glm::radians(g_pCamera->Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

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

#ifdef _DEBUG
void ViewManager::DbgProcessTransformationKeyboardEvents() {
	bool shiftHeld = (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);

	bool coarseAdj = shiftHeld;

	// reset
	if (glfwGetKey(m_pWindow, GLFW_KEY_P) == GLFW_PRESS) { ltm->reset(); }

	// adjust scale
	if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS) { ltm->decScaleAdjX(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS) { ltm->incScaleAdjX(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS) { ltm->decScaleAdjY(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS) { ltm->incScaleAdjY(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_Z) == GLFW_PRESS) { ltm->decScaleAdjZ(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_X) == GLFW_PRESS) { ltm->incScaleAdjZ(coarseAdj); }

	// adjust rotation
	if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS) { ltm->decRotationX(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_R) == GLFW_PRESS) { ltm->incRotationX(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS) { ltm->decRotationY(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_F) == GLFW_PRESS) { ltm->incRotationY(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_C) == GLFW_PRESS) { ltm->decRotationZ(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_V) == GLFW_PRESS) { ltm->incRotationZ(coarseAdj); }

	// adjust position
	if (glfwGetKey(m_pWindow, GLFW_KEY_T) == GLFW_PRESS) { ltm->decPositionX(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_Y) == GLFW_PRESS) { ltm->incPositionX(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_G) == GLFW_PRESS) { ltm->decPositionY(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_H) == GLFW_PRESS) { ltm->incPositionY(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_B) == GLFW_PRESS) { ltm->decPositionZ(coarseAdj); }
	if (glfwGetKey(m_pWindow, GLFW_KEY_N) == GLFW_PRESS) { ltm->incPositionZ(coarseAdj); }
}
#endif