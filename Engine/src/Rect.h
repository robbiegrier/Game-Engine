#ifndef _Rect
#define _Rect

namespace Azul
{
	struct Rect
	{
		Rect();
		Rect(const Rect&) = default;
		Rect& operator = (const Rect&) = default;
		~Rect() = default;

		Rect(float x, float y, float width, float height);

		void Clear();

		float x;
		float y;
		float width;
		float height;
	};
}

#endif
