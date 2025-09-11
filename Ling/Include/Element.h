#pragma once
#include <Windows.h>
#include <memory>
#include "Node.h"
#include "Color.h"
#include "Cursor.h"
#include "Gradient.h"
#include "GradientType.h"

namespace tvg {
	class Shape;
	class Fill;
}

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
			WindowBase* getWindow();
			virtual void layout();
			void update();
			void setBorderWidth(const float& borderWidth);
			float getBorderWidth();
			void setRadius(const float& radius);
			float getRadius();
			void setBackgroundColor(const Color& backgroundColor);
			void setBackgroundColor(const std::shared_ptr<Gradient>& backgroundGradient);
			void setBorderColor(const Color& borderColor);
			void setCaptionFlag(bool captionFlag);
			bool getCaptionFlag();
			void setCursor(const Cursor& cursor);
			Cursor getCursor();
		public:
			friend class MouseEvent;
			friend class ElementBox;
			friend class WindowBase;
			friend class Label;
		protected:
			virtual void setWindow(WindowBase* win);
		private:
			void setParent(Element* ele);
		private:
			std::shared_ptr<Gradient> backgroundGradient;
			Element* parent{ nullptr };
			WindowBase* win;
			float radius{ 0.f };
			bool captionFlag{ false };
			Cursor cursor{Cursor::Arrow};
			tvg::Shape* shape;
	};
}

