#pragma once
#include <string>
#include <vector>
#include "Element.h"

struct YGSize;
struct YGNode;
enum YGMeasureMode;
typedef const struct YGNode* YGNodeConstRef;
namespace Ling {
	struct LineSizeInfo;
	class TextBlock :public Element
	{
		public:
			TextBlock();
			~TextBlock();
			//void paint(SkCanvas* canvas) override;
			const std::string& getText();
			void setText(const std::string& text);
			void setFontSize(const float& fontSize);
			float getFontSize();
		private:
			static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
			void measure();
		private:
			std::string text;
			//std::shared_ptr<SkFont> font;
			float fontSize{ 14.f };
			float lineSpace{ 8.f };
			std::vector<LineSizeInfo> lineSizeInfos;
			float measuredWidth, measuredHeight;
	};
}

