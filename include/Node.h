#pragma once
#include <tuple>
#include <winrt/Windows.UI.Composition.h>
#include "Color.h"
namespace Ling {
	class WinBase;
	class Node
	{
		friend class WinBase;
		public:
			virtual ~Node();
			bool isPosIn(float x,float y);
			void setPosSize(const float& x, const float& y, const float& w, const float& h);
			std::tuple<float, float, float, float> getPosSize();
			void hide();
			void show();
			void setBg(const Color& color);
		public:
			WinBase* win;
			winrt::Windows::UI::Composition::SpriteVisual visual{ nullptr };
		private:
			Node(WinBase* win);
	};
}