#ifndef _SOLightTexture
#define _SOLightTexture

#include "ShaderObject.h"

namespace Azul
{
	class SOLightTexture : public ShaderObject
	{
	public:
		SOLightTexture();
		SOLightTexture(const SOLightTexture&) = delete;
		SOLightTexture& operator = (const SOLightTexture&) = delete;
		~SOLightTexture() = default;
	};
}

#endif