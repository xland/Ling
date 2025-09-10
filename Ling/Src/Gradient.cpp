#include <thorvg.h>
#include "../Include/Gradient.h"

namespace Ling{

	Gradient::Gradient(const GradientType& gradientType) : gradientType{gradientType}
	{
	}

	Gradient::~Gradient()
	{
		//delete fill;
	}

	void Gradient::addColor(const float& offset, const Color& color)
	{
		stops.push_back({ offset,color.getR(),color.getG(), color.getB(), color.getA() });
	}
	GradientType Gradient::getGradientType() const
	{
		return gradientType;
	}
	const tvg::Fill::ColorStop* Gradient::getData() const
	{
		return stops.data();
	}
	int Gradient::getDataCount() const
	{
		return stops.size();
	}
}