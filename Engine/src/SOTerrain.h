#ifndef _SOTerrain
#define _SOTerrain

#include "ShaderObject.h"
#include "MathEngine.h"
#include "LightSystem.h"

namespace Azul
{
	class GraphicsObject;

	class SOTerrain : public ShaderObject, public Align16
	{
	public:
		// Big four
		SOTerrain();
		SOTerrain(const SOTerrain&) = delete;
		SOTerrain& operator = (const SOTerrain&) = delete;
		~SOTerrain();

		virtual void SetFogParameters(float start, float range, const Vec4& col);

		void SetBrushRadius(float radius) { brushArgs.brushRadius = radius; }
		void SetBrushRaycast(const Vec4& raycast) { brushArgs.raycastPos = raycast; }

		void ToggleWireframeMode(bool in) { wireframeMode = in; }

	protected:
		virtual void OnOpen(GraphicsObject* pObject) override;
		virtual void OnClose() override;
		void SetCurrentObject(GraphicsObject* pObject);

	private:
		bool wireframeMode = false;

		struct CBObject
		{
			Mat4 inverse;
			Material material;
		};
		ID3D11Buffer* pConstBuffObject;

		struct CBLightscape
		{
			int numPointLights;
			PointLight pointLights[LightSystem::MAX_LIGHTS];
			DirectionalLight directionalLight;
			Vec4 eyePositionWorld;
		};
		ID3D11Buffer* pConstBuffLightscape;

		struct CBFog
		{
			float FogStart = INFINITY;
			float FogRange = INFINITY;
			Vec4 FogColor = Vec4(0, 0, 0, 0);
		};
		CBFog fog;
		ID3D11Buffer* pConstBuffFog;

		struct CBTerrainParameters
		{
			float gridCellSize;
			float halfWorldScale;
			float heightScale;
			float pad1 = 0.f;
		};
		ID3D11Buffer* pConstBuffTerrainParams;

		struct CBTerrainBrushArgs
		{
			Vec4 raycastPos;
			float brushRadius = 0.f;
			float pad0;
			float pad1;
			float pad2;
		};
		CBTerrainBrushArgs brushArgs;
		ID3D11Buffer* pConstBuffTerrainBrushArgs;

		enum class ConstBuffSlot : UINT
		{
			Lightscape = 5u,
			Object = 6u,
			Fog = 7u,
			TerrainParams = 8u,
			TerrainBrushArgs = 9u
		};
	};
}

#endif