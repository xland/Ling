#include <thorvg.h>
#include "../Include/Gradient.h"

namespace Ling{

	Gradient::Gradient(const GradientType& gradientType) : gradientType{gradientType}
	{
		if (gradientType == GradientType::Linear) {
			fill = tvg::LinearGradient::gen();
		}
		else if (gradientType == GradientType::Radial) {
			fill = tvg::RadialGradient::gen();
		}
	}

	Gradient::~Gradient()
	{
		//delete fill;
	}

	void Gradient::addColor(const float& offset, const Color& color)
	{
		stops.push_back({ offset,color });
	}
	GradientType Gradient::getGradientType() const
	{
		return gradientType;
	}
	std::vector<std::pair<float, Color>>* Gradient::getData()
	{
		return &stops;
	}
	tvg::Fill* Gradient::getFill()
	{
		return fill;
	}
}