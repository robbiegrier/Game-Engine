#include "Rect.h"

namespace Azul
{
	Rect::Rect()
		: x(0.0f), y(0.0f), width(0.0f), height(0.0f)
	{
	}

	Rect::Rect(float inX, float inY, float inWidth, float inHeight)
		: x(inX), y(inY), width(inWidth), height(inHeight)
	{
	}

	void Rect::Clear()
	{
		x = 0.0f;
		y = 0.0f;
		height = 0.0f;
		width = 0.0f;
	}
}