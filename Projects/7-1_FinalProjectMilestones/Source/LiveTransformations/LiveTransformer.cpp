#include "LiveTransformer.h"

#include <string>


void LiveTransformer::reset()
{
	XscaleAdjusted = XscaleSaved;
	YscaleAdjusted = YscaleSaved;
	ZscaleAdjusted = ZscaleSaved;

	XrotationAdjusted = XrotationSaved;
	YrotationAdjusted = YrotationSaved;
	ZrotationAdjusted = ZrotationSaved;

	XpositionAdjusted = XpositionSaved;
	YpositionAdjusted = YpositionSaved;
	ZpositionAdjusted = ZpositionSaved;

	RcolorAdjusted = RcolorSaved;
	GcolorAdjusted = GcolorSaved;
	BcolorAdjusted = BcolorSaved;
}

void LiveTransformer::incScaleAdjX(float increment)
{
	XscaleAdjusted += increment;
}

void LiveTransformer::incScaleAdjY(float increment)
{
	YscaleAdjusted += increment;
}

void LiveTransformer::incScaleAdjZ(float increment)
{
	ZscaleAdjusted += increment;
}

void LiveTransformer::incRotationX(float increment)
{
	XrotationAdjusted += increment;
}

void LiveTransformer::incRotationY(float increment)
{
	YrotationAdjusted += increment;
}

void LiveTransformer::incRotationZ(float increment)
{
	ZrotationAdjusted += increment;
}

void LiveTransformer::incPositionX(float increment)
{
	XpositionAdjusted += increment;
}

void LiveTransformer::incPositionY(float increment)
{
    YpositionAdjusted += increment;
}

void LiveTransformer::incPositionZ(float increment)
{
	ZpositionAdjusted += increment;
}

void LiveTransformer::decScaleAdjX(float increment)
{
	XscaleAdjusted -= increment;
}

void LiveTransformer::decScaleAdjY(float increment)
{
	YscaleAdjusted -= increment;
}

void LiveTransformer::decScaleAdjZ(float increment)
{
	ZscaleAdjusted -= increment;
}

void LiveTransformer::decRotationX(float increment)
{
	XrotationAdjusted -= increment;
}

void LiveTransformer::decRotationY(float increment)
{
	YrotationAdjusted -= increment;
}

void LiveTransformer::decRotationZ(float increment)
{
	ZrotationAdjusted -= increment;
}

void LiveTransformer::decPositionX(float increment)
{
	XpositionAdjusted -= increment;
}

void LiveTransformer::decPositionY(float increment)
{
	YpositionAdjusted -= increment;
}

void LiveTransformer::decPositionZ(float increment)
{
	ZpositionAdjusted -= increment;
}
