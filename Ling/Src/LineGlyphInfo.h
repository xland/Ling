#pragma once
#include <vector>
#include "../Include/Position.h"

namespace Ling {
	struct LineGlyphInfo
	{
		std::vector<uint16_t> glyphs;
		std::vector<Position> wordPos;
		Position pos;
		size_t startIndex;
		size_t length;
	};
}

