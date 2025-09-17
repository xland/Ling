#pragma once
#include "Align.h"
#include "Justify.h"
#include "Edge.h"
#include "Position.h"
#include "Size.h"
#include "FlexDirection.h"

#include "Event.h"

struct YGNode;
namespace Ling {
	class Node :public Event
	{
		public:
			Node();
			~Node();
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
			virtual void setSize(const float& w, const float& h);
			float getLeft();
			float getTop();
			float getWidth();
			float getHeight();
			float getGlobalX();
			float getGlobalY();
			Position getPosition();
			Size getSize();
			bool hittest(const int& x, const int& y);

			void setWidthPercent(const float& percent);
			void setHeightPercent(const float& percent);
			void setSizePercent(const float& w, const float& h);

			void setMargin(const float& val);
			void setMargin(const float& left, const float& top, const float& right, const float& bottom);
			void setMarginLeft(const float& val);
			void setMarginTop(const float& val);
			void setMarginRight(const float& val);
			void setMarginBottom(const float& val);
			float getMarginLeft();
			float getMarginTop();
			float getMarginRight();
			float getMarginBottom();

		public:
			friend class ElementBox;
			friend class Element;
			friend class WindowBase;
			friend class Label;
			friend class TextBlock;
			friend class TextArea;
		private:
			void insertChild(Node* node,const int& index);
			void addChild(Node* node);
		private:			
			float globalX{ 0 }, globalY{ 0 };
			YGNode* node;
	};
}
