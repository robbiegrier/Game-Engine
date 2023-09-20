#ifndef _SOColorByVertex
#define _SOColorByVertex

#include "ShaderObject.h"

namespace Azul
{
	class SOColorByVertex : public ShaderObject
	{
	public:
		SOColorByVertex();
		SOColorByVertex(const SOColorByVertex&) = delete;
		SOColorByVertex& operator = (const SOColorByVertex&) = delete;
		~SOColorByVertex() = default;
	};
}

#endif
