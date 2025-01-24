#pragma once

#include "LiveTransformer.h"

#include <vector>
#include <string>
#include <map>

class LiveTransformers {
private:
	std::map<std::string, LiveTransformer> objectTransformers;

public:
	LiveTransformer* getObjectTransformer(const std::string objectName);

	void RegisterNewObjectTransformer(
		const std::string objectName,      // unique string identifying this object
		float baseScaleX,    float baseScaleY,    float baseScaleZ,      // scale data
		float baseRotationX, float baseRotationY, float baseRotationZ,   // rotation data
		float basePositionX, float basePositionY, float basePositionZ,   // position data
		float baseColorR,    float baseColorG,    float baseColorB       // color data
	);
};