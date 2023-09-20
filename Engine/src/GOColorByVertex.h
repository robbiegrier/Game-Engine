#ifndef _GOColorByVertex
#define _GOColorByVertex

#include "GraphicsObject.h"

namespace Azul
{
	class GOColorByVertex : public GraphicsObject
	{
	public:
		GOColorByVertex(Mesh* model, ShaderObject* pShaderObj);
		GOColorByVertex() = delete;
		GOColorByVertex(const GOColorByVertex&) = delete;
		GOColorByVertex& operator = (const GOColorByVertex&) = delete;
		virtual ~GOColorByVertex() = default;

		virtual void ActivateGraphics() override;
		virtual void Draw() override;
	};
}

#endif
