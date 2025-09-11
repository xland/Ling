#include <yoga/Yoga.h>
#include "../Include/Node.h"

namespace Ling {
	Node::Node() : node{ YGNodeNew() }//YGNodeNew()
	{
		//YGConfigRef config = YGConfigNew();
		//YGConfigSetPointScaleFactor(config, 1.5f);
		//node = YGNodeNewWithConfig(config);
	}

	Node::~Node()
	{
		YGNodeFreeRecursive(node);
	}


	void Node::setFlexGrow(const float& val)
	{
		YGNodeStyleSetFlexGrow(node, val);
	}

	void Node::setFlexShrink(const float& val)
	{
		YGNodeStyleSetFlexShrink(node, val);
	}

	void Node::setSize(const float& w, const float& h)
	{
		YGNodeStyleSetWidth(node, w);
		YGNodeStyleSetHeight(node, h);
	}
	float Node::getLeft()
	{
		return YGNodeLayoutGetLeft(node);
	}
	float Node::getTop()
	{
		return YGNodeLayoutGetTop(node);
	}
	float Node::getWidth()
	{
		return YGNodeLayoutGetWidth(node);
	}
	float Node::getHeight()
	{
		return YGNodeLayoutGetHeight(node);
	}
	float Node::getGlobalX()
	{
		return globalX;
	}
	float Node::getGlobalY()
	{
		return globalY;
	}
	void Node::setWidth(const float& w)
	{
		YGNodeStyleSetWidth(node, w);
	}
	void Node::setHeight(const float& h)
	{
		YGNodeStyleSetHeight(node, h);
	}
	void Node::setWidthPercent(const float& percent)
	{
		YGNodeStyleSetWidthPercent(node, percent);
	}
	void Node::setHeightPercent(const float& percent)
	{
		YGNodeStyleSetHeightPercent(node, percent);
	}
	void Node::setSizePercent(const float& w, const float& h)
	{
		YGNodeStyleSetWidthPercent(node, w);
		YGNodeStyleSetHeightPercent(node, h);
	}

	void Node::setMargin(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdgeAll, val);
	}

	void Node::setMargin(const float& left, const float& top, const float& right, const float& bottom)
	{
		YGNodeStyleSetMargin(node, YGEdgeLeft, left);
		YGNodeStyleSetMargin(node, YGEdgeTop, top);
		YGNodeStyleSetMargin(node, YGEdgeRight, right);
		YGNodeStyleSetMargin(node, YGEdgeBottom, bottom);
	}
	float Node::getMarginLeft()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeLeft).value;
	}
	float Node::getMarginTop()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeTop).value;
	}
	float Node::getMarginRight()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeRight).value;
	}
	float Node::getMarginBottom()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeBottom).value;
	}
	void Node::setMarginLeft(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeLeft, val);
	}
	void Node::setMarginTop(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeTop, val);
	}
	void Node::setMarginRight(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeRight, val);
	}
	void Node::setMarginBottom(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeBottom, val);
	}
	

	void Node::insertChild(Node* tar, const int& index)
	{
		YGNodeInsertChild(node, tar->node, index);
	}

	void Node::addChild(Node* tar)
	{
		YGNodeInsertChild(node, tar->node, YGNodeGetChildCount(node));
	}


	Position Node::getPosition()
	{
		int x = (int)YGNodeLayoutGetLeft(node);
		int y = (int)YGNodeLayoutGetTop(node);
		return Position(x, y);
	}

	Size Node::getSize()
	{
		int w = (int)YGNodeLayoutGetWidth(node);
		int h = (int)YGNodeLayoutGetHeight(node);
		return Size(w, h);
	}

	bool Node::hittest(const int& x, const int& y)
	{
		float right = YGNodeLayoutGetWidth(node) + globalX;
		float bottom = YGNodeLayoutGetHeight(node) + globalY;
		if (x > globalX && y > globalY && x < right && y < bottom)
		{
			return true;
		}
		return false;
	}
}