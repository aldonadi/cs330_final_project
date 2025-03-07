#pragma once

#include "LiveTransformer.h"
#include "LiveTransformationUi.h"

#include <unordered_map>
#include <vector>

class LiveTransformationUi;

class LiveTransformers
{
private:
	std::vector<std::string> objectNames;
	std::unordered_map<std::string, LiveTransformer> objectTransformers;
	LiveTransformationUi ui;

	

public:


	LiveTransformers(GLFWwindow* window) 
		: ui(window, this),
	      objectTransformers(std::unordered_map<std::string, LiveTransformer>()) {};

	LiveTransformer* getObjectTransformer(const std::string objectName);

	void RegisterNewObject(
		const std::string objectName,      // unique string identifying this object
		float baseScaleX,    float baseScaleY,    float baseScaleZ,      // scale data
		float baseRotationX, float baseRotationY, float baseRotationZ,   // rotation data
		float basePositionX, float basePositionY, float basePositionZ,   // position data
		float baseColorR,    float baseColorG,    float baseColorB       // color data
	);

	LiveTransformationUi& getUi();

	std::vector<std::string> getObjectNames();

};