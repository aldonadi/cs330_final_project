#include "LiveTransformer.h"

#include <vector>
#include <string>
#include <map>
#include "LiveTransformers.h"
#include <stdexcept>
#include <sstream>

LiveTransformer* LiveTransformers::getObjectTransformer(const std::string objectName)
{
	
}

void LiveTransformers::RegisterNewObject(
	const std::string objectName,      // unique string identifying this object
	float baseScaleX,    float baseScaleY,    float baseScaleZ,      // scale data
	float baseRotationX, float baseRotationY, float baseRotationZ,   // rotation data
	float basePositionX, float basePositionY, float basePositionZ,   // position data
	float baseColorR,    float baseColorG,    float baseColorB)      // color data
{
	if (this->objectTransformers.find(objectName) != this->objectTransformers.end()) {
	    std::ostringstream oss;
		oss << "An object with name '" << objectName << "' has already been registered.";
		const std::string errMsg = oss.str();

		throw std::runtime_error(errMsg);
	}

	// insert a new Transformer object into the list, saving all current scale/rot/pos/color data	
	objectTransformers.at(objectName) = 
		LiveTransformer(
			objectName,
			baseScaleX,    baseScaleY,    baseScaleZ,      // scale data
			baseRotationX, baseRotationY, baseRotationZ,   // rotation data
			basePositionX, basePositionY, basePositionZ,   // position data
			baseColorR,    baseColorG,    baseColorB
		)

}
