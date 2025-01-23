#include "LiveTransformationManager.h"

#include <string>

void LiveTransformationManager::setSelectedObject(std::string objectName)
{
	this->selectedObject = objectName;
}

std::string LiveTransformationManager::getSelectedObject()
{
	return this->selectedObject;
}

void LiveTransformationManager::reset()
{
	XscaleAdj    = YscaleAdj    = ZscaleAdj    = 0.0f;
	XrotationAdj = YrotationAdj = ZrotationAdj = 0.0f;
	XpositionAdj = YpositionAdj = ZpositionAdj = 0.0f;
}

void LiveTransformationManager::incScaleAdjX(bool coarseAdj)
{
	XscaleAdj += (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::incScaleAdjY(bool coarseAdj)
{
	YscaleAdj += (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::incScaleAdjZ(bool coarseAdj)
{
	ZscaleAdj += (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::incRotationX(bool coarseAdj)
{
	XrotationAdj += (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::incRotationY(bool coarseAdj)
{
	YrotationAdj += (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::incRotationZ(bool coarseAdj)
{
	ZrotationAdj += (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::incPositionX(bool coarseAdj)
{
	XpositionAdj += (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::incPositionY(bool coarseAdj)
{
    YpositionAdj += (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::incPositionZ(bool coarseAdj)
{
	ZpositionAdj += (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::decScaleAdjX(bool coarseAdj)
{
	XscaleAdj -= (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::decScaleAdjY(bool coarseAdj)
{
	YscaleAdj -= (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::decScaleAdjZ(bool coarseAdj)
{
	ZscaleAdj -= (scaleAdjIncrement * (coarseAdj ? scaleCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::decRotationX(bool coarseAdj)
{
	XrotationAdj -= (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::decRotationY(bool coarseAdj)
{
	YrotationAdj -= (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::decRotationZ(bool coarseAdj)
{
	ZrotationAdj -= (rotationAdjIncrement * (coarseAdj ? rotCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::decPositionX(bool coarseAdj)
{
	XpositionAdj -= (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::decPositionY(bool coarseAdj)
{
	YpositionAdj -= (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}

void LiveTransformationManager::decPositionZ(bool coarseAdj)
{
	ZpositionAdj -= (positionAdjIncrement * (coarseAdj ? posCoarseAdjMultiplier : 1.0f));
}
