#pragma once
#include <tuple>
#include <optional>
#include <winrt/Windows.UI.Composition.h>
#include <yoga/Yoga.h>
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"
#include "Position.h"
#include "Edge.h"
#include "Color.h"
namespace Ling {
	class WinBase;
	class Node: public std::enable_shared_from_this<Node>
	{
		friend class WinBase;
		public:
			Node(WinBase* parent);
			virtual ~Node();
			void setId(const std::wstring& id);
			template<typename T> requires std::derived_from<T, Node>
			T* makeChild();
			std::unique_ptr<Node> detachChild(Node* child);
			void removeChild(Node* child);
			bool isPosIn(POINT pos);
			void hide();
			void show();
			virtual void setBg(const Color& color);
			/// <summary>
			/// 父容器在主轴方向上空间有剩余，子元素瓜分并放大尺寸
			/// </summary>
			/// <param name="val"></param>
			void setFlexGrow(float val);
			/// <summary>
			/// 当父容器的可用空间小于所有子节点尺寸总和时，该属性才会生效。
			/// 按照设定的 val 值比例，承担空间不足的压缩量。默认值为 1，表示当空间不足时会按比例缩小；若设为 0，则节点拒绝收缩，保持原始尺寸。
			/// </summary>
			/// <param name="val"></param>
			void setFlexShrink(float val);
			void setWidth(float w);
			void setHeight(float h);
			void setSize(float w, float h);
			void setWidthPercent(float percent);
			void setHeightPercent(float percent);
			void setSizePercent(float w, float h);
			void setMargin(float val);
			void setMargin(float left, float top, float right, float bottom);
			void setMarginLeft(float val);
			void setMarginTop(float val);
			void setMarginRight(float val);
			void setMarginBottom(float val);
			float getMarginLeft();
			float getMarginTop();
			float getMarginRight();
			float getMarginBottom();

			void setPadding(float val);
			void setPadding(float left, float top, float right, float bottom);
			void setPaddingLeft(float val);
			void setPaddingTop(float val);
			void setPaddingRight(float val);
			void setPaddingBottom(float val);
			float getPaddingLeft();
			float getPaddingTop();
			float getPaddingRight();
			float getPaddingBottom();
			std::tuple<float, float, float, float> getPadding();

			// 圆角 & 边框。逻辑像素，内部乘 dpi。
			// - setCornerRadius(0) / setBorderWidth(0) 表示无对应效果。
			// - 无 setBg 时背景走透明 —— clip 依然生效，圆角剪切对子节点仍有效。
			// - 边框沿 (0,0)-(w,h) 外沿绘制，几何自动内 inset borderW/2 保证 stroke 外沿贴边。
			void setBorderRadius(float r);
			void setBorder(float width, const Color& color);
			void setBorderWidth(float width);
			virtual void setBorderColor(const Color& color);

			void setPositionType(const Position val);

			void setPosition(Edge edge, float val);

			void setFlexWrap(const Wrap val);
			/// <summary>
			/// 用来设置 子元素在交叉轴 上的对齐方式。
			/// </summary>
			void setAlignItems(const Align val);
			/// <summary>
			/// 控制子元素在 主轴的分布方式
			/// </summary>
			void setJustifyContent(const Justify val);
			/// <summary>
			/// 设置主轴的方向
			/// </summary>
			void setFlexDirection(const FlexDirection flexDirection);

			// dpi 变化时由 WinBase 递归调用：
			//   1) 用缓存的逻辑值 × 新 dpi 重推 yoga 样式；
			//   2) 调用 onDpiChanged() 让子类重建依赖 dpi 的资源；
			//   3) 递归所有子节点。
			void applyDpiChange();
		public:
			WinBase* win;
			winrt::Windows::UI::Composition::SpriteVisual visual{ nullptr };
			float x{ 0.f }, y{ 0.f }, w{ 0.f }, h{ 0.f };
			YGNodeRef node{ nullptr };
			Node* parent{ nullptr };
			std::vector<std::unique_ptr<Node>> children;
			std::wstring id;
		protected:
			virtual void layout();
			virtual void onDpiChanged() {}
			virtual void setChild(Node* child);
			// 根据当前 w/h/dpi 同步圆角 clip 与边框几何；layout() 末尾调用。
			void syncChrome();
			// 生命周期哨兵：与 this 同生死。事件回调捕获返回值后用 lock()
			// 判定 this 是否还活着；不再触发 std::bad_weak_ptr。
			// 用 weak_ptr<bool> 而不是 weak_ptr<T>，因为 Node 当前用 unique_ptr 管理，
			// 没有真正的 shared_ptr<T> 可借；我们只用控制块的强引用计数来判生死。
			std::weak_ptr<bool> getWeakThis() {
				return std::weak_ptr<bool>(alive);
			}
		protected:
			Color bgColor{0};
		private:
		private:
			std::optional<float> width, height;
			std::optional<float> margin[4];   // left, top, right, bottom
			std::optional<float> padding[4];
			std::optional<float> edges[4];

			// 圆角 & 边框 —— 逻辑像素缓存，DPI 变化时物理像素在 syncChrome() 里重算。
			float cornerRadius{ 0.f };
			float borderWidth{ 0.f };
			Color borderColor{ 0 };
			// 懒创建：只有 cornerRadius>0 时才有 clipGeo；只有 borderWidth>0 时才有 borderVisual。
			winrt::Windows::UI::Composition::CompositionRoundedRectangleGeometry clipGeo{ nullptr };
			winrt::Windows::UI::Composition::ShapeVisual borderVisual{ nullptr };
			winrt::Windows::UI::Composition::CompositionRoundedRectangleGeometry borderGeo{ nullptr };
			winrt::Windows::UI::Composition::CompositionSpriteShape borderShape{ nullptr };
			// 生命周期哨兵：与 this 同生死（Node 析构时 alive 自动析构，强引用归零）。
			// getWeakThis() 借它生成 weak_ptr<bool>；事件回调 lock() 非空即代表 this 还活。
			std::shared_ptr<bool> alive{ std::make_shared<bool>(true) };
	};

	template<typename T> requires std::derived_from<T, Node>
	T* Node::makeChild() {
		auto node = new T(win);
		setChild(node);
		auto savePtr = std::unique_ptr<T>(node);
		children.push_back(std::move(savePtr));
		return node;
	}
}
