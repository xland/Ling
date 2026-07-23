#pragma once
#include <tuple>
#include <optional>
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

			// 下列所有 setter 一律接收“逻辑像素”，内部乘 dpi 塞给 yoga。
			void setFlexGrow(const float& val);
			/// <summary>
			/// 此元素如何在父元素主轴方向上“收缩”
			/// </summary>
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
			/// </summary>
			void setAlignItems(const Align& val);
			/// <summary>
			/// 控制子元素在 主轴的分布方式
			/// </summary>
			void setJustifyContent(const Justify& val);
			/// <summary>
			/// 设置主轴的方向
			/// </summary>
			void setFlexDirection(const FlexDirection& flexDirection);

			// dpi 变化时由 WinBase 递归调用：
			//   1) 用缓存的逻辑值 × 新 dpi 重推 yoga 样式；
			//   2) 调用 onDpiChanged() 让子类重建依赖 dpi 的资源；
			//   3) 递归所有子节点。
			void applyDpiChange();
		public:
			WinBase* win;
			winrt::Windows::UI::Composition::SpriteVisual visual{ nullptr };
			// 布局输出：物理像素（yoga 计算结果）
			float x{ 0.f }, y{ 0.f }, w{ 0.f }, h{ 0.f };
			YGNodeRef node{ nullptr };
			Node* parent{ nullptr };
			std::vector<std::unique_ptr<Node>> children;
		protected:
			virtual void layout();
			// 子类覆盖：dpi 变化后重建自身依赖 dpi 的资源（字体大小、缓存位图等）
			virtual void onDpiChanged() {}
		private:
			// 逻辑像素缓存 —— 只保存跟 dpi 相关的样式项。
			// 没设置过的项为 std::nullopt，applyDpiChange 时跳过。
			struct LogicalStyle {
				std::optional<float> width, height;
				std::optional<float> margin[4];   // left, top, right, bottom
				std::optional<float> padding[4];
			} logical;
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
