#pragma once
#include "Color.h"
#include "ElementBox.h"
#include "Wrap.h"
namespace tvg {
	class Scene;
}
namespace Ling {
	class EffectBox :public ElementBox
	{
		public:
			EffectBox();
			~EffectBox();
			/// <summary>
			/// 添加了新元素之后，应重新执行布局
			/// </summary>
			/// <param name="index"></param>
			/// <param name="ele"></param>
			virtual void insertChild(const int& index, const std::shared_ptr<Element>& ele);
			/// <summary>
			/// 添加了新元素之后，应重新执行布局
			/// </summary>
			/// <param name="ele"></param>
			virtual void addChild(const std::shared_ptr<Element>& ele);
			void setWindow(WindowBase* win) override;
			void setShadow(const Color& color,const float& angle,const float& distance,const float& sigma);
			void setBlur();
		private:
			tvg::Scene* scene;
	};
}
