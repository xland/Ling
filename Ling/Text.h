#pragma once
#include <string>
#include <wrl.h>
#include <dwrite_3.h>
#include <yoga/Yoga.h>
#include <winrt/Windows.UI.Composition.h>
#include "Element.h"
namespace Ling{
	using namespace Microsoft::WRL;
	class WindowBase;
	class Text : public Element
	{
	public:
		Text(WindowBase* win);
		~Text();
	protected:
		void propertyChanged(const Ling::PropertyType& key, const void* value) override;
		void layout() override;
	private:
		static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
		void paint();
	private:
		ComPtr<IDWriteTextLayout> textLayout;
		Composition::CompositionSurfaceBrush brush{nullptr};
		Composition::CompositionDrawingSurface surface{ nullptr };
	};
}

