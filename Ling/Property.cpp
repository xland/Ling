#include "pch.h"
#include "Property.h"
namespace Ling {
	Property::Property()
	{
	}

	Property::~Property()
	{
	}

	winrt::event_token Property::onChangeFloat(winrt::delegate<void(const std::wstring&, const float&)> const& handler)
	{
		return eventsFloat.add(handler);
	}


	winrt::event_token Property::onChangeInt(winrt::delegate<void(const std::wstring&, const int&)> const& handler)
	{
		return eventsInt.add(handler);
	}
	
	winrt::event_token Property::onChangeBool(winrt::delegate<void(const std::wstring&, const bool&)> const& handler)
	{
		return eventsBool.add(handler);
	}

	winrt::event_token Property::onChangeStr(winrt::delegate<void(const std::wstring&, const std::wstring&)> const& handler)
	{
		return eventsStr.add(handler);
	}


	void Property::offChangeFloat(winrt::event_token const& token)
	{
		eventsFloat.remove(token);
	}

	void Property::offChangeInt(winrt::event_token const& token)
	{
		eventsInt.remove(token);
	}

	void Property::offChangeBool(winrt::event_token const& token)
	{
		eventsBool.remove(token);
	}

	void Property::offChangeStr(winrt::event_token const& token)
	{
		eventsStr.remove(token);
	}



	void Property::set(const std::wstring& name, const float& val)
	{
		if (dataFloat.contains(name)) {
			if (dataFloat[name] != val) {
				dataFloat[name] = val;
				eventsFloat(name, val);
			}
		}
		else {
			dataFloat.insert({ name,val });
			eventsFloat(name, val);
		}
	}

	void Property::set(const std::wstring& name, const int& val)
	{
		if (dataInt.contains(name)) {
			if (dataInt[name] != val) {
				dataInt[name] = val;
				eventsInt(name, val);
			}
		}
		else {
			dataInt.insert({ name,val });
			eventsInt(name, val);
		}
	}

	void Property::set(const std::wstring& name,const bool val)
	{
		if (dataBool.contains(name)) {
			if (dataBool[name] != val) {
				dataBool[name] = val;
				eventsBool(name, val);
			}
		}
		else {
			dataBool.insert({ name,val });
			eventsBool(name, val);
		}
	}
	void Property::set(const std::wstring& name, const std::wstring& val)
	{
		if (dataStr.contains(name)) {
			if (dataStr[name] != val) {
				dataStr[name] = val;
				eventsStr(name, val);
			}
		}
		else {
			dataStr.insert({ name,val });
			eventsStr(name, val);
		}
	}
	void Property::setMargin(const float& val)
	{
		setMargin(val, val, val, val);
	}
	void Property::setMargin(const float& left, const float& top, const float& right, const float& bottom)
	{
		set(L"margin-left", left);
		set(L"margin-top", top);
		set(L"margin-right", right);
		set(L"margin-bottom", bottom);
	}
	void Property::setPadding(const float& val)
	{
		setPadding(val, val, val, val);
	}
	void Property::setPadding(const float& left, const float& top, const float& right, const float& bottom)
	{
		set(L"padding-left", left);
		set(L"padding-top", top);
		set(L"padding-right", right);
		set(L"padding-bottom", bottom);
	}
}
