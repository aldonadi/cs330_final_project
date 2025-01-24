#pragma once

#include <string>

class LiveTransformer {
public:

	// huge constructor
	LiveTransformer(
		const std::string objectName,      // unique string identifying this object
		float baseScaleX, float baseScaleY, float baseScaleZ,      // scale data
		float baseRotationX, float baseRotationY, float baseRotationZ,   // rotation data
		float basePositionX, float basePositionY, float basePositionZ,   // position data
		float baseColorR, float baseColorG, float baseColorB)      // color data
	:   objectName(objectName),                                                       // initializer list
		XscaleSaved(baseScaleX), YscaleSaved(baseScaleY), ZscaleSaved(baseScaleZ),  
		XrotationSaved(baseRotationX), YrotationSaved(baseRotationY), ZrotationSaved(baseRotationZ),
		XpositionSaved(basePositionX), YpositionSaved(basePositionY), ZpositionSaved(basePositionZ),
		RcolorSaved(baseColorR), GcolorSaved(baseColorG), BcolorSaved(baseColorB)
	{}

	std::string objectName = "";

	// todo: find a better way to get the effective vertex data printout to the UI
	std::string effectiveTransformationDataString = "";

	// SAVED VALUES
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
	float XscaleAdjusted = 0.0f;
	float YscaleAdjusted = 0.0f;
	float ZscaleAdjusted = 0.0f;
		  
	float XrotationAdjusted = 0.0f;
	float YrotationAdjusted = 0.0f;
	float ZrotationAdjusted = 0.0f;
		  
	float XpositionAdjusted = 0.0f;
	float YpositionAdjusted = 0.0f;
	float ZpositionAdjusted = 0.0f;

	float scaleAdjIncrement = 0.003f;
	float rotationAdjIncrement = 0.04f;
	float positionAdjIncrement = 0.005f;

	float scaleCoarseAdjMultiplier = 10.0f;
	float rotCoarseAdjMultiplier = 10.0f;
	float posCoarseAdjMultiplier = 5.0f;

	void reset();

	// incrementing
	void incScaleAdjX(bool coarseAdj);
	void incScaleAdjY(bool coarseAdj);
	void incScaleAdjZ(bool coarseAdj);

	void incRotationX(bool coarseAdj);
	void incRotationY(bool coarseAdj);
	void incRotationZ(bool coarseAdj);

	void incPositionX(bool coarseAdj);
	void incPositionY(bool coarseAdj);
	void incPositionZ(bool coarseAdj);

	// decrementing
	void decScaleAdjX(bool coarseAdj);
	void decScaleAdjY(bool coarseAdj);
	void decScaleAdjZ(bool coarseAdj);
		 
	void decRotationX(bool coarseAdj);
	void decRotationY(bool coarseAdj);
	void decRotationZ(bool coarseAdj);
		 
	void decPositionX(bool coarseAdj);
	void decPositionY(bool coarseAdj);
	void decPositionZ(bool coarseAdj);
};