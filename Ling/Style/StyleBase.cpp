#include "pch.h"
#include "StyleBase.h"
namespace Ling {
	StyleBase::StyleBase()
	{
	}

	StyleBase::~StyleBase()
	{
	}
	auto StyleBase::onChange(EventHandler const& handler)
	{
		return events.add(handler);
	}
	void StyleBase::offChange(winrt::event_token const& token)
	{
		events.remove(token);
	}
	void StyleBase::notifyChange()
	{
		events(this);
	}

	// get 方法
	float StyleBase::getLeft()
	{
		return left;
	}

	float StyleBase::getTop()
	{
		return top;
	}

	float StyleBase::getWidth()
	{
		return width;
	}

	float StyleBase::getHeight()
	{
		return height;
	}

	float StyleBase::getMarginLeft()
	{
		return marginLeft;
	}

	float StyleBase::getMarginTop()
	{
		return marginTop;
	}

	float StyleBase::getMarginRight()
	{
		return marginRight;
	}

	float StyleBase::getMarginBottom()
	{
		return marginBottom;
	}

	float StyleBase::getPaddingLeft()
	{
		return paddingLeft;
	}

	float StyleBase::getPaddingTop()
	{
		return paddingTop;
	}

	float StyleBase::getPaddingRight()
	{
		return paddingRight;
	}

	float StyleBase::getPaddingBottom()
	{
		return paddingBottom;
	}
}
