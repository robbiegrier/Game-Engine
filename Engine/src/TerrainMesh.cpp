#include "TerrainMesh.h"
#include "Engine.h"
#include "vboData.h"
#include "CSTerrainBrush.h"
#include "TextureObject.h"
#include "TextureObjectManager.h"
#include "TerrainFoliage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Azul
{
	TerrainMesh::TerrainMesh()
		: Mesh(0u, 0u), worldScale(0.f), vertexResolution(0u), heightScale(500.f),
		pVertexGrid{ nullptr }, pIndicies{ nullptr }, pTerrainFoliage{ nullptr }
	{
		for (uint i = 0; i < NUM_SURFACES; i++)
		{
			pSurfaceLayerTextures[i] = nullptr;
		}
	}

	TerrainMesh::TerrainMesh(const char* pInName, float inWorldScale, unsigned int inVertexResolution)
		: Mesh(0u, 0u), worldScale(inWorldScale), vertexResolution(inVertexResolution), heightScale(500.f),
		pVertexGrid{ nullptr }, pIndicies{ nullptr }, pTerrainFoliage{ nullptr }
	{
		AnimTimer timer;
		timer.Tic();

		memcpy(terrainName, pInName, NAME_SIZE);

		assert(vertexResolution > 1);
		assert((vertexResolution & (vertexResolution - 1)) == 0 && "Vertex Resolution must be a power of 2");
		assert(worldScale > 0.f);

		numVerts = vertexResolution * vertexResolution;
		numIndices = 2 * (vertexResolution - 1) * (vertexResolution - 1) * 3;

		ColorType* pElevationTmpBuffer = new ColorType[numVerts];
		memset(pElevationTmpBuffer, 0, numVerts * sizeof(ColorType));

		for (unsigned int i = 0u; (int)i < inVertexResolution; i++)
		{
			for (unsigned int j = 0u; (int)j < inVertexResolution; j++)
			{
				pElevationTmpBuffer[GetGridIndex(i, j)] = ColorType(0);
			}
		}

		float* pSurfacesTmpBuffer = GetInitSurfaces();
		GenerateGrid(pElevationTmpBuffer, pSurfacesTmpBuffer);

		for (uint i = 0; i < NUM_SURFACES; i++)
		{
			pSurfaceLayerTextures[i] = TextureObjectManager::Find(TextureObject::Name::Desert);
		}

		delete[] pSurfacesTmpBuffer;
		delete[] pElevationTmpBuffer;

		AnimTime time = timer.Toc();
		Trace::out("Generated Terrain in: %ds %dms\n",
			(int)AnimTime::Quotient(time, AnimTime(AnimTime::Duration::ONE_SECOND)),
			(int)AnimTime::Quotient(AnimTime::Remainder(time, AnimTime(AnimTime::Duration::ONE_SECOND)), AnimTime(AnimTime::Duration::ONE_MILLISECOND)));
	}

	TerrainMesh::TerrainMesh(const char* pInName, float inWorldScale, float inHeight, const char* pHeightmapFileName)
		: Mesh(0u, 0u), worldScale(inWorldScale), vertexResolution(0u), heightScale(inHeight),
		pVertexGrid{ nullptr }, pIndicies{ nullptr }, pTerrainFoliage{ nullptr }
	{
		AnimTimer timer;
		timer.Tic();

		memcpy(terrainName, pInName, NAME_SIZE);

		int importImageWidth;
		int importImageHeight;
		int importImageComponents;
		int importImageDesiredComponents = 3;
		stbi_us* pImage = stbi_load_16(pHeightmapFileName, &importImageWidth, &importImageHeight, &importImageComponents, importImageDesiredComponents);

		vertexResolution = 2048u;
		numVerts = vertexResolution * vertexResolution;
		numIndices = 2 * (vertexResolution - 1) * (vertexResolution - 1) * 3;

		for (uint i = 0; i < NUM_SURFACES; i++)
		{
			pSurfaceLayerTextures[i] = TextureObjectManager::Find(TextureObject::Name::CoastalSandRocks);
		}

		ColorType* pElevationTmpBuffer = new ColorType[numVerts];
		memset(pElevationTmpBuffer, 0, numVerts * sizeof(ColorType));

		uint16_t* p = pImage;
		for (unsigned int i = 0u; (int)i < importImageWidth; i++)
		{
			for (unsigned int j = 0u; (int)j < importImageWidth; j++)
			{
				pElevationTmpBuffer[GetGridIndex(i, j)] = *p;
				p += 3;
			}
		}

		float* pSurfacesTmpBuffer = GetInitSurfaces();
		GenerateGrid(pElevationTmpBuffer, pSurfacesTmpBuffer);

		delete[] pSurfacesTmpBuffer;
		delete[] pElevationTmpBuffer;
		delete pImage;

		AnimTime time = timer.Toc();
		Trace::out("Generated Terrain in: %ds %dms\n",
			(int)AnimTime::Quotient(time, AnimTime(AnimTime::Duration::ONE_SECOND)),
			(int)AnimTime::Quotient(AnimTime::Remainder(time, AnimTime(AnimTime::Duration::ONE_SECOND)), AnimTime(AnimTime::Duration::ONE_MILLISECOND)));
	}

	TerrainMesh::~TerrainMesh()
	{
		delete[] pVertexGrid;
		delete[] pIndicies;
		delete[] pTextureCoordinates;
		delete[] pHeightfieldTexture;
		delete[] pSurfaceWeights;
		delete pTerrainFoliage;

		SafeRelease(pTexture);
		SafeRelease(pShaderResourceView);
		SafeRelease(pSurfaceLayerWeights);
		SafeRelease(pSurfaceLayerWeightsViews);
	}

	void TerrainMesh::UpdateTextureCoordinates(float tiling)
	{
		if (pVertexBufferTextureCoordinates)
		{
			SafeRelease(pVertexBufferTextureCoordinates);
		}

		const float halfWorldScale = worldScale / 2.f;

		for (unsigned int i = 0; i < vertexResolution; i++)
		{
			for (unsigned int j = 0; j < vertexResolution; j++)
			{
				const unsigned int gridIndex = GetGridIndex(i, j);
				Vec3f currentPos = pVertexGrid[gridIndex];
				pTextureCoordinates[gridIndex] = Vec2f(tiling * (currentPos.x / -halfWorldScale), tiling * (currentPos.z / -halfWorldScale));
			}
		}

		pVertexBufferTextureCoordinates = CreateVertexBuffer(sizeof(Vec2f) * numVerts, pTextureCoordinates);
	}

	Vec3 TerrainMesh::GetVertex(unsigned int i, unsigned int j) const
	{
		const Vec3f& loc = pVertexGrid[GetGridIndex(i, j)];
		return Vec3(loc.x, loc.y, loc.z);
	}

	const Vec3f& TerrainMesh::GetVertexf(unsigned int i, unsigned int j) const
	{
		const Vec3f& loc = pVertexGrid[GetGridIndex(i, j)];
		return loc;
	}

	const Vec3f& TerrainMesh::GetVertexf(unsigned int i) const
	{
		const Vec3f& loc = pVertexGrid[i];
		return loc;
	}

	unsigned int TerrainMesh::GetGridIndex(unsigned int i, unsigned int j) const
	{
		return (j * vertexResolution) + i;
	}

	void TerrainMesh::Serialize(terrain_proto& out) const
	{
		const unsigned int dataSize = numVerts * sizeof(uint16_t);

		uint16_t* elevationData = new uint16_t[dataSize];
		for (unsigned int i = 0; i < numVerts; i++)
		{
			elevationData[i] = GetColorFromElevation(pVertexGrid[i].y);
		}

		out.set_pvertexdata(elevationData, dataSize);
		delete elevationData;

		out.set_numverts(numVerts);
		out.set_vertexresolution(vertexResolution);
		out.set_worldscale(worldScale);
		out.set_terrainname(GetTerrainMeshName());
		out.set_heightscale(heightScale);

		const unsigned int dataSizeWeights = GetSurfaceWeightsTextureResolution() * GetSurfaceWeightsTextureResolution() * sizeof(float) * 4u;
		out.set_psurfaceweightsdata(pSurfaceWeights, dataSizeWeights);

		for (uint i = 0u; i < NUM_SURFACES; i++)
		{
			out.add_surfacetextureids((uint32_t)pSurfaceLayerTextures[i]->GetName());
		}

		pTerrainFoliage->Serialize(out);
	}

	void TerrainMesh::Deserialize(const terrain_proto& in)
	{
		AnimTimer timer;
		timer.Tic();

		numVerts = in.numverts();
		vertexResolution = in.vertexresolution();
		worldScale = in.worldscale();
		heightScale = in.heightscale();
		numIndices = 2 * (vertexResolution - 1) * (vertexResolution - 1) * 3;
		memcpy_s(&terrainName, NAME_SIZE, in.terrainname().data(), NAME_SIZE);

		GenerateGrid((uint16_t*)in.pvertexdata().data(), (float*)in.psurfaceweightsdata().data());

		for (uint i = 0u; i < NUM_SURFACES; i++)
		{
			pSurfaceLayerTextures[i] = TextureObjectManager::Find((TextureObject::Name)in.surfacetextureids().Get(i));
		}

		AnimTime time = timer.Toc();
		Trace::out("Deserialized Terrain in: %ds %dms\n",
			(int)AnimTime::Quotient(time, AnimTime(AnimTime::Duration::ONE_SECOND)),
			(int)AnimTime::Quotient(AnimTime::Remainder(time, AnimTime(AnimTime::Duration::ONE_SECOND)), AnimTime(AnimTime::Duration::ONE_MILLISECOND)));
	}

	void TerrainMesh::GenerateGrid(const ColorType* pColorInput, float* pInSurfaceWeights)
	{
		pVertexGrid = new Vec3f[numVerts];
		pIndicies = new unsigned int[numIndices];
		pTextureCoordinates = new Vec2f[numVerts];

		pHeightfieldTexture = new float[numVerts];

		pSurfaceWeights = new float[GetSurfaceWeightsSize()];

		pTerrainFoliage = new TerrainFoliage();

		const Vec3 center(0.f, INITIAL_HEIGHT, 0.f);
		const float halfWorldScale = worldScale / 2.f;
		const float step = worldScale / static_cast<float>(vertexResolution - 1);
		Vec3 currentPos = center - Vec3(halfWorldScale, INITIAL_HEIGHT, halfWorldScale);

		AnimTimer tgen;
		tgen.Tic();

		for (unsigned int i = 0; i < vertexResolution; i++)
		{
			for (unsigned int j = 0; j < vertexResolution; j++)
			{
				const unsigned int gridIndex = GetGridIndex(i, j);
				float elevation = INITIAL_HEIGHT;

				if (pColorInput)
				{
					pHeightfieldTexture[GetGridIndex(j, i)] = (float)pColorInput[gridIndex] / (float)std::numeric_limits<ColorType>().max();
					elevation = GetElevationFromColor(pColorInput[GetGridIndex(i, j)]);
				}

				pVertexGrid[gridIndex] = Vec3f(currentPos.x(), elevation, currentPos.z());

				currentPos[x] += step;
			}

			currentPos[z] += step;
			currentPos[x] = -halfWorldScale;
		}

		AnimTime time = tgen.Toc();
		Trace::out("Built Terrain Verts in: %ds %dms\n",
			(int)AnimTime::Quotient(time, AnimTime(AnimTime::Duration::ONE_SECOND)),
			(int)AnimTime::Quotient(AnimTime::Remainder(time, AnimTime(AnimTime::Duration::ONE_SECOND)), AnimTime(AnimTime::Duration::ONE_MILLISECOND)));

		tgen.Tic();
		UpdateTextureCoordinates(INITIAL_TILING);
		time = tgen.Toc();
		Trace::out("Update Texture Coordinates in: %ds %dms\n",
			(int)AnimTime::Quotient(time, AnimTime(AnimTime::Duration::ONE_SECOND)),
			(int)AnimTime::Quotient(AnimTime::Remainder(time, AnimTime(AnimTime::Duration::ONE_SECOND)), AnimTime(AnimTime::Duration::ONE_MILLISECOND)));

		tgen.Tic();
		unsigned int indiciesIndex = 0;
		bool windingFlip = true;
		for (unsigned int i = 0; i < vertexResolution; i++)
		{
			for (unsigned int j = 0; j < vertexResolution; j++)
			{
				const bool isRightEdge = i == vertexResolution - 1;
				const bool isTopEdge = j == vertexResolution - 1;

				if (!(isTopEdge || isRightEdge))
				{
					const unsigned int gridIndex = GetGridIndex(i, j);

					if (windingFlip)
					{
						pIndicies[indiciesIndex++] = gridIndex;
						pIndicies[indiciesIndex++] = GetGridIndex(i + 1, j);
						pIndicies[indiciesIndex++] = GetGridIndex(i + 1, j + 1);

						pIndicies[indiciesIndex++] = gridIndex;
						pIndicies[indiciesIndex++] = GetGridIndex(i + 1, j + 1);
						pIndicies[indiciesIndex++] = GetGridIndex(i, j + 1);
					}
					else
					{
						pIndicies[indiciesIndex++] = gridIndex;
						pIndicies[indiciesIndex++] = GetGridIndex(i + 1, j);
						pIndicies[indiciesIndex++] = GetGridIndex(i, j + 1);

						pIndicies[indiciesIndex++] = GetGridIndex(i, j + 1);
						pIndicies[indiciesIndex++] = GetGridIndex(i + 1, j);
						pIndicies[indiciesIndex++] = GetGridIndex(i + 1, j + 1);
					}

					windingFlip = !windingFlip;
				}
			}
		}
		time = tgen.Toc();
		Trace::out("Created Indicies & Winding in: %ds %dms\n",
			(int)AnimTime::Quotient(time, AnimTime(AnimTime::Duration::ONE_SECOND)),
			(int)AnimTime::Quotient(AnimTime::Remainder(time, AnimTime(AnimTime::Duration::ONE_SECOND)), AnimTime(AnimTime::Duration::ONE_MILLISECOND)));

		pVertexBufferPosition = CreateVertexBuffer(sizeof(Vec3f) * numVerts, pVertexGrid);
		pIndexBufferTriangles = CreateIndexBuffer(sizeof(unsigned int) * numIndices, pIndicies);
		pVertexBufferColor = nullptr;
		pConstantBufferLightColor = CreateConstantBuffer(sizeof(Vec3));
		pConstantBufferLightPosition = CreateConstantBuffer(sizeof(Vec3));

		HRESULT hr;
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = vertexResolution;
		desc.Height = vertexResolution;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pHeightfieldTexture;
		initData.SysMemPitch = sizeof(float) * vertexResolution;

		hr = Engine::GetDevice()->CreateTexture2D(&desc, &initData, &pTexture);
		assert(SUCCEEDED(hr));

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = desc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = Engine::GetDevice()->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		assert(SUCCEEDED(hr));

		SetSurfaces(pInSurfaceWeights);
	}

	float* TerrainMesh::GetInitSurfaces()
	{
		uint chunkResolution = vertexResolution / SPLATTING_RESOLUTION;
		uint paintableChunks = chunkResolution * chunkResolution;
		uint dataSize = paintableChunks * 4u;
		float* pInitialDataOnes = new float[dataSize];
		for (uint i = 0; i < dataSize; i += 4)
		{
			pInitialDataOnes[i] = 1.f;
			pInitialDataOnes[i + 1] = 0.0f;
			pInitialDataOnes[i + 2] = 0.0f;
			pInitialDataOnes[i + 3] = 0.0f;

			if (i < dataSize / 8)
			{
			}
			else if (i < dataSize / 4)
			{
				pInitialDataOnes[i + 1] = 0.25f;
			}
			else if (i < dataSize / 2)
			{
				pInitialDataOnes[i + 2] = 0.25f;
			}
		}

		return pInitialDataOnes;
	}

	void TerrainMesh::SetSurfaces(float* pInSurfaceWeights)
	{
		if (pInSurfaceWeights)
		{
			uint chunkResolution = vertexResolution / SPLATTING_RESOLUTION;

			HRESULT hr;
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = chunkResolution;
			desc.Height = chunkResolution;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = pInSurfaceWeights;
			initData.SysMemPitch = (sizeof(float) * 4u * chunkResolution);

			hr = Engine::GetDevice()->CreateTexture2D(&desc, &initData, &pSurfaceLayerWeights);
			assert(SUCCEEDED(hr));

			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			shaderResourceViewDesc.Format = desc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;
			hr = Engine::GetDevice()->CreateShaderResourceView(pSurfaceLayerWeights, &shaderResourceViewDesc, &pSurfaceLayerWeightsViews);
			assert(SUCCEEDED(hr));

			memcpy(pSurfaceWeights, pInSurfaceWeights, GetSurfaceWeightsSize());
		}
	}

	float TerrainMesh::GetElevationFromColor(ColorType color) const
	{
		constexpr static float maxColor = static_cast<float>(std::numeric_limits<ColorType>().max());
		const float percentageColor = static_cast<float>(color) / maxColor;
		return heightScale * percentageColor;
	}

	ColorType TerrainMesh::GetColorFromElevation(float elevation) const
	{
		constexpr static float maxColor = static_cast<float>(std::numeric_limits<ColorType>().max());
		const float percentageHeight = elevation / heightScale;
		return static_cast<ColorType>(percentageHeight * maxColor);
	}

	void TerrainMesh::EditVertex(uint index, const Vec3f& inValue)
	{
		pVertexGrid[index].x = inValue.x;
		pVertexGrid[index].y = std::max(inValue.y, 0.f);
		pVertexGrid[index].z = inValue.z;
	}

	void TerrainMesh::EditVertex(uint i, uint j, const Vec3f& inValue)
	{
		EditVertex(GetGridIndex(i, j), inValue);
	}

	void TerrainMesh::EditVertex(const TerrainCoordinate& coordinate, const Vec3f& inValue)
	{
		EditVertex(coordinate.x, coordinate.y, inValue);
	}

	void TerrainMesh::PushHeightfieldSlow()
	{
		Engine::GetContext()->UpdateSubresource(pTexture, 0, nullptr, pHeightfieldTexture, vertexResolution * sizeof(float), 0);
	}

	void TerrainMesh::PushSurfaceWeightsSlow()
	{
		Engine::GetContext()->UpdateSubresource(pSurfaceLayerWeights, 0, nullptr, pSurfaceWeights, GetSurfaceWeightsRowPitch(), 0);
	}
}