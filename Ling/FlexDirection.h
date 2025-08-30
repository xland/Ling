#pragma once
#include <limits>

namespace Ling{
    constexpr float SizeNaN = std::numeric_limits<float>::quiet_NaN();

    enum class FlexDirection
    {
        Column,
        ColumnReverse,
        Row,
        RowReverse
    };
}