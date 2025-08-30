#pragma once
#include "Element.h"

namespace Ling{
	class ElementBox:public Element
	{
	public:
		ElementBox();
		~ElementBox();
		virtual void paint(SkCanvas* canvas) override;
		std::vector<Element*>* getChildren() override;
		/// <summary>
		/// 添加了新元素之后，应重新执行布局
		/// </summary>
		/// <param name="index"></param>
		/// <param name="ele"></param>
		virtual void insertChild(const int& index, Element* ele);
		/// <summary>
		/// 添加了新元素之后，应重新执行布局
		/// </summary>
		/// <param name="ele"></param>
		virtual void addChild(Element* ele);
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
		/// <summary>
		/// 把所有子元素都添加到父元素中之后，才能执行布局
		/// </summary>
		void layout();
	protected:
		void casecadeShown();
	private:
		void calculateGlobalPos(std::vector<Element*>* children);
	private:
		std::vector<Element*> children;
	};
}

