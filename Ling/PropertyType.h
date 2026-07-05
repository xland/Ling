#pragma once
namespace Ling {
	enum class PropertyType
	{
		Cursor,
		Visible,

		Width,
		Height,
		WidthPercent,
		HeightPercent,

		Position,
		Left,
		Right,
		Top,
		Bottom,

		MarginLeft,
		MarginTop,
		MarginRight,
		MarginBottom,

		PaddingLeft,
		PaddingTop,
		PaddingRight,
		PaddingBottom,

		BorderWidth,
		BorderColor,
		BorderRadius,
		BorderStyle,

		FlexGrow,
		FlexShrink,
		Wrap,
		Align,
		Justify,
		FlexDirection,

		BackgroundColor,
		BackgroundHoverColor,
		BackgroundPressColor,
		ForegroundColor,
		ForegroundHoverColor,
		ForegroundPressColor,

		Text,
		TextDecoration,
		TextAlign,
		FontSize,
		FontFamily,
		FontWeight,
		FontStyle
	};
}
