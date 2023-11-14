#ifndef AZUL_MODEL_H
#define AZUL_MODEL_H

#include "azulModel.pb.h"
#include "meshData.h"

class azulModel
{
public:
	azulModel();
	azulModel(const azulModel&) = delete;
	azulModel& operator = (const azulModel&) = delete;
	~azulModel();

	void Serialize(azulModel_proto& out) const;
	void Deserialize(const azulModel_proto& in);

	void Print(const char* const pName) const;

	meshData      meshes[24];

	unsigned int converterVersion;

	bool IsEmpty() const;
};

#endif

// --- End of File ---
