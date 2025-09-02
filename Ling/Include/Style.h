#pragma once
#include <array>
#include "Color.h"

namespace Ling {
	class Style
	{
		public:
			void setBorderWidth(const float& borderWidth);
			float getBorderWidth();
			void setRadius(const float& radius);
			void setRadius(const float& radiusLT, const float& radiusRT, const float& radiusRB, const float& radiusLB);
			std::array<float, 4> getRadius();
			float getRadiusLT();
			float getRadiusRT();
			float getRadiusRB();
			float getRadiusLB();
			void setBackgroundColor(const Color& backgroundColor);
			void setBorderColor(const Color& borderColor);
			Color& getBackgroundColor();
			Color& getBorderColor();
			void setCaptionFlag(bool captionFlag);
			bool getCaptionFlag();
			void setMargin(const float& val);
			void setMargin(const float& left, const float& top, const float& right, const float& bottom);
			float getMarginL();
			float getMarginT();
			float getMarginR();
			float getMarginB();
			void setMarginL(const float& val);
			void setMarginT(const float& val);
			void setMarginR(const float& val);
			void setMarginB(const float& val);
			void setPadding(const float& val);
			void setPadding(const float& left, const float& top, const float& right, const float& bottom);
			float getPaddingL();
			float getPaddingT();
			float getPaddingR();
			float getPaddingB();
			void setPaddingL(const float& val);
			void setPaddingT(const float& val);
			void setPaddingR(const float& val);
			void setPaddingB(const float& val);
		private:
			float borderWidth{0.f};
			float radiusLT{ 0.f }, radiusRT{ 0.f }, radiusRB{ 0.f }, radiusLB{ 0.f };
			float paddingL{ 0.f }, paddingT{ 0.f }, paddingR{ 0.f }, paddingB{ 0.f };
			float marginL{ 0.f }, marginT{ 0.f }, marginR{ 0.f }, marginB{ 0.f };
			Color backgroundColor{ 0x00000000 }, borderColor{ 0x00000000 };
			bool captionFlag{ false };
	};
}