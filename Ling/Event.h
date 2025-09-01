#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include "MouseEvent.h"
class Event
{
public:
	size_t onMouseEnter(std::function<void(const MouseEvent&)> callback);
	size_t onMouseLeave(std::function<void(const MouseEvent&)> callback);
	size_t onMouseMove(std::function<void(const MouseEvent&)> callback);
	size_t onMouseDown(std::function<void(const MouseEvent&)> callback);
	size_t onMouseUp(std::function<void(const MouseEvent&)> callback);
	size_t onShown(std::function<void()> callback);

	void offMouseEnter(const size_t& callbackId);
	void offMouseLeave(const size_t& callbackId);
	void offMouseMove(const size_t& callbackId);
	void offMouseDown(const size_t& callbackId);
	void offMouseUp(const size_t& callbackId);
	void offShown(const size_t& callbackId);

	virtual void mouseEnter(const MouseEvent& event);
	virtual void mouseLeave(const MouseEvent& event);
	virtual void mouseMove(const MouseEvent& event);
	virtual void mouseDown(const MouseEvent& event);
	virtual void mouseUp(const MouseEvent& event);
	virtual void shown();
protected:

private:
	size_t mouseMoveCBId{ 0 };
	std::unordered_map<size_t, std::function<void(const MouseEvent&)>> mouseMoveCBs;
	size_t mouseDownCBId{ 0 };
	std::unordered_map<size_t, std::function<void(const MouseEvent&)>> mouseDownCBs;
	size_t mouseUpCBId{ 0 };
	std::unordered_map<size_t, std::function<void(const MouseEvent&)>> mouseUpCBs;
	size_t mouseEnterCBId{ 0 };
	std::unordered_map<size_t,std::function<void(const MouseEvent&)>> mouseEnterCBs;
	size_t mouseLeaveCBId{ 0 };
	std::unordered_map<size_t,std::function<void(const MouseEvent&)>> mouseLeaveCBs;
	size_t shownCBId{ 0 };
	std::unordered_map<size_t, std::function<void()>> shownCBs;

	bool isMouseEnter{ false };
};

