#ifndef _Model
#define _Model

#include <d3d11.h>
#include "MeshUtils.h"
#include "Camera.h"
#include "DLink.h"
#include "DLinkedList.h"

namespace Azul
{
	// A Mesh is a set of verticies and metadata representing an object that
	// can be rendered as part of a graphics object.
	class Mesh : public DLink
	{
	public:
		enum class Name
		{
			Sphere,
			Cube,
			Pyramid,
			Diamond,
			Cross,
			Plane,
			Crate,
			Frigate,
			Bracket,
			Fish,
			DogHouse,
			Duck,
			R2D2,
			Corset,
			AntiqueCameraTripod,
			AntiqueCamera,
			Dog,
			Dog1,
			WesternTownHouse,
			WesternTownHouse1,
			DesertRock0,
			DesertRock1,
			DesertRock2,
			DesertRock3,
			DesertRock4,
			DesertRock5,
			DesertRock6,
			DesertRock7,
			DesertRock8,
			DesertRock9,
			DesertRock10,
			DesertRock11,
			DesertRock12,
			DesertRock13,
			Tattoine0,
			Tattoine1,
			Tattoine2,
			Tattoine3,
			Tattoine4,
			Tattoine5,
			Tattoine6,
			Tattoine7,
			Tattoine8,
			Tattoine9,
			Tattoine10,
			Tattoine11,
			Tattoine12,
			Tattoine13,
			Tattoine14,
			Tattoine15,
			Tattoine16,
			Tattoine17,
			Tattoine18,
			Tattoine19,
			Tattoine20,
			Tattoine21,
			Tattoine22,
			Tattoine23,
			Tattoine24,
			Tattoine25,
			Tattoine26,
			Tattoine27,
			Tattoine28,
			Tattoine29,
			Tattoine30,
			Tattoine31,
			Tattoine32,
			Tattoine33,
			Tattoine34,
			Tattoine35,
			Tattoine36,
			Tattoine37,
			Tattoine38,
			Tattoine39,
			UnitSphere,
			UnitIcoSphere,
			Bone,
			Bone_001,
			Bone_L,
			Bone_L_001,
			Bone_L_002,
			Bone_R,
			Bone_R_001,
			Bone_R_002,
			TranslateHandle,
			ScaleHandle,
			RotateHandle,
			Null,
			None
		};

		// Big four
		Mesh(unsigned int numVerts, unsigned int numTriangles);
		Mesh() = delete;
		Mesh(Mesh& copyModel) = delete;
		Mesh& operator = (Mesh& copyModel) = delete;
		virtual ~Mesh();

		// Initialization
		ID3D11Buffer* CreateVertexBuffer(unsigned int numBytes, void* pData);
		ID3D11Buffer* CreateIndexBuffer(unsigned int numBytes, void* pData);
		static ID3D11Buffer* CreateConstantBuffer(unsigned int numBytes);

		// Called during rendering
		void ActivateModel();
		void TransferConstantBuffer(Camera* pCam, const Mat4& pWorld);
		void RenderIndexBuffer();

		// As a DLink Node
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;
		void SetName(Name inName);
		Name GetName() const;
		const char* NameToString();

		float GetBoundingSphereRadius() const;
		const Vec3& GetBoundSphereCenter() const;

	protected:
		void HackSetBoundingSphereData(VertexPos* pData);

		Name name;

		unsigned int numVerts;
		unsigned int numIndices;

		ID3D11Buffer* poVertexBuffer_pos;
		ID3D11Buffer* poVertexBuffer_color;
		ID3D11Buffer* poVertexBuffer_norm;
		ID3D11Buffer* poVertexBuffer_texCoord;
		ID3D11Buffer* poIndexBuffer;

		float boundingSphereRadius;
		Vec3* pBoundingSphereCenter;

	public:
		ID3D11Buffer* poConstantBuff_lightColor;
		ID3D11Buffer* poConstantBuff_lightPos;
	};
}

#endif
