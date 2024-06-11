#ifndef _SOSurfaceShape
#define _SOSurfaceShape

#include "ShaderObject.h"

namespace Azul
{
	class SOSurfaceShape : public ShaderObject
	{
	public:
		SOSurfaceShape();
		SOSurfaceShape(const SOSurfaceShape&) = delete;
		SOSurfaceShape& operator = (const SOSurfaceShape&) = delete;
		~SOSurfaceShape();

		virtual void OnOpen(GraphicsObject* pObject) override;
		virtual void OnClose() override;

	private:
		ID3D11BlendState* pBlendState;
		static constexpr float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr UINT sampleMask = 0xffffffff;

		struct CBTerrainParameters
		{
			float gridCellSize;
			float halfWorldScale;
			float heightScale;
			float pad1 = 0.f;
		};
		ID3D11Buffer* pConstBuffTerrainParams;

		ID3D11RasterizerState* pRasterizerState;
	};
}

#endif