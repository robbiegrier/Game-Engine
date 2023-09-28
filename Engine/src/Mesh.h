#ifndef _Model
#define _Model

#include <d3d11.h>
#include "MeshUtils.h"
#include "Camera.h"
#include "DLink.h"

namespace Azul
{
	// A Mesh is a set of verticies and metadata representing an object that
	// can be rendered as part of a graphics object.
	class Mesh : public DLink
	{
	public:
		enum class Name
		{
			Cube,
			Pyramid,
			Diamond,
			Cross,
			Plane,
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
		void UpdateWorld(const Mat4& pWorld);
		void RenderIndexBuffer();

		// As a DLink Node
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;
		void SetName(Name inName);
		Name GetName() const;
		const char* NameToString();

	protected:
		Name name;

		unsigned int numVerts;
		unsigned int numIndices;

		ID3D11Buffer* poVertexBuffer_pos;
		ID3D11Buffer* poVertexBuffer_color;
		ID3D11Buffer* poVertexBuffer_norm;
		ID3D11Buffer* poVertexBuffer_texCoord;

		ID3D11Buffer* poIndexBuffer;

		ID3D11Buffer* poConstantBuff_Projection;
		ID3D11Buffer* poConstantBuff_World;
		ID3D11Buffer* poConstantBuff_View;

	public:
		ID3D11Buffer* poConstantBuff_lightColor;
		ID3D11Buffer* poConstantBuff_lightPos;
	};
}

#endif
