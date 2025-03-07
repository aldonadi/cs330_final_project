///////////////////////////////////////////////////////////////////////////////
// shadermanager.h
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderManager.h"
#include "ShapeMeshes.h"

#include <string>
#include <vector>
#include <functional>

#ifdef _DEBUG
#include "LiveTransformations/LiveTransformers.h"
#include "LiveTransformations/LiveTransformer.h"
#endif

/***********************************************************
 *  SceneManager
 *
 *  This class contains the code for preparing and rendering
 *  3D scenes, including the shader settings.
 ***********************************************************/
class SceneManager
{
public:
	// constructor
	SceneManager(ShaderManager *pShaderManager);
	// destructor
	~SceneManager();

	struct TEXTURE_INFO
	{
		std::string tag;
		uint32_t ID;
	};

	struct OBJECT_MATERIAL
	{
		float ambientStrength;
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shininess;
		std::string tag;
	};


private:
	// pointer to shader manager object
	ShaderManager* m_pShaderManager;
	// pointer to basic shapes object
	ShapeMeshes* m_basicMeshes;
	// total number of loaded textures
	int m_loadedTextures;
	// loaded textures info
	TEXTURE_INFO m_textureIDs[16];
	// defined object materials
	std::vector<OBJECT_MATERIAL> m_objectMaterials;

	// load texture images and convert to OpenGL texture data
	bool CreateGLTexture(const char* filename, std::string tagT);
	// bind loaded OpenGL textures to slots in memory
	void BindGLTextures();
	// free the loaded OpenGL textures
	void DestroyGLTextures();
	// find a loaded texture by tag
	int FindTextureID(std::string tag);
	int FindTextureSlot(std::string tag);
	// find a defined material by tag
	bool FindMaterial(std::string tag, OBJECT_MATERIAL& material);

	// set the transformation values 
	// into the transform buffer
	void SetTransformations(
		glm::vec3 scaleXYZ,
		float XrotationDegrees,
		float YrotationDegrees,
		float ZrotationDegrees,
		glm::vec3 positionXYZ);

	// set the color values into the shader
	void SetShaderColor(
		float redColorValue,
		float greenColorValue,
		float blueColorValue,
		float alphaValue);

	// set the texture data into the shader
	void SetShaderTexture(
		std::string textureTag);

	void SetShaderTextureOverlay(std::string textureTag);

	// set the UV scale for the texture mapping
	void SetTextureUVScale(
		float u, float v);

	// set the object material into the shader
	void SetShaderMaterial(
		std::string materialTag);

public:

	// The following methods are for the students to 
	// customize for their own 3D scene
	void PrepareScene();
	void RenderScene();

	void RenderBackdrop();
	void RenderOpenBook();
	void RenderClosedBook();
	void RenderTambourine();
	void RenderUpturnedBowl();
	void RenderAquarium();

	// draw the specified shape with given scale, rotation, position, and color or texture
	void TransformAndRender(
		std::string objName,
		std::function<void()> ShapeDrawFunc,
		glm::vec3 scale,
		glm::vec3 rot,
		glm::vec3 pos,
		glm::vec4 color,
		const std::string textureName,
		const std::string overlayTextureName,
		const std::string materialName);

	// commonly-used overload version
	void TransformAndRender(
		std::string objName,
		std::function<void()> ShapeDrawFunc,
		float scaleX, float scaleY, float scaleZ,
		float rotX,   float rotY,   float rotZ,
		float posX,   float posY,   float posZ,
		float colorR, float colorG, float colorB,
		const std::string textureName = "", 
		const std::string overlayTextureName = "",
		const std::string materialName = ""
	);

	// configures the different materials for 3D objects
	void DefineObjectMaterials();

	// configures light sources for the scene
	void SetupSceneLights();

	// loads textures from image files
	void LoadSceneTextures();

	

#ifdef _DEBUG
	/*** Allows adjusting the scale, rotation, and position ***/
	/*** of a selected object while app is running          ***/
	LiveTransformers* xfmrs = nullptr;
#endif // DEBUG

};