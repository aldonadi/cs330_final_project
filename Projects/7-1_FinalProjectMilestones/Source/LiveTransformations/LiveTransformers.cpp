#include "LiveTransformer.h"

#include <vector>
#include <string>
#include <map>
#include "LiveTransformers.h"
#include <stdexcept>
#include <sstream>

/*
LiveTransformers::LiveTransformers(GLFWwindow* window) {
	this->ui = LiveTransformationUi(window, this);
	this->objectTransformers = std::unordered_map<std::string, LiveTransformer>();
}
*/

LiveTransformer* LiveTransformers::getObjectTransformer(const std::string objectName)
{
	return &objectTransformers.at(objectName);
}

void LiveTransformers::RegisterNewObject(
	const std::string objectName,      // unique string identifying this object
	float baseScaleX,    float baseScaleY,    float baseScaleZ,      // scale data
	float baseRotationX, float baseRotationY, float baseRotationZ,   // rotation data
	float basePositionX, float basePositionY, float basePositionZ,   // position data
	float baseColorR,    float baseColorG,    float baseColorB)      // color data
{
	if (this->objectTransformers.find(objectName) != this->objectTransformers.end()) {
	    // old approach: throw an error
		/*
		std::ostringstream oss;
		oss << "An object with name '" << objectName << "' has already been registered.";
		const std::string errMsg = oss.str();

		throw std::runtime_error(errMsg);
		*/
		
		// TODO: reevaluate: just silently ignore it?
		return;
	}

	// insert a new Transformer object into the list, saving all current scale/rot/pos/color data	
	// I am creating this std::pair nonsense instead of just doing
	// objectTransformers[objectName] = LiveTransformer(args) because 
	// unordered_map::operator[], when the key does not exist yet, first creates a 
	// LiveTransformer object using the default constructor. Unfortunately, LiveTransformer
	// has no default constructor.
	// TODO: evaluate allowing a default constructor for LiveTransformer
	std::pair<std::string, LiveTransformer> newObject(objectName, LiveTransformer(
		objectName,
		baseScaleX, baseScaleY, baseScaleZ,      // scale data
		baseRotationX, baseRotationY, baseRotationZ,   // rotation data
		basePositionX, basePositionY, basePositionZ,   // position data
		baseColorR, baseColorG, baseColorB
	));
	
	objectTransformers.insert(newObject);

	objectNames.push_back(objectName);
}

LiveTransformationUi& LiveTransformers::getUi()
{
	return this->ui;
}

std::vector<std::string> LiveTransformers::getObjectNames()
{
	return this->objectNames;
}
