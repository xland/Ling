#pragma once
#include "Ling.h"

namespace Ling {
	class WinBase;
	class Button :public Node, public EventBase
	{
	public:
		Button(WinBase* win);
		~Button();
		void setText(const std::wstring& text);
		void setFontSize(float val);
		void setFontFamily(const std::wstring& val);
		void setColor(Color color);
		void setHoverColor(Color color);
		void setHoverBg(Color color);
	private:
		void onMove(void* e);
		void onDown(void* e);
	private:
		NodeText* label;
		size_t onMoveId, onDownId;
		Color hoverColor, hoverBg;
		bool isHover;
	};
}




