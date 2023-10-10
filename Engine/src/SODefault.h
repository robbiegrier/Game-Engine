#ifndef _SODefault
#define _SODefault

#include "ShaderObject.h"
#include "MathEngine.h"

namespace Azul
{
	class GraphicsObject;

	class SODefault : public ShaderObject
	{
	public:
		// Big four
		SODefault();
		SODefault(const SODefault&) = delete;
		SODefault& operator = (const SODefault&) = delete;
		~SODefault();

		// Set the data for a point light in the scene
		virtual void SetPointLightParameters(int index, const Vec3& pos, float r, const Vec3& att, const Vec3& amb, const Vec3& dif, const Vec3& sp);

		// Set the data for the directional light in the scene
		void SetDirectionalLightParameters(const Vec3& dir, const Vec3& amb, const Vec3& dif, const Vec3& sp);

	protected:
		virtual void OnOpen(GraphicsObject* pObject) override;
		void SetCurrentObject(GraphicsObject* pObject);

	private:
		static constexpr int MaxLights = 15;

		struct Material
		{
			Vec4 ambient;
			Vec4 diffuse;
			Vec4 specular;
		};

		struct PointLight
		{
			Material light;
			Vec4 position;
			Vec4 attenuation;
			float range;
		};
		PointLight pointLight;
		PointLight pointLights[MaxLights];

		struct DirectionalLight
		{
			Material light;
			Vec4 direction;
		};
		DirectionalLight directionalLight;

		struct CBObject
		{
			Mat4 world;
			Mat4 inverse;
			Material material;
		};
		ID3D11Buffer* pConstBuffObject;

		struct CBLightscape
		{
			int numPointLights;
			PointLight pointLights[MaxLights];
			DirectionalLight directionalLight;
			Vec4 eyePositionWorld;
		};
		ID3D11Buffer* pConstBuffLightscape;

		enum class ConstBuffSlot : UINT
		{
			Lightscape = 5u,
			Object = 6u
		};
	};
}

#endif