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

		}


		//if (dataFloat.contains(name)) {
		//	if (dataFloat[name] != val) {
		//		dataFloat[name] = val;
		//		notifyChange(name, val);
		//	}
		//}
		//else {
		//	dataFloat.insert({ name,val });
		//	notifyChange(name, val);
		//}
	}

	void Property::notifyChange(const std::wstring& name, const std::variant<float, int, bool, std::wstring>& value)
	{
		for (auto& ele:elements)
		{
			ele->changeProperty(name, value);
		}
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
