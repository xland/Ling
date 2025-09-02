#pragma once
#include <string>
#include "Element.h"

namespace Ling {
	class TextBlock :public Element
	{
		public:
			TextBlock();
			~TextBlock();
			void paint(SkCanvas* canvas) override;
			void setText(const std::wstring& text);
		protected:

		private:
			std::wstring text;
	};
}

