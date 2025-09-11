#pragma once
#include <vector>
#include "GradientType.h"
#include "Color.h"

namespace tvg {
	class Fill;
}

namespace Ling {
	class Gradient
	{
		public:
			Gradient() = default;
			Gradient(const GradientType& gradientType);
			~Gradient();
			void addColor(const float& offset, const Color& color);
			GradientType getGradientType() const;
			std::vector<std::pair<float, Color>>* getData();
			tvg::Fill* getFill();
		private:
			std::vector<std::pair<float,Color>> stops;
			GradientType gradientType;
			tvg::Fill* fill;
	};
}