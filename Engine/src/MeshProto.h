#ifndef MESH_PROTO_H
#define MESH_PROTO_H

#include "Mesh.h"
#include "azulModel.h"

namespace Azul
{
	// A mesh loaded from a proto.azul file
	class MeshProto : public Mesh
	{
	public:
		// Big four
		MeshProto(const char* const pMeshFileName, unsigned int index = 0);
		MeshProto(const meshData& mB);
		MeshProto() = delete;
		MeshProto(const MeshProto&) = delete;
		MeshProto& operator = (MeshProto&) = delete;
		virtual ~MeshProto();

	private:
		void SetFromMeshData(const meshData& mB);
		void VerifyImportedData(const meshData& mB) const;
	};
}

#endif
