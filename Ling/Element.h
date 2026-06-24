#pragma once
#include "Header.h"
#include <yoga/Yoga.h>
#include "Color.h"
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"

namespace Ling {
	class WindowBase;
	class Element : public std::enable_shared_from_this<Element>
	{
	public:
		Element();
		~Element();
		void insertChild(const int& index, const std::shared_ptr<Element>& ele);
		void addChild(const std::shared_ptr<Element>& ele);
		virtual void layout();
		void setBackgroundColor(const Color& backgroundColor);
		void setWidth(const float& w);
		void setHeight(const float& h);
		virtual void setSize(const float& w, const float& h);
		void setWidthPercent(const float& percent);
		void setHeightPercent(const float& percent);
		void setSizePercent(const float& w, const float& h);
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
		float getLeft();
		float getTop();
		float getWidth();
		float getHeight();

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
	protected:
		virtual void initProperty(const Composition::Compositor& comp);
	protected:
		YGNodeRef node{ nullptr };
		Composition::SpriteVisual visual{ nullptr };
		WindowBase* win{ nullptr };
		Element* parent{ nullptr };
		std::vector<std::shared_ptr<Element>> children;
		float xAbs{ 0.f }, yAbs{ 0.f }, x{0.f}, y{ 0.f }, w{ 0.f }, h{ 0.f };
		Color backgroundColor;
	private:
	};
}
