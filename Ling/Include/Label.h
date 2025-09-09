#pragma once
#include <string>
#include <memory>
#include "FontWeight.h"
#include "FontWidth.h"
#include "FontSlant.h"
#include "Element.h"

struct YGSize;
struct YGNode;
enum YGMeasureMode;
typedef const struct YGNode* YGNodeConstRef;
namespace tvg {
	class Text;
}
namespace Ling {
	class Label :public Element
	{
		public:
			Label();
			~Label();
			void layout() override;
			void setWindow(WindowBase* win) override;
			const std::u8string& getText();
			void setText(const std::u8string& text);
			void setForegroundColor(const Color& foregroundColor);
			Color getForegroundColor();
			void setFontSize(const float& fontSize);
			float getFontSize();
			void setFontName(const std::string& fontName);
			std::string getFontName();
			void setItalic(const bool& italic);
			bool getItalic();
		protected:
		private:
			/// <summary>
			/// 如果你在 YGNodeStyleSetWidth(node, 100) / YGNodeStyleSetHeight(node, 50) 里已经指定了固定大小，Yoga 就直接用这个值，不会去调用 measureFunc。  
			/// 如果父容器已经约束住了大小，比如 flex:1 填充满了，Yoga 也不会再问 measureFunc。  
			/// </summary>
			/// <param name="node"></param>
			/// <param name="width"></param>
			/// <param name="widthMode"></param>
			/// <param name="height"></param>
			/// <param name="heightMode"></param>
			/// <returns></returns>
			static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
			void measure();
		private:
			std::u8string text;
			//std::shared_ptr<SkFont> font;
			//std::unique_ptr<SkRect> measuredRect;
			float fontSize{ 14.f },mWidth,mHeight;
			tvg::Text* textShape;
			Color foregroundColor;
			std::string fontName;
			bool italic{ false };
	};
}

