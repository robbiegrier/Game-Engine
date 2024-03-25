#ifndef _Glyph
#define _Glyph

#include "Rect.h"

namespace Azul
{
	class Glyph
	{
	public:
		Glyph() = delete;
		explicit Glyph(Rect inRect);
		Glyph(const Glyph&) = delete;
		Glyph& operator=(const Glyph&) = delete;
		~Glyph() = default;

		const Rect& GetRect() const;

	private:
		Rect rect;
	};
}

#endif