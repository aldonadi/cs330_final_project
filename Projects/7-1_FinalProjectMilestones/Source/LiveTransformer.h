#pragma once

#include <string>

class LiveTransformer {
private:
	std::string selectedObject = "";

public:

	void setSelectedObject(std::string objectName);
	std::string getSelectedObject();

	// todo: find a better way to get the effective vertex data printout to the UI
	std::string effectiveTransformationDataString = "";

	float XscaleAdj = 0.0f;
	float YscaleAdj = 0.0f;
	float ZscaleAdj = 0.0f;
		  
	float XrotationAdj = 0.0f;
	float YrotationAdj = 0.0f;
	float ZrotationAdj = 0.0f;
		  
	float XpositionAdj = 0.0f;
	float YpositionAdj = 0.0f;
	float ZpositionAdj = 0.0f;

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