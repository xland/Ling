#pragma once
#include <vector>
#include "../Include/Position.h"
#include "../Include/Size.h"

namespace Ling {
	struct LineSizeInfo
	{
		Position pos;
		size_t startIndex;
		size_t length;
	};
}