#include "LiveTransformer.h"

#include <string>
#include <sstream>
#include <iomanip>


std::string LiveTransformer::getTransformAndRenderCodeString()
{
	std::ostringstream oss;

	// Set the precision to 2 decimal places and fixed point notation
	oss << std::fixed << std::setprecision(2);

	oss << "    TransformAndRender(" << std::endl
	    << "        \"" << objectName << "\"," << std::endl
	    << "        std::bind(&ShapeMeshes::Draw____Mesh, m_basicMeshes)," << std::endl
	    << "        //  x           y           z" << std::endl
		<< "        " << std::setw(7) << XscaleAdjusted    << "f,   " << std::setw(7) << YscaleAdjusted    << "f,   " << std::setw(7) << ZscaleAdjusted    << "f,      // scale" << std::endl
	    << "        " << std::setw(7) << XrotationAdjusted << "f,   " << std::setw(7) << YrotationAdjusted << "f,   " << std::setw(7) << ZrotationAdjusted << "f,      // rotation" << std::endl
	    << "        " << std::setw(7) << XpositionAdjusted << "f,   " << std::setw(7) << YpositionAdjusted << "f,   " << std::setw(7) << ZpositionAdjusted << "f,      // position" << std::endl
        << "        //  r           g           b" << std::endl
		<< "        " << std::setw(7) << RcolorAdjusted    << "f,   " << std::setw(7) << GcolorAdjusted    << "f,   " << std::setw(7) << BcolorAdjusted    << "f       // color" << std::endl
		<< "    );" << std::endl;

	return oss.str();
}

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
