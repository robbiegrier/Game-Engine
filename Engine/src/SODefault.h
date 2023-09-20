#ifndef _SODefault
#define _SODefault

#include "ShaderObject.h"
#include "MathEngine.h"

namespace Azul
{
	class SODefault : public ShaderObject
	{
	public:
		SODefault();
		SODefault(const SODefault&) = delete;
		SODefault& operator = (const SODefault&) = delete;
		~SODefault();

		void SendWorldAndMaterial(const Mat4& world, const Vec3& amb = Vec3(.5f, .5f, .5f), const Vec3& dif = Vec3(.5f, .5f, .5f), const Vec3& sp = Vec3(.5f, .5f, .5f));
		virtual void SetPointLightParameters(const Vec3& pos, float r, const Vec3& att, const Vec3& amb = Vec3(1, 1, 1), const Vec3& dif = Vec3(1, 1, 1), const Vec3& sp = Vec3(1, 1, 1));

	protected:
		virtual void OnOpen(GraphicsObject* pObject) override;

	private:
		struct Material
		{
			Vec4 Ambient;
			Vec4 Diffuse;
			Vec4 Specular;
		};

		struct PhongADS
		{
			Vec4 Ambient;
			Vec4 Diffuse;
			Vec4 Specular;
		};

		struct PointLight
		{
			PhongADS Light;
			Vec4 Position;
			Vec4 Attenuation;
			float Range;
		};

		PointLight PointLightData;

		struct Data_WorldAndMaterial
		{
			Mat4 World;
			Mat4 WorldInv;
			Material Mat;
		};
		ID3D11Buffer* mpBuffWordAndMaterial;

		struct Data_LightParams
		{
			PointLight PntLight;
			Vec4 EyePosWorld;
		};
		ID3D11Buffer* mpBufferLightParams;
	};
}

#endif