#pragma once
#include <Windows.h>
#include "Node.h"
#include "Color.h"
#include "Cursor.h"


namespace Ling {
	class MouseEvent;
	class WindowBase;
	class ElementBox;
	class Label;
	class TextBlock;
	class Element:public Node
	{
		public:
			Element();
			~Element();		
			virtual std::vector<Element*>* getChildren() { return nullptr; };
			Element* getParent();
			virtual void paint(SkCanvas* canvas);			
			WindowBase* getWindow();			
			void update();

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
			void setCursor(const Cursor& cursor);
			Cursor getCursor();
			void paintStyle(SkCanvas* canvas, SkRect& rect);
		public:
			friend class MouseEvent;
			friend class ElementBox;
			friend class WindowBase;
		protected:
		private:
			void setWindow(WindowBase* win);
			void setParent(Element* ele);
			void paintRect(SkCanvas* canvas, const SkPaint& paint, const SkRect& rect);
		private:
			Element* parent{ nullptr };
			WindowBase* win;
			float borderWidth{ 0.f };
			float radiusLT{ 0.f }, radiusRT{ 0.f }, radiusRB{ 0.f }, radiusLB{ 0.f };
			Color backgroundColor{ 0x00000000 }, borderColor{ 0x00000000 };
			bool captionFlag{ false };
			Cursor cursor{Cursor::Arrow};
	};
}

