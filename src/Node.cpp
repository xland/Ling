#include "pch.h"
#include "../include/WinBase.h"
#include "../include/Node.h"

namespace Ling {

	Node::Node(WinBase* win) :win{ win }, node(YGNodeNew()), visual{ win->compositor.CreateSpriteVisual() }
	{
	}

	Node::~Node()
	{
		children.clear();
		YGNodeFree(node);
	}

	void Node::setId(const std::wstring& id)
	{
		this->id = id;
	}

	std::unique_ptr<Node> Node::detachChild(Node* child) {
		auto it = std::find_if(children.begin(), children.end(), [child](const std::unique_ptr<Node>& p) { return p.get() == child; });
		if (it == children.end()) return nullptr;
		YGNodeRemoveChild(node, child->node);
		visual.Children().Remove(child->visual);
		child->parent = nullptr;
		std::unique_ptr<Node> owned = std::move(*it);
		children.erase(it);
		return owned;
	}

	void Node::removeChild(Node* child) {
		// 返回值丢弃 == 立即销毁该子节点（unique_ptr 临时对象析构）。
		if (detachChild(child)) win->refresh();
	}
	void Node::removeAllChildren()
	{
		// 不能写成 for (auto& c : children) detachChild(c.get()); ——
		// detachChild 内部会 children.erase()，vector 的 erase 使被删位置及其之后
		// 的迭代器全部失效，range-for 的 ++ 于是跳过下一个元素（每两个只删掉一个），
		// 且最后一次 erase 后可能越过 end()，是 UB。
		// 改成：先把 yoga / visual 的挂接逐个摘掉，最后一次性清空容器。
		for (auto& child : children) {
			YGNodeRemoveChild(node, child->node);
			visual.Children().Remove(child->visual);
			child->parent = nullptr;
		}
		// unique_ptr 析构即销毁子节点；clear 不触碰迭代器，安全。
		children.clear();
		win->refresh();
	}
	void Node::setChild(Node* child)
	{
		child->parent = this;
		visual.Children().InsertAtTop(child->visual);
		YGNodeInsertChild(this->node, child->node, YGNodeGetChildCount(this->node));
		// 保证边框始终盖在所有子节点之上（如果启用了边框）
		if (borderVisual) {
			visual.Children().Remove(borderVisual);
			visual.Children().InsertAtTop(borderVisual);
		}
	}
	bool Node::isPosIn(POINT pos)
	{
		if (pos.x > x && pos.x<x + w && pos.y>y && pos.y < y + h) {
			return true;
		}
		return false;
	}
	void Node::hide()
	{
		visual.IsVisible(false);
		YGNodeStyleSetDisplay(node, YGDisplayNone);
		win->refresh();
	}

	void Node::show()
	{
		visual.IsVisible(true);
		YGNodeStyleSetDisplay(node, YGDisplayFlex);
		win->refresh();
	}

	void Node::setBg(const Color& color)
	{
		bgColor = color;
		visual.Brush(win->compositor.CreateColorBrush(color.getUIColor()));
	}

	void Node::layout()
	{
		x = YGNodeLayoutGetLeft(node);
		y = YGNodeLayoutGetTop(node);
		w = YGNodeLayoutGetWidth(node);
		h = YGNodeLayoutGetHeight(node);
		visual.Offset({ x, y, 0.0f });
		visual.Size({ w, h });
		if (parent) {
			x = parent->x + x;
			y = parent->y + y;
		}
		syncChrome();
		for (auto& child : children) {
			child->layout();
		}
	}

	void Node::applyDpiChange()
	{
		const float d = win->dpi;
		if (width)  YGNodeStyleSetWidth (node, *width  * d);
		if (height) YGNodeStyleSetHeight(node, *height * d);
		if (margin[0])  YGNodeStyleSetMargin(node, YGEdgeLeft,   *margin[0]  * d);
		if (margin[1])  YGNodeStyleSetMargin(node, YGEdgeTop,    *margin[1]  * d);
		if (margin[2])  YGNodeStyleSetMargin(node, YGEdgeRight,  *margin[2]  * d);
		if (margin[3])  YGNodeStyleSetMargin(node, YGEdgeBottom, *margin[3]  * d);
		if (padding[0]) YGNodeStyleSetPadding(node, YGEdgeLeft,   *padding[0] * d);
		if (padding[1]) YGNodeStyleSetPadding(node, YGEdgeTop,    *padding[1] * d);
		if (padding[2]) YGNodeStyleSetPadding(node, YGEdgeRight,  *padding[2] * d);
		if (padding[3]) YGNodeStyleSetPadding(node, YGEdgeBottom, *padding[3] * d);
		if (edges[0]) YGNodeStyleSetPosition(node, YGEdgeLeft, *edges[0] * win->dpi);
		if (edges[1]) YGNodeStyleSetPosition(node, YGEdgeTop, *edges[1] * win->dpi);
		if (edges[2]) YGNodeStyleSetPosition(node, YGEdgeRight, *edges[2] * win->dpi);
		if (edges[3]) YGNodeStyleSetPosition(node, YGEdgeBottom, *edges[3] * win->dpi);
		onDpiChanged();
		for (auto& child : children) {
			child->applyDpiChange();
		}
	}

	void Node::setFlexGrow(float val)
	{
		YGNodeStyleSetFlexGrow(node, val);
	}

	void Node::setFlexShrink(float val)
	{
		YGNodeStyleSetFlexShrink(node, val);
	}

	void Node::setWidth(float w)
	{
		width = w;
		YGNodeStyleSetWidth(node, w * win->dpi);
	}
	void Node::setHeight(float h)
	{
		height = h;
		YGNodeStyleSetHeight(node, h * win->dpi);
	}

	void Node::setSize(float w, float h)
	{
		setWidth(w);
		setHeight(h);
	}

	void Node::setWidthPercent(float percent)
	{
		// 百分比不随 dpi 变化，直接透传给 yoga
		YGNodeStyleSetWidthPercent(node, percent);
	}

	void Node::setHeightPercent(float percent)
	{
		YGNodeStyleSetHeightPercent(node, percent);
	}

	void Node::setSizePercent(float w, float h)
	{
		YGNodeStyleSetWidthPercent(node, w);
		YGNodeStyleSetHeightPercent(node, h);
	}

	void Node::setMargin(float val)
	{
		margin[0] = val;
		margin[1] = val;
		margin[2] = val;
		margin[3] = val;
		YGNodeStyleSetMargin(node, YGEdgeAll, val * win->dpi);
	}

	void Node::setMargin(float left, float top, float right, float bottom)
	{
		setMarginLeft(left);
		setMarginTop(top);
		setMarginRight(right);
		setMarginBottom(bottom);
	}

	float Node::getMarginLeft()   { return margin[0].value_or(0.f); }
	float Node::getMarginTop()    { return margin[1].value_or(0.f); }
	float Node::getMarginRight()  { return margin[2].value_or(0.f); }
	float Node::getMarginBottom() { return margin[3].value_or(0.f); }

	void Node::setMarginLeft(float val)
	{
		margin[0] = val;
		YGNodeStyleSetMargin(node, YGEdgeLeft, val * win->dpi);
	}

	void Node::setMarginTop(float val)
	{
		margin[1] = val;
		YGNodeStyleSetMargin(node, YGEdgeTop, val * win->dpi);
	}

	void Node::setMarginRight(float val)
	{
		margin[2] = val;
		YGNodeStyleSetMargin(node, YGEdgeRight, val * win->dpi);
	}

	void Node::setMarginBottom(float val)
	{
		margin[3] = val;
		YGNodeStyleSetMargin(node, YGEdgeBottom, val * win->dpi);
	}

	void Node::setPadding(float val)
	{
		padding[0] = val;
		padding[1] = val;
		padding[2] = val;
		padding[3] = val;
		YGNodeStyleSetPadding(node, YGEdgeAll, val * win->dpi);
	}

	void Node::setPadding(float left, float top, float right, float bottom)
	{
		setPaddingLeft(left);
		setPaddingTop(top);
		setPaddingRight(right);
		setPaddingBottom(bottom);
	}

	void Node::setPaddingLeft(float val)
	{
		padding[0] = val;
		YGNodeStyleSetPadding(node, YGEdgeLeft, val * win->dpi);
	}

	void Node::setPaddingTop(float val)
	{
		padding[1] = val;
		YGNodeStyleSetPadding(node, YGEdgeTop, val * win->dpi);
	}

	void Node::setPaddingRight(float val)
	{
		padding[2] = val;
		YGNodeStyleSetPadding(node, YGEdgeRight, val * win->dpi);
	}

	void Node::setPaddingBottom(float val)
	{
		padding[3] = val;
		YGNodeStyleSetPadding(node, YGEdgeBottom, val * win->dpi);
	}

	float Node::getPaddingLeft()   { return padding[0].value_or(0.f); }
	float Node::getPaddingTop()    { return padding[1].value_or(0.f); }
	float Node::getPaddingRight()  { return padding[2].value_or(0.f); }
	float Node::getPaddingBottom() { return padding[3].value_or(0.f); }

	std::tuple<float, float, float, float>  Node::getPadding() {
		return std::make_tuple(
			padding[0].value_or(0.f),
			padding[1].value_or(0.f),
			padding[2].value_or(0.f),
			padding[3].value_or(0.f)
		);
	}
	void Node::setPositionType(const Position val)
	{
		YGNodeStyleSetPositionType(node, (YGPositionType)val);
	}

	void Node::setPosition(Edge edge, float val)
	{
		if (edge == Edge::Left) {
			edges[0] = val;
		}
		else if (edge == Edge::Top) {
			edges[1] = val;
		}
		else if (edge == Edge::Right) {
			edges[2] = val;
		}
		else if (edge == Edge::Bottom) {
			edges[3] = val;
		}
		YGNodeStyleSetPosition(node, (YGEdge)edge, val*win->dpi);
	}

	void Node::setAlignItems(const Align val)
	{
		YGNodeStyleSetAlignItems(node, (YGAlign)val);
	}

	void Node::setFlexWrap(const Wrap val)
	{
		YGNodeStyleSetFlexWrap(node, (YGWrap)val);
	}
	void Node::setJustifyContent(const Justify val)
	{
		YGNodeStyleSetJustifyContent(node, (YGJustify)val);
	}
	void Node::setFlexDirection(const FlexDirection flexDirection)
	{
		YGNodeStyleSetFlexDirection(node, (YGFlexDirection)flexDirection);
	}

	// ---- 圆角 & 边框 ----------------------------------------------------------
	// 设计：
	//  - 圆角用 GeometricClip(RoundedRectangleGeometry) 挂到 visual 上 —— 背景
	//    Brush（setBg 设的 ColorBrush）和子节点会一起被裁剪。
	//  - 边框用 ShapeVisual + SpriteShape 描一圈 stroke。Composition 的 stroke
	//    以几何路径为中线两侧扩散 —— 要让 stroke 外沿贴 (0,0)-(w,h)，几何本身
	//    需要向内 inset borderW/2；对应地，几何的圆角半径也要减去 borderW/2。
	//  - 真正把物理像素写进 Composition 的动作集中在 syncChrome()，每次 layout
	//    以及 setter 触发时调用；DPI 变化后 WinBase 会 relayout，无需额外挂钩。
	void Node::setBorderRadius(float r)
	{
		cornerRadius = r;
		if (r > 0.f && !clipGeo) {
			clipGeo = win->compositor.CreateRoundedRectangleGeometry();
			visual.Clip(win->compositor.CreateGeometricClip(clipGeo));
		}
		else if (r <= 0.f && clipGeo) {
			// 关掉圆角：把 clip 清掉，几何对象也丢，syncChrome 才不会再更新它
			visual.Clip(winrt::Windows::UI::Composition::CompositionClip{ nullptr });
			clipGeo = nullptr;
		}
		syncChrome();
		win->refresh();
	}

	void Node::setBorder(float width, const Color& color)
	{
		// 走 virtual：子类（如 Button）可以在此缓存"正常态"颜色。
		// 顺序：先 setBorderColor（此时 borderShape 可能还没创建，Node 版本只更新成员），
		//      再 setBorderWidth（创建 shape 时会读 borderColor 做首刷）。
		setBorderColor(color);
		setBorderWidth(width);   // 内部会走 syncChrome
	}

	void Node::setBorderWidth(float width)
	{
		borderWidth = width;
		if (width > 0.f && !borderVisual) {
			borderVisual = win->compositor.CreateShapeVisual();
			borderGeo = win->compositor.CreateRoundedRectangleGeometry();
			borderShape = win->compositor.CreateSpriteShape(borderGeo);
			borderShape.StrokeBrush(win->compositor.CreateColorBrush(borderColor.getUIColor()));
			borderVisual.Shapes().Append(borderShape);
			// 顶层：保证边框盖在所有子节点之上
			visual.Children().InsertAtTop(borderVisual);
		}
		else if (width <= 0.f && borderVisual) {
			visual.Children().Remove(borderVisual);
			borderVisual = nullptr;
			borderGeo = nullptr;
			borderShape = nullptr;
		}
		syncChrome();
		win->refresh();
	}

	void Node::setBorderColor(const Color& color)
	{
		borderColor = color;
		if (borderShape) {
			borderShape.StrokeBrush(win->compositor.CreateColorBrush(color.getUIColor()));
			win->refresh();
		}
	}



	void Node::syncChrome()
	{
		if (w <= 0.f || h <= 0.f) return;
		const float d = win->dpi;
		if (clipGeo) {
			// 圆角剪切：clip 相对 visual 自身坐标系 (0,0)-(w,h)
			const float r = std::min({ cornerRadius * d, w * 0.5f, h * 0.5f });
			clipGeo.Size({ w, h });
			clipGeo.CornerRadius({ r, r });
		}

		if (borderVisual) {
			const float bw = borderWidth * d;
			const float inset = bw * 0.5f;
			// stroke 以路径为中线双向扩散，路径内缩 bw/2 才能让外沿贴 (0,0)-(w,h)
			borderVisual.Size({ w, h });
			borderVisual.Offset({ 0.f, 0.f, 0.f });
			// Geometry 无 Offset 属性，把位移写在 Shape 上（CompositionShape.Offset）
			borderShape.Offset({ inset, inset });
			borderGeo.Size({ std::max(0.f, w - bw), std::max(0.f, h - bw) });
			// 圆角边框：外圆角 = cornerRadius，描边中线半径 = outerR - bw/2
			const float outerR = std::min({ cornerRadius * d, w * 0.5f, h * 0.5f });
			const float midR = std::max(0.f, outerR - inset);
			borderGeo.CornerRadius({ midR, midR });
			borderShape.StrokeThickness(bw);
		}
	}
	std::weak_ptr<bool> Node::getWeakThis()
	{
		return std::weak_ptr<bool>(alive);
	}
}
