#include "pch.h"
#include "../include/EventBase.h"
#include "../include/Event.h"

namespace Ling {
	EventBase::EventBase()
	{
	}

	EventBase::~EventBase()
	{
	}

	size_t EventBase::on(const Event& eventName, std::function<void(void*)> cb)
	{
		// 每个 Event 实例自己维护计数器，避免跨实例冲突
		auto id = ++nextId;
		events[eventName].push_back({ id, std::move(cb) });
		return id;
	}

	void EventBase::off(const Event& eventName, const size_t& id)
	{
		auto it = events.find(eventName);
		if (it == events.end()) return;
		auto& vec = it->second;		
		// 按 id 摘除；用 erase-remove_if 一次遍历完成
		vec.erase(std::remove_if(vec.begin(), vec.end(), [id](const Slot& s) { return s.id == id; }), vec.end() );
		// 该事件没剩下的回调，把 key 也删了，避免 map 累积空 vector
		if (vec.empty()) events.erase(it);
	}

	void EventBase::emit(const Event& eventName, void* arg)
	{
		auto it = events.find(eventName);
		if (it == events.end()) return;
		// 禁止在回调内 on/off 自身
		auto& slots = it->second;
		for (const auto& s : slots) {
			s.cb(arg);
		}
	}

}
