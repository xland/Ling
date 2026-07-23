#include "pch.h"
#include "../include/Event.h"
#include "../include/NodeText.h"
#include "../include/WinBase.h"
#include "../include/D2D.h"

namespace Ling {

	NodeText::NodeText(Node* parent, const std::wstring& text) :Node(parent)
	{
		auto d2d = D2D::get();
		surface = d2d->createDrawingSurface(win->compositor);
		Composition::CompositionSurfaceBrush brush = win->compositor.CreateSurfaceBrush(surface);
		visual.Brush(brush);
		textLayout = d2d->createTextLayout(win->title, FLT_MAX, FLT_MAX);
	}

	NodeText::~NodeText()
	{
	}

	void NodeText::onDown(void* e)
	{
	}

	void NodeText::onUp(void* e)
	{
	}

	void NodeText::onMove(void* e)
	{
	}

	void NodeText::setAutoSize(float paddingLeftRight, float paddingTopBottom)
	{
		DWRITE_TEXT_METRICS metrics;
		textLayout->GetMetrics(&metrics);
		auto w = metrics.width + 2 * paddingLeftRight;
		auto h = metrics.height + 2 * paddingTopBottom;
		setSize(w, h);
	}

}