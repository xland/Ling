#pragma once
#include <include/core/SkImageInfo.h>
#include <include/core/SkSurface.h>
#include <include/core/SkColorSpace.h>

class Element;
class WindowBase;
class WindowBaseImpl
{
public:
	WindowBaseImpl(WindowBase* win);
	~WindowBaseImpl();
	void reset();
	void paintElement(Element* ele);
	const SkPixmap getPix();
private:
private:
	sk_sp<SkSurface> surface;
	WindowBase* win;
};

