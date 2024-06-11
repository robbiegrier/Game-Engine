#ifndef _SOFoliage
#define _SOFoliage

#include "ShaderObject.h"
#include "MathEngine.h"
#include "FoliageType.h"
#include "LightSystem.h"

namespace Azul
{
	class GraphicsObject;

	class SOFoliage : public ShaderObject, public Align16
	{
	public:
		// Big four
		SOFoliage();
		SOFoliage(const SOFoliage&) = delete;
		SOFoliage& operator = (const SOFoliage&) = delete;
		~SOFoliage();

		virtual void SetFogParameters(float start, float range, const Vec4& col);

		void UnifiedOpen();
		void FoliageTypeOpen(FoliageType* pFolaigeType);

		struct CBLightscape
		{
			int numPointLights;
			PointLight pointLights[LightSystem::MAX_LIGHTS];
			DirectionalLight directionalLight;
			Vec4 eyePositionWorld;
		};
		ID3D11Buffer* pConstBuffLightscape;

		void BindLights();

		struct CBFoliageParameters
		{
			float renderDistance = 1500.f;
			float scaleFalloffStart = 1000.f;
			float followNormal = 1.f;
			float pad3 = 0.f;
		};

		CBFoliageParameters& GetFoliageParams() { return foliageParams; }

	protected:
		virtual void OnOpen(GraphicsObject* pObject) override;
		virtual void OnClose() override;
		void SetCurrentObject(GraphicsObject* pObject);

	private:
		struct CBObject
		{
			Mat4 inverse;
			Material material;
		};
		ID3D11Buffer* pConstBuffObject;

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

		ID3D11Buffer* pConstBuffFoliageParams;
		CBFoliageParameters foliageParams;

		enum class ConstBuffSlot : UINT
		{
			Lightscape = 5u,
			Object = 6u,
			Fog = 7u,
			TerrainParams = 8u,
			FoliageParams = 9u
		};
	};
}

#endif