#include "GLTF.h"
#include "GLTF_Helper.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "vboDataConverter.h"
#include "MathEngine.h"
#include "azulModel.h"
#include "md5.h"
#include "BoundingSphere.h"
#include "ChickenBot.h"
#include "ConvertAnim.h"
#include "ConvertFont.h"
#include "ConvertSkin.h"
#include "ConverterUtils.h"
#include "ProtoAzul.h"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>
#include <Windows.h>

using namespace Azul;
using json = nlohmann::json;

constexpr unsigned int CONVERTER_VERSION = 2;
char NPath[MAX_PATH];

void ConvertGltfToRuntime(tinygltf::Model& gltfModel, azulModel& azulRunModel, char* pBinaryBuff, const char* const pFileName)
{
	for (int i = 0; i < (int)gltfModel.meshes.size(); i++)
	{
		tinygltf::Mesh& gltfMesh = gltfModel.meshes[i];
		tinygltf::Primitive& primitive = gltfMesh.primitives[0];

		if (primitive.material < 0)
		{
			tinygltf::Material mat;
			mat.name = pFileName;
			gltfModel.materials.push_back(mat);
			primitive.material = 0;
		}

		tinygltf::Material& gltfMaterial = gltfModel.materials[primitive.material];
		meshData& runModel = azulRunModel.meshes[i];

		memcpy_s(
			runModel.pMeshName, meshData::FILE_NAME_SIZE,
			gltfMaterial.name.c_str(), gltfMaterial.name.length()
		);

		auto& attributes = primitive.attributes;
		GLTF::SetVBO_pos(gltfModel, attributes["POSITION"], runModel.vbo_vert, pBinaryBuff);
		GLTF::SetVBO(gltfModel, attributes["NORMAL"], runModel.vbo_norm, pBinaryBuff);
		GLTF::SetVBO_uv(gltfModel, attributes["TEXCOORD_0"], runModel.vbo_uv, pBinaryBuff);
		GLTF::SetVBO_index(gltfModel, primitive.indices, runModel.vbo_index, pBinaryBuff);

		runModel.triCount = runModel.vbo_index.count / 3;
		runModel.vertCount = runModel.vbo_vert.count;
		runModel.mode = meshDataConverter::GetMode(primitive.mode);

		const int textureIndexInt = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
		unsigned int colorIndex;

		if (textureIndexInt > 0)
		{
			colorIndex = (unsigned int)textureIndexInt;
		}
		else
		{
			colorIndex = 0u;
		}

		std::string fs = std::string(pFileName);
		ConverterUtils::StringReplace(fs, ".glb", "_BaseColor.png");

		if (ConverterUtils::DoesFileExist(fs))
		{
			PTrace("\tUsing external texture: %s\n", fs.c_str());
			GLTF::SetExternalTexture(fs.c_str(), colorIndex, runModel.text_color);
		}
		else
		{
			GLTF::SetTexture(gltfModel, colorIndex, runModel.text_color, pBinaryBuff);
		}

		runModel.boundingSphereRadius = 10.f;
		runModel.boundingSphereCenter[0] = 1.f;
		runModel.boundingSphereCenter[1] = 2.f;
		runModel.boundingSphereCenter[2] = 3.f;

		Sphere sphere;
		Vec3* pVerts = new Vec3[runModel.vertCount];

		for (unsigned int j = 0; j < runModel.vertCount; j++)
		{
			Vec3f* pVec3f = (Vec3f*)runModel.vbo_vert.poData;
			pVerts[j].set(pVec3f[j].x, pVec3f[j].y, pVec3f[j].z);
		}

		RitterSphere(sphere, pVerts, runModel.vertCount);

		runModel.boundingSphereRadius = sphere.rad;
		runModel.boundingSphereCenter[0] = sphere.cntr[x];
		runModel.boundingSphereCenter[1] = sphere.cntr[y];
		runModel.boundingSphereCenter[2] = sphere.cntr[z];

		GLTF::InsertBoundingBoxData(runModel);

		delete[] pVerts;
	}

	azulRunModel.converterVersion = CONVERTER_VERSION;
}

void WriteAndVerifyRuntimeModel(azulModel& azulRunModel, const char* const filenameIn)
{
	//azulRunModel.Print("azul_mA");
	azulModel_proto mA_proto;
	azulRunModel.Serialize(mA_proto);

	//Trace::out("message size: %d \n", mA_proto.ByteSizeLong());
	//Trace::out("\n");

	std::string meshName = std::string(filenameIn);
	const char* filename = meshName.replace(meshName.end() - 4, meshName.end(), ".proto.azul").c_str();

	ProtoAzul::WriteProtoFile(mA_proto, filename);

	Trace::out("  => Created %s\n", filename);

	//azulModel_proto mB_proto;
	//ProtoAzul::ReadProtoFile(mB_proto, filename);
	//azulModel mB;
	//mB.Deserialize(mB_proto);
	//mB.Print("azul_mB");
	//PTrace("Created %s\\%s\n", NPath, filename);
}

void ConvertModel(const char* const pFileName)
{
	char* poBuff = nullptr;
	char* poJSON = nullptr;
	char* pBinaryBuff = nullptr;
	unsigned int BuffSize = 0u;
	unsigned int BinaryBuffSize = 0u;
	unsigned int JsonSize = 0u;
	tinygltf::Model gltfModel;
	azulModel azulRunModel;

	Trace::out("Converting Mesh: %s\n", pFileName);

	bool status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pFileName); assert(status);
	//GLTF::DumpGLBHeader(poBuff, BuffSize);
	status = GLTF::GetRawJSON(poJSON, JsonSize, poBuff, BuffSize); assert(status);
	status = GLTF::GetBinaryBuffPtr(pBinaryBuff, BinaryBuffSize, poBuff, BuffSize); assert(status);
	status = GLTF::Load(gltfModel, pFileName); assert(status);

	ConvertGltfToRuntime(gltfModel, azulRunModel, pBinaryBuff, pFileName);
	WriteAndVerifyRuntimeModel(azulRunModel, pFileName);

	delete[] poBuff;
	delete[] poJSON;
}

void ConvertTexture(const char* const pFileName)
{
	char* poBuff = nullptr;
	char* poJSON = nullptr;
	char* pBinaryBuff = nullptr;
	unsigned int BuffSize = 0u;
	unsigned int BinaryBuffSize = 0u;
	unsigned int JsonSize = 0u;
	tinygltf::Model gltfModel;
	azulModel azulRunModel;

	Trace::out("Converting Texture: %s\n", pFileName);

	const char* const pPlaceholderName = "cube.glb";

	bool status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pPlaceholderName); assert(status);
	//GLTF::DumpGLBHeader(poBuff, BuffSize);
	status = GLTF::GetRawJSON(poJSON, JsonSize, poBuff, BuffSize); assert(status);
	status = GLTF::GetBinaryBuffPtr(pBinaryBuff, BinaryBuffSize, poBuff, BuffSize); assert(status);
	status = GLTF::Load(gltfModel, pPlaceholderName); assert(status);

	ConvertGltfToRuntime(gltfModel, azulRunModel, pBinaryBuff, pPlaceholderName);
	meshData& runModel = azulRunModel.meshes[0];
	GLTF::SetExternalTexture(pFileName, 0, runModel.text_color);
	azulRunModel.converterVersion = CONVERTER_VERSION;
	WriteAndVerifyRuntimeModel(azulRunModel, pFileName);

	delete[] poBuff;
	delete[] poJSON;
}

// Input types:
//
// GLB as Skinned Mesh:
//		- Mesh						(combine all sub-meshes as one)
//		- Skeleton					(compare with existing skeletons to see if it can be reused)
//		- Texture					(if there is texture data)
//		- Animations				(for each animation)
//
// GLB with Animations Only:
//		- Animations				(for each animation)
//
// GLB as Static Mesh:
//		- Mesh						(option to combine as one, or kitbash)
//		- Texture					(if there is texture data)
//
// TGA or PNG:
//		- Texture
//
// font.xml:
//		- Font Description			(texture converted as PNG job)

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	GetCurrentDirectory(MAX_PATH, NPath);
	PTrace("path: %s\n", NPath);

	system("copy ..\\Models\\*.glb .");
	system("copy ..\\Models\\*.png .");
	system("copy ..\\Models\\*.tga .");
	system("copy ..\\Models\\*.xml .");

	std::vector<std::string> modelsToConvert;

	if (argc == 1)
	{
		modelsToConvert = ConverterUtils::GetAllSourceFileNames("*.glb");
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			std::string name = argv[i];
			ConverterUtils::StringReplace(name, "%20", " ");
			modelsToConvert.push_back(name);
		}
	}

	for (const auto& modelName : modelsToConvert)
	{
		PTrace("will convert: %s\n", modelName.c_str());
	}

	if (argc == 1)
	{
		for (const std::string& file : ConverterUtils::GetAllSourceFileNames("*.tga"))
		{
			ConvertTexture(file.c_str());
		}
	}

	for (const std::string& file : modelsToConvert)
	{
		ConvertModel(file.c_str());
	}

	for (const std::string& file : ConverterUtils::GetAllSourceFileNames("*.png"))
	{
		ConvertTexture(file.c_str());
	}

	for (const std::string& file : ConverterUtils::GetAllSourceFileNames("*.font.xml"))
	{
		ConvertFont(file.c_str());
	}

	ConvertAnim("hit_front.glb");
	ConvertAnim("sidestep_right.glb");
	ConvertAnim("walk_mesh.glb");
	ConvertAnim("run.glb");
	ConvertAnim("shot_up.glb");
	ConvertSkin("walk_mesh.glb");
	ConvertSkeleton("walk_mesh.glb");

	ConvertAnim("mannequin.glb");
	ConvertAnim("RunJump.glb");
	ConvertSkeleton("mannequin.glb");
	ConvertSkin("mannequin.glb");

	ConvertSkin("Paladin.glb");
	ConvertAnim("Paladin.glb");
	ConvertSkeleton("Paladin.glb");

	ConvertSkin("Knight.glb");
	ConvertAnim("Knight.glb");
	ConvertSkeleton("Knight.glb");

	google::protobuf::ShutdownProtobufLibrary();

	system("del *.glb");
	system("del *.png");
	system("del *.tga");
	system("del *.xml");

	return 0;
}