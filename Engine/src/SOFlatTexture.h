#ifndef _SOFlatTexture
#define _SOFlatTexture

#include "ShaderObject.h"

namespace Azul
{
	class SOFlatTexture : public ShaderObject
	{
	public:
		SOFlatTexture();
		SOFlatTexture(const SOFlatTexture&) = delete;
		SOFlatTexture& operator = (const SOFlatTexture&) = delete;
		~SOFlatTexture() = default;
	};
}

#endif