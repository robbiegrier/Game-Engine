#ifndef _SOSprite
#define _SOSprite

#include "ShaderObject.h"
#include "MathEngine.h"

namespace Azul
{
	class GraphicsObject;

	class SOSprite : public ShaderObject
	{
	public:
		// Big four
		SOSprite();
		SOSprite(const SOSprite&) = delete;
		SOSprite& operator = (const SOSprite&) = delete;
		~SOSprite();

	protected:
		virtual void OnOpen(GraphicsObject* pObject) override;
		virtual void OnClose() override;

		virtual Camera* GetCamera() const override;

	private:
		ID3D11BlendState* pBlendStateSprite;
		static constexpr float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr UINT sampleMask = 0xffffffff;
	};
}

#endif