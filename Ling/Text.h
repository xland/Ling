#pragma once
#include "Header.h"
#include "Element.h"
namespace Ling{
	class Text : public Element
	{
	public:
		Text();
		~Text();
		void setText(const std::wstring text);
	protected:
		void initProperty(const Composition::Compositor& comp) override;
		void layout() override;
	private:
		/// <summary>
		/// 如果你在 YGNodeStyleSetWidth(node, 100) / YGNodeStyleSetHeight(node, 50) 里已经指定了固定大小，Yoga 就直接用这个值，不会去调用 measureFunc。  
		/// 如果父容器已经约束住了大小，比如 flex:1 填充满了，Yoga 也不会再问 measureFunc。  
		/// </summary>
		/// <param name="node"></param>
		/// <param name="width"></param>
		/// <param name="widthMode"></param>
		/// <param name="height"></param>
		/// <param name="heightMode"></param>
		/// <returns></returns>
		static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
	private:
		ComPtr<IDWriteTextLayout> textLayout;
		std::wstring text;
		Composition::CompositionSurfaceBrush brush{nullptr};
		Composition::CompositionDrawingSurface surface{ nullptr };
	};
}

