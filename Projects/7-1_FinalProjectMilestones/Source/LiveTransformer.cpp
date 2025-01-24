#include "LiveTransformer.h"

#include <string>

void LiveTransformer::setSelectedObject(std::string objectName)
{
	this->selectedObject = objectName;
}

std::string LiveTransformer::getSelectedObject()
{
	return this->selectedObject;
}

void LiveTransformer::reset()
{
	XscaleAdj    = YscaleAdj    = ZscaleAdj    = 0.0f;
	XrotationAdj = YrotationAdj = ZrotationAdj = 0.0f;
	XpositionAdj = YpositionAdj = ZpositionAdj = 0.0f;
}

void LiveTransformer::incScaleAdjX(bool coarseAdj)
{
	XscaleAdj += (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::incScaleAdjY(bool coarseAdj)
{
	YscaleAdj += (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::incScaleAdjZ(bool coarseAdj)
{
	ZscaleAdj += (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::incRotationX(bool coarseAdj)
{
	XrotationAdj += (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::incRotationY(bool coarseAdj)
{
	YrotationAdj += (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::incRotationZ(bool coarseAdj)
{
	ZrotationAdj += (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::incPositionX(bool coarseAdj)
{
	XpositionAdj += (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::incPositionY(bool coarseAdj)
{
    YpositionAdj += (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::incPositionZ(bool coarseAdj)
{
	ZpositionAdj += (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::decScaleAdjX(bool coarseAdj)
{
	XscaleAdj -= (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::decScaleAdjY(bool coarseAdj)
{
	YscaleAdj -= (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::decScaleAdjZ(bool coarseAdj)
{
	ZscaleAdj -= (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::decRotationX(bool coarseAdj)
{
	XrotationAdj -= (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::decRotationY(bool coarseAdj)
{
	YrotationAdj -= (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::decRotationZ(bool coarseAdj)
{
	ZrotationAdj -= (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::decPositionX(bool coarseAdj)
{
	XpositionAdj -= (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::decPositionY(bool coarseAdj)
{
	YpositionAdj -= (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformer::decPositionZ(bool coarseAdj)
{
	ZpositionAdj -= (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}
