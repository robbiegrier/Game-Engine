#ifndef _SONull
#define _SONull

#include "ShaderObject.h"

namespace Azul
{
	class SONull : public ShaderObject
	{
	public:

	public:
		SONull() = default;
		SONull(const SONull&) = delete;
		SONull& operator = (const SONull&) = delete;
		~SONull() = default;

		virtual void Open(GraphicsObject* pObject) override;
		virtual void Close() override;
	};
}

#endif