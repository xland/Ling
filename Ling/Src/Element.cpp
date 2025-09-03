#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>
#include <include/core/SkRRect.h>

#include "../Include/Element.h"
#include "../Include/WindowBase.h"
#include "../Include/App.h"
namespace Ling {
	Element::Element() 
	{
	}

	Element::~Element()
	{
		
	}
	WindowBase* Element::getWindow()
	{
		return win;
	}
	void Element::update()
	{
		int left = (int)getGlobalX();
		int top = (int)getGlobalY();
		int right = (int)getWidth() + left;
		int bottom = (int)getHeight() + top;
		RECT r{ .left{left},.top{top}, .right{right}, .bottom{bottom} };
		auto hwnd = getWindow()->getHandle();
		InvalidateRect(hwnd, &r, false);
	}

	void Element::setWindow(WindowBase* win)
	{
		this->win = win;
	}

	void Element::setParent(Element* ele)
	{
		this->parent = ele;
	}

	Element* Element::getParent()
	{
		return parent;
	}

	void Element::paint(SkCanvas* canvas)
	{
		float x = getLeft();
		float y = getTop();
		float w = getWidth();
		float h = getHeight();
		SkRect rect = SkRect::MakeXYWH(x, y, w, h);
		paintStyle(canvas, rect);
	}

	void Element::setBorderWidth(const float& borderWidth)
	{
		this->borderWidth = borderWidth;
	}
	float Element::getBorderWidth()
	{
		return borderWidth;
	}
	void Element::setRadius(const float& radius)
	{
		radiusLT = radius;
		radiusRT = radius;
		radiusRB = radius;
		radiusLB = radius;
	}
	void Element::setRadius(const float& radiusLT, const float& radiusRT, const float& radiusRB, const float& radiusLB)
	{
		this->radiusLT = radiusLT;
		this->radiusRT = radiusRT;
		this->radiusRB = radiusRB;
		this->radiusLB = radiusLB;
	}
	std::array<float, 4> Element::getRadius()
	{
		return std::array<float, 4>{radiusLT, radiusRT, radiusRB, radiusLB};
	}
	float Element::getRadiusLT()
	{
		return radiusLT;
	}
	float Element::getRadiusRT()
	{
		return radiusRT;
	}
	float Element::getRadiusRB()
	{
		return radiusRB;
	}
	float Element::getRadiusLB()
	{
		return radiusLB;
	}
	void Element::setBackgroundColor(const Color& backgroundColor)
	{
		this->backgroundColor = backgroundColor;
	}
	void Element::setBorderColor(const Color& borderColor)
	{
		this->borderColor = borderColor;
	}
	Color& Element::getBackgroundColor()
	{
		return backgroundColor;
	}
	Color& Element::getBorderColor()
	{
		return borderColor;
	}
	void Element::setCaptionFlag(bool captionFlag)
	{
		this->captionFlag = captionFlag;
	}
	bool Element::getCaptionFlag()
	{
		return captionFlag;
	}

	void Element::paintStyle(SkCanvas* canvas, SkRect& rect)
	{
		if (backgroundColor != 0) { //绘制背景
			SkPaint paint;
			paint.setColor(backgroundColor);
			paint.setStyle(SkPaint::kFill_Style);
			paintRect(canvas, paint, rect);
		}
		if (borderColor != 0 && borderWidth > 0) { //绘制边框
			SkPaint paint;
			paint.setAntiAlias(true);
			paint.setColor(borderColor);
			paint.setStrokeWidth(borderWidth);
			paint.setStyle(SkPaint::kStroke_Style);
			paintRect(canvas, paint, rect);
		}
	}
	void Element::paintRect(SkCanvas* canvas, const SkPaint& paint, const SkRect& rect)
	{
		if (radiusLT > 0 || radiusRT > 0 || radiusRB > 0 || radiusLB > 0) {
			SkVector radii[4]{ {radiusLT, radiusLT}, {radiusRT, radiusRT}, {radiusRB, radiusRB}, {radiusLB, radiusLB} };
			SkRRect rr;
			rr.setRectRadii(rect, radii);
			canvas->drawRRect(rr, paint);
		}
		else {
			canvas->drawRect(rect, paint);
		}
	}
}


