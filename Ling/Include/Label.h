#pragma once
#include <string>
#include <memory>
#include "FontWeight.h"
#include "FontWidth.h"
#include "FontSlant.h"
#include "Element.h"

struct YGSize;
struct YGNode;
class SkFont;
struct SkRect;
enum YGMeasureMode;
typedef const struct YGNode* YGNodeConstRef;
namespace Ling {
	class Label :public Element
	{
		public:
			Label();
			~Label();
			void paint(SkCanvas* canvas) override;
			const std::string& getText();
			SkFont* getFont();
			void setText(const std::string& text);
			void setFont(const std::string& fontName = "Microsoft YaHei",
				const FontWeight& fontWeight = FontWeight::Normal,
				const FontWidth& fontWidth = FontWidth::Normal,
				const FontSlant& fontSlant = FontSlant::Upright);
			void setFontSize(const float& fontSize);
			float getFontSize();
		protected:
		private:
			static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
			void measure();
		private:
			std::string text;
			std::shared_ptr<SkFont> font;
			float fontSize{ 14.f };
			std::unique_ptr<SkRect> measuredRect;
	};
}

