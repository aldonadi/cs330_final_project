///////////////////////////////////////////////////////////////////////////////
// shadermanager.cpp
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifdef _DEBUG
#include "LiveTransformations/LiveTransformer.h"
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>

#include <functional>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_TextureOverlayValueName = "objectTextureOverlay";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseTextureOverlayName = "bUseTextureOverlay";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();

	// initialize the texture collection
	for (int i = 0; i < 16; i++)
	{
		m_textureIDs[i].tag = "/0";
		m_textureIDs[i].ID = -1;
	}
	m_loadedTextures = 0;
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;

	// destroy the created OpenGL textures
	DestroyGLTextures();
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.ambientColor = m_objectMaterials[index].ambientColor;
			material.ambientStrength = m_objectMaterials[index].ambientStrength;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}




/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationX * rotationY * rotationZ * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 *  This also resets (toggles off) using an overlay texture
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setIntValue(g_UseTextureOverlayName, false);

		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 *  This also resets (toggles off) using an overlay texture
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureOverlayName, false);

		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetShaderTextureOverlay()
 *
 *  This method is used for setting a second texture overlay
 *  associated with the passed in ID into the shader.
 *  This texture should have transparency if the "underneath"
 *  texture should be visible.
 ***********************************************************/
void SceneManager::SetShaderTextureOverlay(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureOverlayName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureOverlayValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.ambientColor", material.ambientColor);
			m_pShaderManager->setFloatValue("material.ambientStrength", material.ambientStrength);
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/

 /***********************************************************
  *  DefineObjectMaterials()
  *
  *  This method is used for configuring the various material
  *  settings for all of the objects within the 3D scene.
  ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	/**
	 ** Material Properties
	 **
	 ** **ambientColor** is a vec3 color value that can be used to set the ambient emission color.
	 ** **ambientStrength** is a float value that defines how much ambient light is emited.
	 ** **diffuseColor** is a vec3 color value that defines the diffuse light emission color.
	 ** **specularColor** is a vec3 color value that defines the specular light reflection color.
	 ** **shininess** is a float value that tells how shiny the material is and how much specular light will be
	 **   reflected off the surface.
	 ** **tag** is a special string that is used to identify the material in the collection.
	 **/

	// default material is an adaptation of "white rubber" from
	//  http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html

	OBJECT_MATERIAL defaultMaterial;
	defaultMaterial.ambientColor = glm::vec3(0.5f, 0.5f, 0.5f);
	defaultMaterial.ambientStrength = 0.1f;
	defaultMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	defaultMaterial.specularColor = glm::vec3(0.7f, 0.7f, 0.7f);
	defaultMaterial.shininess = 10.0;
	defaultMaterial.tag = "default";

	m_objectMaterials.push_back(defaultMaterial);
	
	OBJECT_MATERIAL goldMaterial;
	goldMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.1f);
	goldMaterial.ambientStrength = 0.1f;
	goldMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	goldMaterial.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	goldMaterial.shininess = 22.0;
	goldMaterial.tag = "metal";

	m_objectMaterials.push_back(goldMaterial);

	OBJECT_MATERIAL cementMaterial;
	cementMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	cementMaterial.ambientStrength = 0.1f;
	cementMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	cementMaterial.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	cementMaterial.shininess = 0.5;
	cementMaterial.tag = "cement";

	m_objectMaterials.push_back(cementMaterial);

	OBJECT_MATERIAL woodMaterial;
	woodMaterial.ambientColor = glm::vec3(0.4f, 0.3f, 0.1f);
	woodMaterial.ambientStrength = 0.1;
	woodMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	woodMaterial.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	woodMaterial.shininess = 0.3;
	woodMaterial.tag = "wood";

	m_objectMaterials.push_back(woodMaterial);

	OBJECT_MATERIAL tileMaterial;
	tileMaterial.ambientColor = glm::vec3(0.2f, 0.3f, 0.4f);
	tileMaterial.ambientStrength = 0.1f;
	tileMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	tileMaterial.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	tileMaterial.shininess = 25.0;
	tileMaterial.tag = "tile";

	m_objectMaterials.push_back(tileMaterial);

	OBJECT_MATERIAL glassMaterial;
	glassMaterial.ambientColor = glm::vec3(0.4f, 0.4f, 0.4f);
	glassMaterial.ambientStrength = 0.1f;
	glassMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glassMaterial.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glassMaterial.shininess = 85.0;
	glassMaterial.tag = "glass";

	m_objectMaterials.push_back(glassMaterial);

	OBJECT_MATERIAL clayMaterial;
	clayMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.3f);
	clayMaterial.ambientStrength = 0.1f;
	clayMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	clayMaterial.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	clayMaterial.shininess = 0.5;
	clayMaterial.tag = "clay";

	m_objectMaterials.push_back(clayMaterial);

	OBJECT_MATERIAL tutorialMaterial;
	tutorialMaterial.ambientColor = glm::vec3(0.74f, 0.38f, 0.45f);
	tutorialMaterial.ambientStrength = 0.5f;
	tutorialMaterial.diffuseColor = glm::vec3(0.74f, 0.38f, 0.45f);
	tutorialMaterial.specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
	tutorialMaterial.shininess = 32.0f;
	tutorialMaterial.tag = "light";

	m_objectMaterials.push_back(tutorialMaterial);

}

/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene.  There are up to 4 light sources.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	// this line of code is NEEDED for telling the shaders to render 
	// the 3D scene with custom lighting, if no light sources have
	// been added then the display window will be black - to use the 
	// default OpenGL lighting then comment out the following line
	m_pShaderManager->setBoolValue(g_UseLightingName, true);

	/**
	 ** Light Properties
	 **
	 ** **position** is a vec3 value that sets the position of the light source in 3D space.
	 ** **ambientColor** is a vec3 value that defines the color of the ambient light being emited.
	 ** **diffuseColor** is a vec3 value that defines the color of the diffuse light being emited.
	 ** **specularColor** is a vec3 value that defines the color of the specular light being emited.
	 ** **focalStrength** is a float value that defines the focus of the light beam being emited.
	 ** **specularIntensity** is a float value that defines the intensity of the emited specular lighting.
	 **/

	m_pShaderManager->setVec3Value("lightSources[0].position", 3.0f, 14.0f, 3.0f);
	m_pShaderManager->setVec3Value("lightSources[0].ambientColor", 0.3f, 0.3f, 0.3f);
	m_pShaderManager->setVec3Value("lightSources[0].diffuseColor", 1.0f, 1.0f, 1.0f);
	m_pShaderManager->setVec3Value("lightSources[0].specularColor", 1.0f, 1.0f, 1.0f);
	m_pShaderManager->setFloatValue("lightSources[0].focalStrength", 32.0f);
	m_pShaderManager->setFloatValue("lightSources[0].specularIntensity", 0.05f);

	/*
	m_pShaderManager->setVec3Value("lightSources[1].position", -5.0f, 3.0f, 5.0f);
	m_pShaderManager->setVec3Value("lightSources[1].ambientColor", 0.1f, 0.1f, 0.1f);
	m_pShaderManager->setVec3Value("lightSources[1].diffuseColor", 0.3f, 0.3f, 0.3f);
	m_pShaderManager->setVec3Value("lightSources[1].specularColor", 1.0f, 1.0f, 1.0f);
	m_pShaderManager->setFloatValue("lightSources[1].focalStrength", 32.0f);
	m_pShaderManager->setFloatValue("lightSources[1].specularIntensity", 0.05f);
	*/

}

void SceneManager::LoadSceneTextures()
{
	/*** STUDENTS - add the code BELOW for loading the textures that ***/
	/*** will be used for mapping to objects in the 3D scene. Up to  ***/
	/*** 16 textures can be loaded per scene. Refer to the code in   ***/
	/*** the OpenGL Sample for help.                                 ***/

	bool bReturn = false;
	bReturn = CreateGLTexture(
		"../../Utilities/textures/from-my-ai/book-cover-red-leather.png",
		"open-book-cover");
	assert(bReturn);

	bReturn = CreateGLTexture(
		"../../Utilities/textures/from-my-ai/open-book-left-page.png",
		"open-book-left-page");
	assert(bReturn);

	bReturn = CreateGLTexture(
		"../../Utilities/textures/from-my-ai/open-book-right-page.png",
		"open-book-right-page");
	assert(bReturn);

	bReturn = CreateGLTexture(
		"../../Utilities/textures/from-my-ai/open-book-page-crinkle-effect.png",
		"open-book-page-crinkle-effect");
	assert(bReturn);

	bReturn = CreateGLTexture(
		"../../Utilities/textures/from-my-ai/wood_paneling.png",
		"wood-paneling");
	assert(bReturn);

	bReturn = CreateGLTexture(
		"../../Utilities/textures/from-my-camera/hay-bales.png",
		"hay-bales");
	assert(bReturn);

	bReturn = CreateGLTexture(
		"../../Utilities/textures/from-my-ai/book-cover-image-1.png",
		"book-cover-image-1");
	assert(bReturn);

	bReturn = CreateGLTexture(
		"../../Utilities/textures/from-my-ai/book-cover-image-2.png",
		"book-cover-image-2");
	assert(bReturn);

	// after the texture image data is loaded into memory, the
	// loaded textures need to be bound to texture slots - there
	// are a total of 16 available slots for scene textures
	BindGLTextures();
}

/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{ 
	// load the textures for the 3D scene
	LoadSceneTextures();

	// load the materials for scene objects
	DefineObjectMaterials();

	// load the light sources for the scene
	SetupSceneLights();

	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene

	m_basicMeshes->LoadTilingPlaneMesh(10.0, 5.0);    // floor
	m_basicMeshes->LoadPlaneMesh();     // pages
	m_basicMeshes->LoadBoxMesh();       // books
	m_basicMeshes->LoadCylinderMesh();  // tambourine and rattles
	m_basicMeshes->LoadTaperedCylinderMesh(); // upturned bowl

}

/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene()
{
	RenderBackdrop();
	RenderOpenBook();
	RenderClosedBook();
	RenderTambourine();
	RenderUpturnedBowl();
	RenderAquarium();
}

void SceneManager::RenderBackdrop()
{	
	/****** The Floor *******/
	TransformAndRender(
		"floor",
		std::bind(&ShapeMeshes::DrawTilingPlaneMesh, m_basicMeshes),
		//  x           y           z
		20.0f,         1.0f,      10.0f,       // scale
		0.65f,         0.46f,      0.65f,      // rotation
		0.0f,          0.0f,       0.0f,       // position
		//  r           g           b
		0.68,          0.41,       0.17,       // color   ("wood-floor brown")
		"wood-paneling",                       // texture
		"",                                    // texture overlay
		"wood"                                 // material
	);
}

void SceneManager::RenderOpenBook()
{
	    TransformAndRender(
        "open-book-cover-left",
        std::bind(&ShapeMeshes::DrawBoxMesh, m_basicMeshes),
        //  x           y           z
           2.15f,      0.20f,      2.70f,      // scale
          90.50f,      0.75f,    -43.00f,      // rotation
          -0.50f,      1.36f,      6.00f,      // position
        //  r           g           b
           0.82f,      0.17f,      0.07f,       // color
		  "open-book-cover",                     // texture
		  "",                                   // texture overlay
		  "glass"                               // material
    );

    TransformAndRender(
        "open-book-cover-right",
        std::bind(&ShapeMeshes::DrawBoxMesh, m_basicMeshes),
        //  x           y           z
           2.00f,      0.20f,      2.70f,      // scale
          90.00f,      0.00f,   -141.00f,      // rotation
           0.95f,      1.36f,      5.96f,      // position
        //  r           g           b
           0.82f,      0.17f,      0.07f,       // color
		"open-book-cover",                     // texture
		"",                                   // texture overlay
		"glass"                               // material
    );


	// TODO: add texture to pages
    TransformAndRender(
        "open-book-page-left",
        std::bind(&ShapeMeshes::DrawPlaneMesh, m_basicMeshes),
        //  x           y           z
           0.91f,      1.34f,      1.22f,      // scale
          90.50f,      0.75f,    -43.00f,      // rotation
          -0.50f,      1.36f,      6.14f,      // position
        //  r           g           b
           1.00f,      1.00f,      1.00f,      // color
		  "open-book-left-page",
		  "open-book-page-crinkle-effect"
    );


    TransformAndRender(
        "open-book-page-right",
        std::bind(&ShapeMeshes::DrawPlaneMesh, m_basicMeshes),
        //  x           y           z
           0.91f,      1.34f,      1.22f,      // scale
          90.00f,      0.00f,   -141.00f,      // rotation
           0.85f,      1.36f,      6.01f,      // position
        //  r           g           b
           1.00f,      1.00f,      1.00f,      // color
		  "open-book-right-page",
		  "open-book-page-crinkle-effect"
    );


	TransformAndRender(
		"open-book-front-cover",
		std::bind(&ShapeMeshes::DrawPlaneMesh, m_basicMeshes),
		//  x           y           z
           0.91f,      1.34f,      1.22f,      // scale
          89.80f,      0.00f,    137.00f,      // rotation
          -0.543f,     1.36f,      5.901f,     // position
        //  r           g           b
           1.00f,      1.00f,      1.00f,      // color
		"book-cover-image-2",                  // texture
		"",                                    // texture overlay
		"glass"                                // material
	);
}

void SceneManager::RenderClosedBook()
{
	TransformAndRender(
		"closed-book",
		std::bind(&ShapeMeshes::DrawBoxMesh, m_basicMeshes),
		//  x           y           z
		   2.00f,      0.20f,      2.60f,      // scale
		   0.00f,     90.00f,      0.00f,      // rotation
		   0.43f,      2.77f,      6.09f,      // position
		//  r           g           b
		0.82f,         0.17f,      0.07f,      // color
		"open-book-cover",                     // texture   
		"",                                    // texture overlay
		"glass"                                // material
	);

		TransformAndRender(
		"closed-book-illustration",
		std::bind(&ShapeMeshes::DrawPlaneMesh, m_basicMeshes),
        //  x           y           z
           0.97f,      0.00f,      1.26f,      // scale
           0.00f,     90.00f,      0.00f,      // rotation
           0.43f,      2.875f,     6.09f,      // position
		//  r           g           b
		0.82f,         0.17f,      0.07f,      // color
		"book-cover-image-1",                  // texture   
		"",                                    // texture overlay
		"cement"                                // material
	);
}

void SceneManager::RenderTambourine()
{
	TransformAndRender(
		"tambourine",
		std::bind(&ShapeMeshes::DrawCylinderMesh, m_basicMeshes, true, true, true),  // drawTop, drawBottom, drawSides
		//  x           y           z
		0.65f,         0.46f,      0.65f,      // scale
		0.00f,        90.00f,      0.00f,      // rotation
		1.03f,         2.87f,      6.42f,      // position
		//  r           g           b
		0.73f,         0.59f,      0.44f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"wood"                                 // material
	);

	TransformAndRender(
		"tambourine-rattle-1",
		std::bind(&ShapeMeshes::DrawCylinderMesh, m_basicMeshes, true, true, true),  // drawTop, drawBottom, drawSides
		//  x           y           z
		0.17f,         0.04f,      0.17f,      // scale
		0.00f,        90.00f,      0.00f,      // rotation
		0.60f,         3.05f,      6.91f,      // position
		//  r           g           b
		0.38f,         0.38f,      0.38f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"metal"                                // material
	);

	TransformAndRender(
		"tambourine-rattle-2",
		std::bind(&ShapeMeshes::DrawCylinderMesh, m_basicMeshes, true, true, true),  // drawTop, drawBottom, drawSides
//  x           y           z
           0.17f,      0.04f,      0.17f,      // scale
           0.00f,     90.00f,      0.00f,      // rotation
           1.31f,      3.05f,      7.00f,      // position
        //  r           g           b
           0.38f,      0.38f,      0.38f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"metal"                                // material
	);

	TransformAndRender(
		"tambourine-rattle-3",
		std::bind(&ShapeMeshes::DrawCylinderMesh, m_basicMeshes, true, true, true),  // drawTop, drawBottom, drawSides
        //  x           y           z
           0.17f,      0.04f,      0.17f,      // scale
           0.00f,     90.00f,      0.00f,      // rotation
           1.69f,      3.05f,      6.42f,      // position
        //  r           g           b
           0.38f,      0.38f,      0.38f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"metal"                                // material
	);

	TransformAndRender(
		"tambourine-rattle-4",
		std::bind(&ShapeMeshes::DrawCylinderMesh, m_basicMeshes, true, true, true),  // drawTop, drawBottom, drawSides
        //  x           y           z
           0.17f,      0.04f,      0.17f,      // scale
           0.00f,     90.00f,      0.00f,      // rotation
           1.36f,      3.05f,      5.82f,      // position
        //  r           g           b
           0.38f,      0.38f,      0.38f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"metal"                                // material
	);

	TransformAndRender(
		"tambourine-rattle-5",
		std::bind(&ShapeMeshes::DrawCylinderMesh, m_basicMeshes, true, true, true),  // drawTop, drawBottom, drawSides
        //  x           y           z
           0.17f,      0.04f,      0.17f,      // scale
           0.00f,     90.00f,      0.00f,      // rotation
           0.71f,      3.05f,      5.82f,      // position
        //  r           g           b
           0.38f,      0.38f,      0.38f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"metal"                                // material
	);

	TransformAndRender(
		"tambourine-rattle-6",
		std::bind(&ShapeMeshes::DrawCylinderMesh, m_basicMeshes, true, true, true),  // drawTop, drawBottom, drawSides
        //  x           y           z
           0.17f,      0.04f,      0.17f,      // scale
           0.00f,     90.00f,      0.00f,      // rotation
           0.38f,      3.05f,      6.37f,      // position
        //  r           g           b
           0.38f,      0.38f,      0.38f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"metal"                                // material
	);
}

void SceneManager::RenderUpturnedBowl()
{
		TransformAndRender(
		"bowl",
		std::bind(&ShapeMeshes::DrawTaperedCylinderMesh, m_basicMeshes, true, true, true),  // drawTop, drawBottom, drawSides
        //  x           y           z
           0.50f,      0.46f,      0.50f,      // scale
           0.00f,     90.00f,      0.00f,      // rotation
          -0.22f,      2.87f,      5.66f,      // position
        //  r           g           b
           0.28f,      0.28f,      0.40f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"clay"                                 // material
	);

		
	TransformAndRender(
		"hay",
		std::bind(&ShapeMeshes::DrawBoxMesh, m_basicMeshes),  // drawTop, drawBottom, drawSides
        //  x           y           z
           0.25f,      0.03f,      0.25f,      // scale
           0.00f,     78.261f,     0.00f,      // rotation
          -0.111f,     3.342f,     5.606f,     // position
        //  r           g           b
           0.28f,      0.28f,      0.40f,      // color
		"hay-bales",                           // texture   
		"",                                    // texture overlay
		"clay"                                 // material
	);
}

void SceneManager::RenderAquarium() {
	TransformAndRender(
		"aquarium-bottom",
		std::bind(&ShapeMeshes::DrawBoxMesh, m_basicMeshes),
        //  x           y           z
           0.40f,      0.29f,      0.40f,      // scale
           0.00f,     90.00f,      0.00f,      // rotation
           1.03f,      3.01f,      5.28f,      // position
        //  r           g           b
           0.24f,      0.56f,      0.71f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"glass"                                // material
	);
	
	// uses the new overload of TransformAndRender that allows color alpha
	TransformAndRender(
		"aquarium-middle",
		std::bind(&ShapeMeshes::DrawBoxMesh, m_basicMeshes),
               //  x           y           z
        glm::vec3(0.40f,      0.90f,      0.40f),      // scale
        glm::vec3(0.00f,     90.00f,      0.00f),      // rotation
        glm::vec3(1.03f,      3.58f,      5.28f),      // position
               //  r           g            b            a
        glm::vec4(0.24f,      0.56f,      0.71f,        0.5f),    // color with alpha
		"",                                    // texture   
		"",                                    // texture overlay
		"glass"                                // material
	);
	
	TransformAndRender(
		"aquarium-top",
		std::bind(&ShapeMeshes::DrawBoxMesh, m_basicMeshes),
        //  x           y           z
           0.40f,      0.29f,      0.40f,      // scale
           0.00f,     90.00f,      0.00f,      // rotation
           1.03f,      4.175f,     5.28f,      // position
        //  r           g           b
           0.24f,      0.56f,      0.71f,      // color
		"",                                    // texture   
		"",                                    // texture overlay
		"glass"                                // material
	);
}

/***********************************************************
 *  TransformAndRender()
 *
 *  This method is used for rendering a single Shape with given
 *  scale, rotation, translation, and color or texture data.
 *  If textureName is not empty, the texture is selected by name from
 *  the textures loaded in SceneManager::LoadSceneTextures.
 *  If textureName is empty, then the color data is used instead.
 *  It takes the draw function that should be called to draw the object.
 *  If the draw func is a member method, it must be properly bound by std::bind.
 ***********************************************************/
void SceneManager::TransformAndRender(
	std::string objName,
	std::function<void()> ShapeDrawFunc,
	glm::vec3 scale,
	glm::vec3 rot,
	glm::vec3 pos,
	glm::vec4 color,
	const std::string textureName,
	const std::string overlayTextureName,
	const std::string materialName)
{

#ifdef _DEBUG
	// register this object with the LiveTransformer
	// if object has already been registered, this does nothing
	this->xfmrs->RegisterNewObject(
		 objName,
		 scale.x,  scale.y,  scale.z,
		 rot.x,    rot.y,    rot.z,
		 pos.x,    pos.y,    pos.z,
		 color.x,  color.y,  color.z);

	LiveTransformer* objXfmr = this->xfmrs->getObjectTransformer(objName);

	// TODO: handle this better
	if (objXfmr == nullptr) {
		std::cerr << "Failed to get object transformer for '" << objName << "'" << std::endl;
		assert(false);
	}

	scale.x = objXfmr->XscaleAdjusted;
	scale.y = objXfmr->YscaleAdjusted;
	scale.z = objXfmr->ZscaleAdjusted;

	rot.x = objXfmr->XrotationAdjusted;
	rot.y = objXfmr->YrotationAdjusted;
	rot.z = objXfmr->ZrotationAdjusted;

	pos.x = objXfmr->XpositionAdjusted;
	pos.y = objXfmr->YpositionAdjusted;
	pos.z = objXfmr->ZpositionAdjusted;

	color.x = objXfmr->RcolorAdjusted;
	color.y = objXfmr->GcolorAdjusted;
	color.z = objXfmr->BcolorAdjusted;

#endif

	/******************************************************************/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/

	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(scale.x, scale.y, scale.z);

	// set the XYZ rotation for the mesh
	XrotationDegrees = rot.x;
	YrotationDegrees = rot.y;
	ZrotationDegrees = rot.z;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(pos.x, pos.y, pos.z);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// texture is assumed if textureName is not blank
	bool useTexture = (textureName != "");

	if (useTexture) {
		// set the texture name into the shader
		SetShaderTexture(textureName);
	}
	else
	{
		// set the color values into the shader
		SetShaderColor(color.x, color.y, color.z, color.w);
	}

	// set the overlay texture, if specified
	bool useOverlayTexture = (overlayTextureName != "");

	if (useOverlayTexture) {
		// set the overlay texture name into the shader
		SetShaderTextureOverlay(overlayTextureName);
	}

	// set the material if specified, otherwise set the default material
	if (materialName != "") {
		SetShaderMaterial(materialName);
	}
	else
	{
		SetShaderMaterial("default");
	}

	// draw the mesh with transformation values
	ShapeDrawFunc();
}

// Overload that is commonly used for non-alpha colored objects
void SceneManager::TransformAndRender(
	std::string objName,
	std::function<void()> ShapeDrawFunc,
	float scaleX, float scaleY, float scaleZ,
	float rotX,   float rotY,   float rotZ,
	float posX,   float posY,   float posZ,
	float colorR, float colorG, float colorB,
	const std::string textureName,
	const std::string overlayTextureName,
	const std::string materialName)
{
	TransformAndRender(
		objName,
		ShapeDrawFunc,
		glm::vec3(scaleX,  scaleY,  scaleZ),
		glm::vec3(rotX,    rotY,    rotZ),
		glm::vec3(posX,    posY,    posZ),
		glm::vec4(colorR,  colorG,  colorB,  1.0f),      // this overload assumes no transparency
		textureName,
		overlayTextureName,
		materialName
	);
}