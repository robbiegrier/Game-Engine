//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "meshData.h"

meshData::~meshData()
{
}

meshData::meshData()
	:pMeshName{ 0 },
	mode{ meshData::RENDER_MODE::DEFAULT },
	triCount{ 0 },
	vertCount{ 0 },
	vbo_vert(),
	vbo_norm(),
	vbo_uv(),
	vbo_index(),
	text_color(),
	boundingSphereRadius(0u)
{
	memset(boundingSphereCenter, 0, sizeof(boundingSphereCenter));
}

void meshData::Serialize(meshData_proto& out) const
{
	AZUL_UNUSED_VAR(out);
	std::string sName((const char*)this->pMeshName, FILE_NAME_SIZE);
	out.set_pmeshname(sName);
	out.set_mode((meshData_proto_RENDER_MODE)this->mode);
	out.set_tricount(this->triCount);
	out.set_vertcount(this->vertCount);

	vboData_proto* pVBO_proto;

	pVBO_proto = new vboData_proto();
	this->vbo_vert.Serialize(*pVBO_proto);
	out.set_allocated_vbo_vert(pVBO_proto);

	pVBO_proto = new vboData_proto();
	this->vbo_norm.Serialize(*pVBO_proto);
	out.set_allocated_vbo_norm(pVBO_proto);

	pVBO_proto = new vboData_proto();
	this->vbo_uv.Serialize(*pVBO_proto);
	out.set_allocated_vbo_uv(pVBO_proto);

	pVBO_proto = new vboData_proto();
	this->vbo_index.Serialize(*pVBO_proto);
	out.set_allocated_vbo_index(pVBO_proto);

	textureData_proto* pText_proto;

	pText_proto = new textureData_proto();
	this->text_color.Serialize(*pText_proto);
	out.set_allocated_text_color(pText_proto);

	out.set_boundingsphereradius(boundingSphereRadius);

	out.add_boundingspherecenter(boundingSphereCenter[0]);
	out.add_boundingspherecenter(boundingSphereCenter[1]);
	out.add_boundingspherecenter(boundingSphereCenter[2]);
}

void meshData::Deserialize(const meshData_proto& in)
{
	memcpy_s(this->pMeshName, FILE_NAME_SIZE, in.pmeshname().data(), FILE_NAME_SIZE);
	this->mode = (RENDER_MODE)in.mode();
	this->triCount = in.tricount();
	this->vertCount = in.vertcount();
	this->vbo_vert.Deserialize(in.vbo_vert());
	this->vbo_norm.Deserialize(in.vbo_norm());
	this->vbo_uv.Deserialize(in.vbo_uv());
	this->vbo_index.Deserialize(in.vbo_index());

	this->text_color.Deserialize(in.text_color());

	boundingSphereRadius = in.boundingsphereradius();
	boundingSphereCenter[0] = in.boundingspherecenter().at(0);
	boundingSphereCenter[1] = in.boundingspherecenter().at(1);
	boundingSphereCenter[2] = in.boundingspherecenter().at(2);
}

void meshData::Print(const char* const pName) const
{
	Trace::out("%s: \n", pName);

	Trace::out("\n");
	this->vbo_vert.Print("vbo_vert");
	Trace::out("\n");
	this->vbo_norm.Print("vbo_norm");
	Trace::out("\n");
	this->vbo_uv.Print("vbo_uv");
	Trace::out("\n");
	this->vbo_index.Print("vbo_index");
	Trace::out("\n");
	this->text_color.Print("text_color");
	Trace::out("\n");
	Trace::out("radius: %f\n", boundingSphereRadius);
	Trace::out("center: %f %f %f\n", boundingSphereCenter[0], boundingSphereCenter[1], boundingSphereCenter[2]);
}

bool meshData::IsEmpty() const
{
	return strlen(pMeshName) == 0u;
}

// --- End of File ---