#include "GLTF.h"
#include "GLTF_Helper.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "File.h"
#include "vboDataConverter.h"
#include "MathEngine.h"
#include "azulModel.h"
#include "md5.h"
#include "BoundingSphere.h"
#include "ChickenBot.h"
#include "ConvertAnim.h"

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

bool StringReplace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

bool FileExists(const std::string& filename)
{
	WIN32_FIND_DATAA fd = { 0 };
	HANDLE hFound = FindFirstFileA(filename.c_str(), &fd);
	bool retval = hFound != INVALID_HANDLE_VALUE;
	FindClose(hFound);

	return retval;
}

void WriteMeshDataProtoToFile(const azulModel_proto& source, const char* const filename)
{
	File::Handle fh;

	File::Error err = File::Open(fh, filename, File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	source.SerializeToString(&strOut);

	File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);
}

void ReadMeshDataProtoFromFile(azulModel_proto& output, const char* const filename)
{
	File::Handle fh;

	File::Error err = File::Open(fh, filename, File::Mode::READ);
	assert(err == File::Error::SUCCESS);

	err = File::Seek(fh, File::Position::END, 0);
	assert(err == File::Error::SUCCESS);

	DWORD FileLength;
	err = File::Tell(fh, FileLength);
	assert(err == File::Error::SUCCESS);

	char* poNewBuff = new char[FileLength]();
	assert(poNewBuff);

	err = File::Seek(fh, File::Position::BEGIN, 0);
	assert(err == File::Error::SUCCESS);

	err = File::Read(fh, poNewBuff, FileLength);
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	std::string strIn(poNewBuff, FileLength);
	delete[] poNewBuff;

	output.ParseFromString(strIn);
}

void DumpGLBHeader(const char* poBuff, unsigned int BuffSize)
{
	GLB_header glbHeader;
	bool status = GLTF::GetGLBHeader(glbHeader, poBuff, BuffSize);
	assert(status);

	Trace::out("GLB Header:\n");
	Trace::out("\tmagic: 0x%x\n", glbHeader.magic);
	Trace::out("\tversion: 0x%x\n", glbHeader.version);
	Trace::out("\tlength: 0x%08x %d\n", glbHeader.length, glbHeader.length);
}

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
		StringReplace(fs, ".glb", "_BaseColor.png");

		if (FileExists(fs))
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

		delete[] pVerts;
	}

	azulRunModel.converterVersion = CONVERTER_VERSION;
}

void WriteAndVerifyRuntimeModel(azulModel& azulRunModel, const char* const filenameIn)
{
	azulRunModel.Print("azul_mA");
	azulModel_proto mA_proto;
	azulRunModel.Serialize(mA_proto);

	//runModel.Print("mA");
	//meshData_proto mA_proto;
	//runModel.Serialize(mA_proto);

	Trace::out("message size: %d \n", mA_proto.ByteSizeLong());
	Trace::out("\n");

	std::string meshName = std::string(filenameIn);
	const char* filename = meshName.replace(meshName.end() - 4, meshName.end(), ".proto.azul").c_str();

	WriteMeshDataProtoToFile(mA_proto, filename);

	azulModel_proto mB_proto;
	ReadMeshDataProtoFromFile(mB_proto, filename);

	azulModel mB;
	mB.Deserialize(mB_proto);
	mB.Print("azul_mB");

	PTrace("Created %s\\%s\n", NPath, filename);
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

	PTrace("\n\n\nConverting %s\n", pFileName);

	bool status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pFileName);
	assert(status);

	// First is the GLB header
	DumpGLBHeader(poBuff, BuffSize);

	// Then the JSON description
	status = GLTF::GetRawJSON(poJSON, JsonSize, poBuff, BuffSize);
	assert(status);

	// Everything else is the Binary buffer
	status = GLTF::GetBinaryBuffPtr(pBinaryBuff, BinaryBuffSize, poBuff, BuffSize);
	assert(status);

	status = GLTF::Load(gltfModel, pFileName);
	assert(status);

	ConvertGltfToRuntime(gltfModel, azulRunModel, pBinaryBuff, pFileName);

	WriteAndVerifyRuntimeModel(azulRunModel, pFileName);

	PTrace("Success: converted %s\n", pFileName);

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

	PTrace("\n\n\nConverting texture: %s\n", pFileName);

	const char* const pPlaceholderName = "cube.glb";

	bool status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pPlaceholderName);
	assert(status);

	// First is the GLB header
	DumpGLBHeader(poBuff, BuffSize);

	// Then the JSON description
	status = GLTF::GetRawJSON(poJSON, JsonSize, poBuff, BuffSize);
	assert(status);

	// Everything else is the Binary buffer
	status = GLTF::GetBinaryBuffPtr(pBinaryBuff, BinaryBuffSize, poBuff, BuffSize);
	assert(status);

	status = GLTF::Load(gltfModel, pPlaceholderName);
	assert(status);

	ConvertGltfToRuntime(gltfModel, azulRunModel, pBinaryBuff, pPlaceholderName);

	meshData& runModel = azulRunModel.meshes[0];

	//memcpy_s(
	//	runModel.pMeshName, meshData::FILE_NAME_SIZE,
	//	pFileName, strlen(pFileName)
	//);

	GLTF::SetExternalTexture(pFileName, 0, runModel.text_color);

	azulRunModel.converterVersion = CONVERTER_VERSION;

	WriteAndVerifyRuntimeModel(azulRunModel, pFileName);

	PTrace("Success: converted texture: %s\n", pFileName);

	delete[] poBuff;
	delete[] poJSON;
}

std::vector<std::string> GetAllSourceFileNames(const std::string& pattern)
{
	std::vector<std::string> names;
	WIN32_FIND_DATA fd;

	HANDLE hFind = FindFirstFile(pattern.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				names.push_back(fd.cFileName);
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}

	return names;
}

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	GetCurrentDirectory(MAX_PATH, NPath);
	PTrace("path: %s\n", NPath);

	system("copy ..\\Models\\*.glb .");
	system("copy ..\\Models\\*.png .");
	system("copy ..\\Models\\*.tga .");

	std::vector<std::string> modelsToConvert;

	if (argc == 1)
	{
		modelsToConvert = GetAllSourceFileNames("*.glb");
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			std::string name = argv[i];
			StringReplace(name, "%20", " ");
			modelsToConvert.push_back(name);
		}
	}

	for (const auto& modelName : modelsToConvert)
	{
		PTrace("will convert: %s\n", modelName.c_str());
	}

	if (argc == 1)
	{
		for (const std::string& file : GetAllSourceFileNames("*.tga"))
		{
			ConvertTexture(file.c_str());
		}
	}

	for (const std::string& file : modelsToConvert)
	{
		ConvertModel(file.c_str());
	}

	ConvertAnim("hit_front.glb");
	ConvertAnim("sidestep_right.glb");
	ConvertAnim("run.glb");
	ConvertAnim("walk_mesh.glb");
	ConvertAnim("shot_up.glb");
	CreateChickenBotModels();

	google::protobuf::ShutdownProtobufLibrary();

	system("del *.glb");
	system("del *.png");
	system("del *.tga");

	return 0;
}

// ---  End of File ---------------