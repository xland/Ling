#include "Event.h"


size_t Event::onMouseMove(std::function<void(const MouseEvent&)> callback)
{
	mouseMoveCBId += 1;
	mouseMoveCBs.insert({ mouseMoveCBId,callback });
	return mouseMoveCBId;
}
size_t Event::onMouseDown(std::function<void(const MouseEvent&)> callback)
{
	mouseDownCBId += 1;
	mouseDownCBs.insert({ mouseDownCBId,callback });
	return mouseDownCBId;
}
size_t Event::onMouseUp(std::function<void(const MouseEvent&)> callback)
{
	mouseDownCBId += 1;
	mouseUpCBs.insert({ mouseUpCBId,callback });
	return mouseDownCBId;
}

size_t Event::onMouseEnter(std::function<void(const MouseEvent&)> callback)
{
	mouseEnterCBId += 1;
	mouseEnterCBs.insert({ mouseEnterCBId,callback });
	return mouseEnterCBId;
}

size_t Event::onMouseLeave(std::function<void(const MouseEvent&)> callback)
{
	mouseLeaveCBId += 1;
	mouseLeaveCBs.insert({ mouseLeaveCBId,callback });
	return mouseLeaveCBId;
}

size_t Event::onShown(std::function<void()> callback)
{
	shownCBId += 1;
	shownCBs.insert({ mouseLeaveCBId,callback });
	return mouseLeaveCBId;
}

void Event::offMouseEnter(const size_t& callbackId)
{
	mouseEnterCBs.erase(callbackId);
}
void Event::offMouseLeave(const size_t& callbackId)
{
	mouseLeaveCBs.erase(callbackId);
}
void Event::offMouseMove(const size_t& callbackId)
{
	mouseMoveCBs.erase(callbackId);
}
void Event::offMouseDown(const size_t& callbackId)
{
	mouseDownCBs.erase(callbackId);
}
void Event::offMouseUp(const size_t& callbackId)
{
	mouseUpCBs.erase(callbackId);
}
void Event::offShown(const size_t& callbackId)
{
	shownCBs.erase(callbackId);
}

void Event::mouseEnter(const MouseEvent& event)
{
	for (const auto& pair : mouseEnterCBs) {
		pair.second(event);
	}
}

void Event::mouseLeave(const MouseEvent& event)
{
	for (const auto& pair : mouseLeaveCBs) {
		pair.second(event);
	}
}

void Event::mouseMove(const MouseEvent& event)
{
	for (const auto& pair : mouseMoveCBs) {
		pair.second(event);
	}
}

void Event::mouseDown(const MouseEvent& event)
{
	for (const auto& pair : mouseDownCBs) {
		pair.second(event);
	}
}

void Event::mouseUp(const MouseEvent& event)
{
	for (const auto& pair : mouseUpCBs) {
		pair.second(event);
	}
}

void Event::shown()
{
	for (const auto& pair : shownCBs) {
		pair.second();
	}
}

