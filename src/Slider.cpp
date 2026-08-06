#include "pch.h"
#include "../include/Slider.h"
#include "../include/WinBase.h"

namespace Ling {

	// 逻辑像素常量（内部乘 dpi），风格同 ScrollerBox 的 sliderW/sliderMinH。
	constexpr float trackH{ 1.f };     // 轨道粗细
	constexpr float thumbR{ 3.f };     // 滑块半径
	// 滚轮一格改变值域的 1/20（无 step 时）。
	constexpr float wheelNotches{ 20.f };

	Slider::Slider(WinBase* win) : Node(win)
	{
		// 走 Node::setWidth/setHeight，逻辑值会被缓存进 width/height，
		// DPI 变化时 applyDpiChange 能自动重推物理像素。
		setWidth(200.f);
		setHeight(20.f);

		shapeVisual = win->compositor.CreateShapeVisual();

		trackGeo = win->compositor.CreateRoundedRectangleGeometry();
		trackShape = win->compositor.CreateSpriteShape(trackGeo);
		trackShape.FillBrush(win->compositor.CreateColorBrush(Color(0xDDDDDDFF).getUIColor()));

		fillGeo = win->compositor.CreateRoundedRectangleGeometry();
		fillShape = win->compositor.CreateSpriteShape(fillGeo);
		fillShape.FillBrush(win->compositor.CreateColorBrush(Color(0x3B82F6FF).getUIColor()));

		thumbGeo = win->compositor.CreateEllipseGeometry();
		thumbShape = win->compositor.CreateSpriteShape(thumbGeo);
		thumbBrush = win->compositor.CreateColorBrush(Color(0x3B82F6FF).getUIColor());
		hoverThumbBrush = win->compositor.CreateColorBrush(Color(0x2563EBFF).getUIColor());
		thumbShape.FillBrush(thumbBrush);

		// 叠放顺序：后 Append 的在上。底轨 → 已选段 → 滑块。
		shapeVisual.Shapes().Append(trackShape);
		shapeVisual.Shapes().Append(fillShape);
		shapeVisual.Shapes().Append(thumbShape);
		visual.Children().InsertAtTop(shapeVisual);

		auto weakThis = getWeakThis();
		downTok  = win->onMouseDown .add([this, weakThis](POINT pos, bool isRight) { if (!weakThis.lock()) return; onDown(pos, isRight); });
		moveTok  = win->onMouseMove .add([this, weakThis](POINT pos)               { if (!weakThis.lock()) return; onMove(pos); });
		upTok    = win->onMouseUp   .add([this, weakThis](POINT pos, bool isRight) { if (!weakThis.lock()) return; onUp(pos, isRight); });
		wheelTok = win->onMouseWheel.add([this, weakThis](POINT pos, float space)  { if (!weakThis.lock()) return; onWheel(pos, space); });
	}

	Slider::~Slider()
	{
		win->onMouseDown .remove(downTok);
		win->onMouseMove .remove(moveTok);
		win->onMouseUp   .remove(upTok);
		win->onMouseWheel.remove(wheelTok);
	}

	// ---- 值 -------------------------------------------------------------------

	float Slider::snap(float v) const
	{
		if (maxVal <= minVal) return minVal;
		v = std::clamp(v, minVal, maxVal);
		if (step > 0.f) {
			v = minVal + std::round((v - minVal) / step) * step;
			// step 除不尽值域时，最后一格会超出 maxVal，再夹一次。
			v = std::clamp(v, minVal, maxVal);
		}
		return v;
	}

	void Slider::setRange(float min, float max)
	{
		minVal = min;
		maxVal = max;
		// 新值域可能把当前值挤出范围；这里必须重新夹紧。
		// 注意 setValue 内部有"值未变就早退"，所以 syncVisual 可能不被调用，
		// 但行程/比例都变了 —— 显式同步一次。
		const float old = value;
		value = snap(value);
		syncVisual();
		if (value != old) onValueChanged(this, value);
	}

	void Slider::setValue(float val)
	{
		val = snap(val);
		// 早退：拖动时 WM_MOUSEMOVE 很密，同一个值不重复写 Composition、不重复发事件。
		if (val == value) return;
		value = val;
		syncVisual();
		onValueChanged(this, value);
	}

	float Slider::getValue() const { return value; }
	float Slider::getMin() const { return minVal; }
	float Slider::getMax() const { return maxVal; }

	void Slider::setStep(float val)
	{
		step = val;
		setValue(value);   // 当前值可能不落在新的 step 格点上
	}

	// ---- 颜色 -----------------------------------------------------------------

	void Slider::setTrackColor(const Color& color)
	{
		trackShape.FillBrush(win->compositor.CreateColorBrush(color.getUIColor()));
	}

	void Slider::setFillColor(const Color& color)
	{
		fillShape.FillBrush(win->compositor.CreateColorBrush(color.getUIColor()));
	}

	void Slider::setThumbColor(const Color& color)
	{
		thumbBrush = win->compositor.CreateColorBrush(color.getUIColor());
		if (!isHover) thumbShape.FillBrush(thumbBrush);
	}

	void Slider::setHoverThumbColor(const Color& color)
	{
		hoverThumbBrush = win->compositor.CreateColorBrush(color.getUIColor());
		if (isHover) thumbShape.FillBrush(hoverThumbBrush);
	}

	// ---- 几何 -----------------------------------------------------------------

	void Slider::syncVisual()
	{
		if (w <= 0.f || h <= 0.f) return;
		const float d = win->dpi;
		const float r = thumbR * d;
		const float th = trackH * d;
		const float len = w - 2.f * r;      // 滑块中心的可行程 = 底轨长度
		if (len <= 0.f) return;             // 太窄，画不出来，别写脏属性

		shapeVisual.Size({ w, h });

		const float cy = h * 0.5f;
		const float trackTop = cy - th * 0.5f;
		const float ratio = maxVal > minVal ? (value - minVal) / (maxVal - minVal) : 0.f;
		const float cx = r + ratio * len;

		trackGeo.Size({ len, th });
		trackGeo.CornerRadius({ th * 0.5f, th * 0.5f });
		trackShape.Offset({ r, trackTop });

		// 已选段与底轨同起点同高，只是长度到滑块中心为止。
		fillGeo.Size({ ratio * len, th });
		fillGeo.CornerRadius({ th * 0.5f, th * 0.5f });
		fillShape.Offset({ r, trackTop });

		// EllipseGeometry 的 Center 保持 (0,0)，位移交给 shape.Offset。
		thumbGeo.Radius({ r, r });
		thumbShape.Offset({ cx, cy });
	}

	void Slider::layout()
	{
		Node::layout();
		syncVisual();
	}

	float Slider::posToValue(float px) const
	{
		const float r = thumbR * win->dpi;
		const float len = w - 2.f * r;
		if (len <= 0.f || maxVal <= minVal) return minVal;
		// x 已被 Node::layout 累加成窗口绝对坐标，和传入的 px 同一坐标系。
		const float ratio = std::clamp((px - (x + r)) / len, 0.f, 1.f);
		return minVal + ratio * (maxVal - minVal);
	}

	// ---- 交互 -----------------------------------------------------------------

	void Slider::onDown(POINT pos, bool isRight)
	{
		if (isRight) return;
		if (!visual.IsVisible()) return;
		if (!isPosIn(pos)) return;
		// 捕获鼠标：拖动过程中光标移出控件甚至移出窗口，仍能收到 WM_MOUSEMOVE。
		SetCapture(win->hwnd);
		dragging = true;
		// 点轨道任意处，滑块直接跳过去 —— 标准滑块行为。
		setValue(posToValue((float)pos.x));
	}

	void Slider::onMove(POINT pos)
	{
		if (dragging) {
			setValue(posToValue((float)pos.x));
			return;
		}
		if (!visual.IsVisible()) return;
		// mouseLeave 会广播 {INT_MAX, INT_MAX}，isPosIn 自然返回 false，hover 正确复位。
		const auto hoverFlag = isPosIn(pos);
		if (isHover == hoverFlag) return;
		isHover = hoverFlag;
		thumbShape.FillBrush(isHover ? hoverThumbBrush : thumbBrush);
	}

	void Slider::onUp(POINT pos, bool isRight)
	{
		if (!dragging) return;
		ReleaseCapture();
		dragging = false;
	}

	void Slider::onWheel(POINT pos, float space)
	{
		if (!visual.IsVisible()) return;
		if (!isPosIn(pos)) return;
		// WinBase::mouseWheel 传来的 space 是像素量（delta/WHEEL_DELTA * 60 * dpi），
		// 滑块要的是值增量，所以先还原成"滚了几格"。上滚为正 -> 值增大。
		const float notches = space / (60.f * win->dpi);
		const float delta = step > 0.f ? step : (maxVal - minVal) / wheelNotches;
		setValue(value + notches * delta);
	}
}
