#include "Glyph.h"

namespace Azul
{
	Glyph::Glyph(Rect inRect)
		: rect(inRect)
	{
	}

	const Rect& Glyph::GetRect() const
	{
		return rect;
	}
}