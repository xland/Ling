#pragma once
#include <vector>
#include "GradientType.h"
#include "Color.h"

namespace tvg {
	class Fill;
	struct Fill::ColorStop;
}
namespace Ling {
	class Gradient
	{
		public:
			Gradient(const GradientType& gradientType);
			~Gradient();
			void addColor(const float& offset, const Color& color);
			GradientType getGradientType() const;
			const tvg::Fill::ColorStop* getData() const;
			int getDataCount() const;
		private:
			std::vector<tvg::Fill::ColorStop> stops;
			GradientType gradientType;
	};
}