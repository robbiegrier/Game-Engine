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
		virtual void SetPointLightParameters(const Vec3& pos, float r, const Vec3& att,
			const Vec3& amb = Vec3(1.f, 1.f, 1.f), const Vec3& dif = Vec3(1.f, 1.f, 1.f), const Vec3& sp = Vec3(1.f, 1.f, 1.f));

	protected:
		virtual void OnOpen(GraphicsObject* pObject) override;
		void SetCurrentObject(GraphicsObject* pObject);

	private:
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

		struct CBObject
		{
			Mat4 world;
			Mat4 inverse;
			Material material;
		};
		ID3D11Buffer* pConstBuffObject;

		struct CBLightscape
		{
			PointLight pointLight;
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