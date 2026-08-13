#pragma once
#include "Node.h"

namespace Ling {
	class WinBase;
	class Slider : public Node
	{
	public:
		Slider(WinBase* win);
		~Slider();

		// 值域。max <= min 时滑块锁在最左端（不崩，只是没有可拖行程）。
		// 改值域后当前值会被重新夹紧到新范围内。
		void setRange(float min, float max);
		void setValue(float val);
		float getValue() const;
		float getMin() const;
		float getMax() const;
		// 步进。0 = 连续（默认）；>0 则值吸附到 min + n*step。
		void setStep(float val);
		// 窗口 x 坐标处对应的值：在这里按下去会得到什么值，就返回什么值（已夹紧 + step 吸附）。
		// px 与 isPosIn 同坐标系（窗口绝对坐标，物理像素）。
		// 给外部做悬停提示用 —— 鼠标只是路过滑轨时 value 并没有变，拿 getValue 是不对的。
		float getValueAt(float px) const;

		void setTrackColor(const Color& color);
		void setFillColor(const Color& color);
		void setThumbColor(const Color& color);
		void setHoverThumbColor(const Color& color);
	public:
		// 值发生变化时触发（拖动 / 滚轮 / setValue 都会触发）。值未变则不触发。
		winrt::event<winrt::delegate<Slider*, float>> onValueChanged;
	private:
		// 内容完全由自身绘制，屏蔽掉子节点与内部布局相关的 API（同 Text/Image 的做法）。
		// setWidth/setHeight/setFlexGrow/setMargin/setBg/setBorder* 保持可用。
		using Node::makeChild;
		using Node::setPadding;
		using Node::setPaddingLeft;
		using Node::setPaddingTop;
		using Node::setPaddingRight;
		using Node::setPaddingBottom;
		using Node::setFlexWrap;
		using Node::setAlignItems;
		using Node::setJustifyContent;
		using Node::setFlexDirection;

		void layout() override;
		// 把当前 value/w/h/dpi 写进 Composition 的几何属性。
		void syncVisual();
		// 窗口 x 坐标 -> 值（不吸附，供内部拖动与 getValueAt 复用）。
		// x 是 Node::layout 累加后的窗口绝对坐标，与 isPosIn 同坐标系。
		float posToValue(float px) const;
		// clamp 到值域 + step 吸附。
		float snap(float v) const;
		void onDown(POINT pos, bool isRight);
		void onMove(POINT pos);
		void onUp(POINT pos, bool isRight);
		void onWheel(POINT pos, float space);
	private:
		winrt::Windows::UI::Composition::ShapeVisual shapeVisual{ nullptr };
		winrt::Windows::UI::Composition::CompositionRoundedRectangleGeometry trackGeo{ nullptr };
		winrt::Windows::UI::Composition::CompositionRoundedRectangleGeometry fillGeo{ nullptr };
		winrt::Windows::UI::Composition::CompositionEllipseGeometry thumbGeo{ nullptr };
		winrt::Windows::UI::Composition::CompositionSpriteShape trackShape{ nullptr };
		winrt::Windows::UI::Composition::CompositionSpriteShape fillShape{ nullptr };
		winrt::Windows::UI::Composition::CompositionSpriteShape thumbShape{ nullptr };
		// 滑块两态的 brush 缓存下来，hover 切换只做引用替换，不再每次 new（同 Button）。
		winrt::Windows::UI::Composition::CompositionColorBrush thumbBrush{ nullptr };
		winrt::Windows::UI::Composition::CompositionColorBrush hoverThumbBrush{ nullptr };
		winrt::event_token downTok{}, moveTok{}, upTok{}, wheelTok{};
		float minVal{ 0.f }, maxVal{ 100.f }, value{ 0.f }, step{ 0.f };
		bool dragging{ false }, isHover{ false };
	};
}
