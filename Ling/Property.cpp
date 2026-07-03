#include "pch.h"
#include "Property.h"
#include "IElement.h"
namespace Ling {
	Property::Property()
	{
	}

	Property::~Property()
	{
	}

	void Property::set(const PropertyType& type, const std::variant<float, int, bool, std::wstring>& value)
	{
		if (type == PropertyType::Width) {
			auto& val = std::get<float>(value);
			if (setFloat(type, val)) {
				for (auto& ele: elements)
				{
					//ele.setWidth(std::get<float>(value));
				}
			}
		}
	}

	void Property::notifyChange(const std::wstring& name, const std::variant<float, int, bool, std::wstring>& value)
	{
		for (auto& ele:elements)
		{
			ele->changeProperty(name, value);
		}
	}
	bool Property::setFloat(const PropertyType& type, const float& value)
	{
		if (dataFloat.contains(type)) {
			if (dataFloat[type] != value) {
				dataFloat[type] = value;
				return true;
			}
		}
		else {
			dataFloat.insert({ type,value });
			return true;
		}
		return false;
	}
	const float& Property::getFloat(const std::wstring& name)
	{
		// TODO: 在此处插入 return 语句
	}
	const int& Property::getInt(const std::wstring& name)
	{
		// TODO: 在此处插入 return 语句
	}
	const bool& Property::getBool(const std::wstring& name)
	{
		// TODO: 在此处插入 return 语句
	}
	const std::wstring& Property::getStr(const std::wstring& name)
	{
		// TODO: 在此处插入 return 语句
	}
}
