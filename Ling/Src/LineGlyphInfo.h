#pragma once
#include <vector>
#include "../Include/Position.h"

struct SkPoint;
typedef uint16_t SkGlyphID;
namespace Ling {
	struct LineGlyphInfo
	{
		std::vector<SkGlyphID> glyphs;
		std::vector<SkPoint> wordPos;
		Position pos;
	};
}

