#ifndef _GONull
#define _GONull

#include "GraphicsObject.h"

namespace Azul
{
	class GONull : public GraphicsObject
	{
	public:
		GONull();
		GONull(const GONull&) = delete;
		GONull& operator = (const GONull&) = delete;
		virtual ~GONull() = default;

		virtual void ActivateGraphics() override;
		virtual void Draw() override;
		virtual GraphicsObject* Clone() override;
	};
}

#endif
