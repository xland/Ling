#pragma once
#include <memory>
#include "Element.h"

struct YGSize;
struct YGNode;
enum YGMeasureMode;
typedef const struct YGNode* YGNodeConstRef;
namespace tvg {
	class Picture;
}
namespace Ling {
	class Image :public Element
	{
	public:
		Image();
		~Image();
		void layout() override;
		void setWindow(WindowBase* win) override;
		const std::wstring& getSrc();
		void setSrc(const std::wstring& src);
		void setSize(const float& w, const float& h) override;
	private:
		static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
	private:
		tvg::Picture* imgShape;
		std::wstring src;
	};
}
