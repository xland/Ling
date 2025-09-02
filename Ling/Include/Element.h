#pragma once
#include <Windows.h>
#include "Align.h"
#include "Justify.h"
#include "Edge.h"
#include "Color.h"
#include "Position.h"
#include "Size.h"
#include "FlexDirection.h"
#include "Event.h"

struct YGNode;
struct SkRect;
class SkCanvas;
class SkPaint;
namespace Ling {
	class MouseEvent;
	class WindowBase;
	class ElementBox;
	class Label;
	class TextBlock;
	class Element:public Event
	{
		public:
			Element();
			~Element();
			virtual std::vector<Element*>* getChildren() { return nullptr; };
			virtual void paint(SkCanvas* canvas);
			Element* getParent();
			/// <summary>
			/// 此元素如何在父元素主轴方向上“长大”占据剩余空间
			/// </summary>
			/// <param name="val"></param>
			void setFlexGrow(const float& val);
			/// <summary>
			/// 此元素如何在父元素主轴方向上“收缩”
			/// </summary>
			/// <param name="val"></param>
			void setFlexShrink(const float& val);
			void setWidth(const float& w);
			void setHeight(const float& h);
			void setSize(const float& w, const float& h);
			void setWidthPercent(const float& percent);
			void setHeightPercent(const float& percent);
			void setSizePercent(const float& w, const float& h);
			void setMargin(const float& val);
			void setMargin(const float& left, const float& top, const float& right, const float& bottom);
			void setMargin(const Edge& type, const float& val);
			void setPadding(const float& val);
			void setPadding(const float& left, const float& top, const float& right, const float& bottom);
			void setPadding(const Edge& type, const float& val);
			void setBorderWidth(const float& width);
			void setBorderColor(const Color& color); 
			void setBackgroundColor(const Color& color);
			void setRadius(float r);
			void setRadius(float lt, float rt, float rb, float lb);
			void setCaption(bool flag);
			bool getCaption();
			Position getPosition();
			Size getSize();
			WindowBase* getWindow();
			bool hittest(const int& x, const int& y);
			void update();
		public:
			friend class MouseEvent;
			friend class ElementBox;
			friend class Label;
			friend class TextBlock;
			friend class WindowBase;
		protected:

		protected:

		private:
			void paintRect(SkCanvas* canvas, const SkPaint& paint, const SkRect& rect);
		private:
			Element* parent{ nullptr };
			YGNode* node;
			WindowBase* win;
			float borderWidth;
			float radiusLT{ 0.f }, radiusRT{ 0.f }, radiusRB{ 0.f }, radiusLB{ 0.f };
			Color bgColor{ 0x00000000 }, borderColor{ 0x00000000 };
			bool isCaption{ false };
			float globalX{ 0 }, globalY{ 0 };
	};
}

