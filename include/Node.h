#pragma once
#include <tuple>
#include <winrt/Windows.UI.Composition.h>
#include <yoga/Yoga.h>
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"
#include "Color.h"
namespace Ling {
	class WinBase;
	class Node
	{
		friend class WinBase;
		public:
			Node(WinBase* parent);
			virtual ~Node();
			template<typename T> requires std::derived_from<T, Node>
			T* makeChild();
			std::unique_ptr<Node> detachChild(Node* child); 
			void removeChild(Node* child);            
			bool isPosIn(POINT pos);
			void hide();
			void show();
			void setBg(const Color& color);

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
			void setMarginLeft(const float& val);
			void setMarginTop(const float& val);
			void setMarginRight(const float& val);
			void setMarginBottom(const float& val);
			float getMarginLeft();
			float getMarginTop();
			float getMarginRight();
			float getMarginBottom();

			void setPadding(const float& val);
			void setPadding(const float& left, const float& top, const float& right, const float& bottom);
			void setPaddingLeft(const float& val);
			void setPaddingTop(const float& val);
			void setPaddingRight(const float& val);
			void setPaddingBottom(const float& val);
			float getPaddingLeft();
			float getPaddingTop();
			float getPaddingRight();
			float getPaddingBottom();

			void setFlexWrap(const Wrap& val);
			/// <summary>
			/// 用来设置 子元素在交叉轴 上的对齐方式。
			/// 主轴 ：由 flex-direction 决定（水平方向或垂直方向）。
			/// 交叉轴 ：与主轴垂直的方向。
			/// </summary>
			/// <param name="val"></param>
			void setAlignItems(const Align& val);
			/// <summary>
			/// 控制子元素在 主轴的分布方式
			/// </summary>
			/// <param name="val"></param>
			void setJustifyContent(const Justify& val);
			/// <summary>
			/// 设置主轴的方向
			/// </summary>
			/// <param name="flexDirection"></param>
			void setFlexDirection(const FlexDirection& flexDirection);
		public:
			WinBase* win;
			winrt::Windows::UI::Composition::SpriteVisual visual{ nullptr };
			float x{ 0.f }, y{ 0.f }, w{ 0.f }, h{ 0.f };
			YGNodeRef node{ nullptr };
			Node* parent{ nullptr };
			std::vector<std::unique_ptr<Node>> children;
		protected:
			virtual void layout();
		private:
	};

	template<typename T> requires std::derived_from<T, Node>
	T* Node::makeChild() {
		auto node = new T(win);
		node->parent = this;
		visual.Children().InsertAtTop(node->visual);
		YGNodeInsertChild(this->node, node->node, YGNodeGetChildCount(this->node));

		auto savePtr = std::unique_ptr<T>(node);
		children.push_back(std::move(savePtr));
		return node;
	}
}