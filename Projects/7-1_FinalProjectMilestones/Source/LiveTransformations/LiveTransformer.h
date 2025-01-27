#pragma once

#include <string>

class LiveTransformer
{
public:
	std::string objectName = "";

	// huge constructor
	LiveTransformer(
		const std::string objectName,      // unique string identifying this object
		float baseScaleX, float baseScaleY, float baseScaleZ,      // scale data
		float baseRotationX, float baseRotationY, float baseRotationZ,   // rotation data
		float basePositionX, float basePositionY, float basePositionZ,   // position data
		float baseColorR, float baseColorG, float baseColorB)      // color data
		: objectName(objectName),                                                       // initializer list
		XscaleSaved(baseScaleX), YscaleSaved(baseScaleY), ZscaleSaved(baseScaleZ),
		XrotationSaved(baseRotationX), YrotationSaved(baseRotationY), ZrotationSaved(baseRotationZ),
		XpositionSaved(basePositionX), YpositionSaved(basePositionY), ZpositionSaved(basePositionZ),
		RcolorSaved(baseColorR), GcolorSaved(baseColorG), BcolorSaved(baseColorB),

		XscaleAdjusted(baseScaleX), YscaleAdjusted(baseScaleY), ZscaleAdjusted(baseScaleZ),
		XrotationAdjusted(baseRotationX), YrotationAdjusted(baseRotationY), ZrotationAdjusted(baseRotationZ),
		XpositionAdjusted(basePositionX), YpositionAdjusted(basePositionY), ZpositionAdjusted(basePositionZ),
		RcolorAdjusted(baseColorR), GcolorAdjusted(baseColorG), BcolorAdjusted(baseColorB)
	{
	}

	// generate a block of code with the transformation data
	std::string getTransformAndRenderCodeString();

	// todo: find a better way to get the effective vertex data printout to the UI
	std::string effectiveTransformationDataString = "";

	// SAVED VALUES, for resetting back to orig
	float XscaleSaved;
	float YscaleSaved;
	float ZscaleSaved;

	float XrotationSaved;
	float YrotationSaved;
	float ZrotationSaved;
						
	float XpositionSaved;
	float YpositionSaved;
	float ZpositionSaved;
						
	float RcolorSaved;
	float GcolorSaved;
	float BcolorSaved;

	// ADJUSTMENTS
	float XscaleAdjusted;
	float YscaleAdjusted;
	float ZscaleAdjusted;
		  
	float XrotationAdjusted;
	float YrotationAdjusted;
	float ZrotationAdjusted;
		  
	float XpositionAdjusted;
	float YpositionAdjusted;
	float ZpositionAdjusted;

	float RcolorAdjusted;
	float GcolorAdjusted;
	float BcolorAdjusted;

	float scaleAdjIncrement = 0.003f;
	float rotationAdjIncrement = 0.04f;
	float positionAdjIncrement = 0.005f;

	float scaleCoarseAdjMultiplier = 10.0f;
	float rotCoarseAdjMultiplier = 10.0f;
	float posCoarseAdjMultiplier = 5.0f;

	void reset();

	// incrementing
	void incScaleAdjX(float increment);
	void incScaleAdjY(float increment);
	void incScaleAdjZ(float increment);

	void incRotationX(float increment);
	void incRotationY(float increment);
	void incRotationZ(float increment);

	void incPositionX(float increment);
	void incPositionY(float increment);
	void incPositionZ(float increment);

	// decrementing
	void decScaleAdjX(float increment);
	void decScaleAdjY(float increment);
	void decScaleAdjZ(float increment);
		 
	void decRotationX(float increment);
	void decRotationY(float increment);
	void decRotationZ(float increment);
		 
	void decPositionX(float increment);
	void decPositionY(float increment);
	void decPositionZ(float increment);
};