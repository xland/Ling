#pragma once
#include <thorvg.h>


namespace Ling {
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
}


