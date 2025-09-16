#include <thorvg.h>
#include <yoga/Yoga.h>
#include "../Include/WindowBase.h"
#include "../Include/EffectBox.h"

namespace Ling {
	EffectBox::EffectBox() :scene{ tvg::Scene::gen() } 
	{
		scene->push(shape);
	}

	EffectBox::~EffectBox() 
	{

	}

	void EffectBox::addChild(const std::shared_ptr<Element>& ele)
	{
		ElementBox::addChild(ele);
		scene->push(ele->shape);
	}

	void EffectBox::insertChild(const int& index, const std::shared_ptr<Element>& ele)
	{
		ElementBox::insertChild(index, ele);
		scene->push(ele->shape);
	}

	void EffectBox::setWindow(WindowBase* win)
	{
		auto result = win->scene->push(scene);
		this->win = win;
	}

	void EffectBox::setShadow(const Color& color, const float& angle, const float& distance, const float& sigma)
	{
		scene->push(tvg::SceneEffect::DropShadow,
			color.getR(), color.getG(), color.getB(), color.getA(),
			angle, distance, sigma, 100);
	}
	void EffectBox::setBlur()
	{
	}
}