#include "pch.h"
#include "Screen.h"

namespace Ling{
    std::tuple<int, int, int, int> Screen::getMainScreenPos()
    {
        auto monitor = MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);
        MONITORINFO mi{ sizeof(MONITORINFO) };
        GetMonitorInfo(monitor, &mi);
        auto& workArea = mi.rcWork;
        return std::make_tuple(workArea.left, workArea.top, workArea.right, workArea.bottom);
    }
}

