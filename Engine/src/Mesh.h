#ifndef _Model
#define _Model

#include <d3d11.h>
#include "MeshUtils.h"
#include "Camera.h"
#include "DLink.h"
#include "DLinkedList.h"
#include "Clip.h"

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
			BoneTransform,
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
			Sprite,
			ChickenBotSkin,
			MannequinSkin,
			PaladinSkin,
			KnightSkin,
			UnitCube,
			Terrain,
			SimpleRock,
			SimpleRock2,
			DryGrass0,
			DryGrass1,
			DryGrass2,
			DryGrass3,
			TropicalTree0,
			TropicalTree1,
			TropicalTree2,
			TropicalTree3,
			BirchTree0,
			BirchTree1,
			BirchTree2,
			BirchTree3,
			BirchTree4,
			BirchTree5,
			BirchTree6,
			BirchTree7,
			Cylinder,
			SimpleRock3,
			SimpleRock4,
			SimpleRock5,
			SimpleRock6,
			Null,
			None
		};
		static constexpr int BoneCountMax = BONE_COUNT_MAX;

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
		const Vec3& GetAABBMax() const;
		const Vec3& GetAABBMin() const;

		void TransferUVCorrection(Mat4* pUVMatrix);
		void TransferConstantBufferBoneWorldArray(Mat4* pWorldBoneArray);

		void AttachSubMesh(Mesh* pInSubMesh);

		ID3D11Buffer* GetConstantBufferLightColor() const { return pConstantBufferLightColor; }
		Mat4* GetInverseBindArray() const { return pInverseBindArray; }
		const DLinkedList& GetSubMeshList() const { return subMeshes; }

		unsigned int GetNumIndicies() const { return numIndices; }

	protected:
		void HackSetBoundingSphereData(VertexPos* pData);
		void HackSetBoundingBoxData(VertexPos* pData);

		// Index buffer
		ID3D11Buffer* pIndexBufferTriangles;

		// Vertex Buffers
		ID3D11Buffer* pVertexBufferPosition;
		ID3D11Buffer* pVertexBufferColor;
		ID3D11Buffer* pVertexBufferNormal;
		ID3D11Buffer* pVertexBufferTextureCoordinates;
		ID3D11Buffer* pVertexBufferJoints;
		ID3D11Buffer* pVertexBufferWeights;

		// Constant Buffers
		ID3D11Buffer* pConstantBufferLightColor;
		ID3D11Buffer* pConstantBufferLightPosition;
		ID3D11Buffer* pConstantBufferUVMatrix;
		ID3D11Buffer* pConstantBufferBoneWorld;

		// Animation Data
		Mat4* pInverseBindArray;

		// Submeshes as a composite
		DLinkedList subMeshes;

		// Metadata
		Name name;
		unsigned int numVerts;
		unsigned int numIndices;

		// Bounding volume data
		float boundingSphereRadius;
		Vec3* pBoundingSphereCenter;
		Vec3* pAABBMax;
		Vec3* pAABBMin;

	public:
		constexpr static UINT vertexStride_pos = sizeof(VertexPos);
		constexpr static UINT vertexStride_color = sizeof(VertexColor);
		constexpr static UINT vertexStride_norm = sizeof(VertexNorm);
		constexpr static UINT vertexStride_texCoord = sizeof(VertexTexCoord);
		constexpr static UINT vertexStride_weights = sizeof(VertexWeights);
		constexpr static UINT vertexStride_joints = sizeof(VertexJoints);
		constexpr static UINT offset = 0u;
	};
}

#endif
