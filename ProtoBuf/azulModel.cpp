#include "azulModel.h"

azulModel::~azulModel()
{
}

azulModel::azulModel()
{
}

void azulModel::Serialize(azulModel_proto& out) const
{
	for (const meshData& mesh : meshes)
	{
		if (mesh.vertCount > 0)
		{
			meshData_proto* mB_proto = out.add_meshes();
			mesh.Serialize(*mB_proto);
		}
	}

	out.set_converterversion(converterVersion);
}

void azulModel::Deserialize(const azulModel_proto& in)
{
	int i = 0;
	for (const meshData_proto& mB_proto : in.meshes())
	{
		meshes[i++].Deserialize(mB_proto);
	}

	converterVersion = in.converterversion();
}

void azulModel::Print(const char* const pName) const
{
	Trace::out("%s: \n", pName);
	Trace::out("Converter Version: v%d\n", converterVersion);

	for (const meshData& mesh : meshes)
	{
		if (!mesh.IsEmpty())
		{
			mesh.Print("mesh");
		}
	}
}

bool azulModel::IsEmpty() const
{
	for (const meshData& mesh : meshes)
	{
		if (!mesh.IsEmpty())
		{
			return false;
		}
	}

	return true;
}