#pragma once
#include <string>
#include "Element.h"
#include "FontWeight.h"
#include "FontWidth.h"
#include "FontSlant.h"

struct YGSize;
struct YGNode;
enum YGMeasureMode;
typedef const struct YGNode* YGNodeConstRef;
namespace Ling {
	struct LineGlyphInfo;
	class TextArea :public Element
	{
		public:
			TextArea();
			~TextArea();
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
			void shown() override;
			void mouseDown(const MouseEvent& event) override;
		private:
			static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
			void measure();
		private:
			std::string text;
			std::shared_ptr<SkFont> font;
			float fontSize{ 16.f };
			float lineSpace{ 18.f };
			std::vector<LineGlyphInfo> lineGlyphInfos;
			float measuredWidth, measuredHeight;
			bool caretVisible{false};
	};
}

