#pragma once
#include <string>
#include <functional>
#include "Event.h"
namespace Ling {
	class EventBase
	{
	public:
		EventBase();
		virtual ~EventBase();
		size_t on(const Event& eventName, std::function<void(void*)> cb);
		void off(const Event& eventName, const size_t& id);
		void emit(const Event& eventName, void* arg);
	public:
	private:
		struct Slot
		{
			size_t id;
			std::function<void(void*)> cb;
		};
		std::unordered_map<Event, std::vector<Slot>> events;
		size_t nextId{ 0 };
	};
}