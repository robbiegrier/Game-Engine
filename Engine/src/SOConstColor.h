#ifndef _SOConstColor
#define _SOConstColor

#include "ShaderObject.h"

namespace Azul
{
	class SOConstColor : public ShaderObject
	{
	public:

	public:
		SOConstColor();
		SOConstColor(const SOConstColor&) = delete;
		SOConstColor& operator = (const SOConstColor&) = delete;
		~SOConstColor() = default;

	protected:
		virtual void OnOpen(GraphicsObject* pObject) override;
		virtual void OnClose() override;
	};
}

#endif