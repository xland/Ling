#pragma once
#include <winrt/Windows.Foundation.h>
#include <yoga/Yoga.h>
#include <variant>
#include <any>
#include "Color.h"
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"
#include "PropertyType.h"
namespace Ling {
	class IElement;
	class Property
	{
	public:
		Property();
		virtual ~Property();	
		void set(const PropertyType& type, const std::variant<float, int, bool, std::wstring>& value);
		const float& getFloat(const std::wstring& name);
		const int& getInt(const std::wstring& name);
		const bool& getBool(const std::wstring& name);
		const std::wstring& getStr(const std::wstring& name);
	private:
		void notifyChange(const std::wstring& name, const std::variant<float, int, bool, std::wstring>& value);
	private:
		std::unordered_map<PropertyType, float> dataFloat;
		std::unordered_map<PropertyType, int> dataInt;
		std::unordered_map<PropertyType, bool> dataBool;
		std::unordered_map<PropertyType, std::wstring> dataStr;
		std::unordered_map<std::wstring, std::any> dataCustom;
		std::vector<IElement*> elements;
	};
}

