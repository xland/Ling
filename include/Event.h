#pragma once
#include <string>
#include <functional>
#include "EventType.h"
namespace Ling {
	class Event
	{
	public:
		Event();
		virtual ~Event();
		size_t on(const EventType& eventName, std::function<void(void*)> cb);
		void off(const EventType& eventName, const size_t& id);
		void emit(const EventType& eventName, void* arg);
	public:
	private:
		struct Slot
		{
			size_t id;
			std::function<void(void*)> cb;
		};
		std::unordered_map<EventType, std::vector<Slot>> events;
		size_t nextId{ 0 };
	};
}