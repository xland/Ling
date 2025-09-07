#pragma once
#include <string>
#include <vector>
#include "FontWeight.h"
#include "FontWidth.h"
#include "FontSlant.h"
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
			void paint(SkCanvas* canvas) override;
			const std::string& getText();
			void setText(const std::string& text);
			void setFont(const std::string& fontName = "Microsoft YaHei",
				const FontWeight& fontWeight = FontWeight::Normal,
				const FontWidth& fontWidth = FontWidth::Normal,
				const FontSlant& fontSlant = FontSlant::Upright);
			void setFontSize(const float& fontSize);
			float getFontSize();
		private:
			static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
			void measure();
		private:
			std::string text;
			std::shared_ptr<SkFont> font;
			float fontSize{ 14.f };
			float lineSpace{ 8.f };
			std::vector<LineSizeInfo> lineSizeInfos;
			float measuredWidth, measuredHeight;
	};
}

