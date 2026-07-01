#pragma once
#include <winrt/Windows.Foundation.h>
#include <yoga/Yoga.h>
#include "Color.h"
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"
#include <variant>
namespace Ling {
	using namespace winrt::Windows::Foundation;
	class Property
	{
	public:
		Property();
		virtual ~Property();
		winrt::event_token onChangeFloat(winrt::delegate<void(const std::wstring&, const float&)> const& handler);
		winrt::event_token onChangeInt(winrt::delegate<void(const std::wstring&, const int&)> const& handler);
		winrt::event_token onChangeBool(winrt::delegate<void(const std::wstring&, const bool&)> const& handler);
		winrt::event_token onChangeStr(winrt::delegate<void(const std::wstring&, const std::wstring&)> const& handler);
		void offChangeFloat(winrt::event_token const& token);
		void offChangeInt(winrt::event_token const& token);
		void offChangeBool(winrt::event_token const& token);
		void offChangeStr(winrt::event_token const& token);		
		void set(const std::wstring& name, const float& val);
		void set(const std::wstring& name, const int& val);
		void set(const std::wstring& name, const bool val);
		void set(const std::wstring& name, const std::wstring& val);
		void setMargin(const float& val);
		void setMargin(const float& left, const float& top, const float& right, const float& bottom);
		void setPadding(const float& val);
		void setPadding(const float& left, const float& top, const float& right, const float& bottom);
		const float& getFloat(const std::wstring& name);
		const int& getInt(const std::wstring& name);
		const bool& getBool(const std::wstring& name);
		const std::wstring& getStr(const std::wstring& name);
	private:
		void notifyChange(const std::wstring& name);
	private:
		winrt::event<winrt::delegate<void(const std::wstring&, const float&)>> eventsFloat;
		winrt::event<winrt::delegate<void(const std::wstring&, const int&)>> eventsInt;
		winrt::event<winrt::delegate<void(const std::wstring&, const bool&)>> eventsBool;
		winrt::event<winrt::delegate<void(const std::wstring&, const std::wstring&)>> eventsStr;
		std::unordered_map<std::wstring, float> dataFloat;
		std::unordered_map<std::wstring, int> dataInt;
		std::unordered_map<std::wstring, bool> dataBool;
		std::unordered_map<std::wstring, std::wstring> dataStr;
	};
}

