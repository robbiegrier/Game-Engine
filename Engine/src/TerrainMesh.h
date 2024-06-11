#ifndef _TerrainMesh
#define _TerrainMesh

#include "Mesh.h"
#include "TerrainUtils.h"
#include "terrain.pb.h"

namespace Azul
{
	class TextureObject;
	class TerrainFoliage;

	// Holds all data that defines a particular terrain mesh. Can be fully serialized to a file to save the
	// state of a Terrain. Contains the metadata, heightmap, surface weights, and foliage descriptions.
	class TerrainMesh : public Mesh
	{
	public:
		// Big four
		TerrainMesh();
		TerrainMesh(const char* pInName, float inWorldScale, unsigned int inVertexResolution);
		TerrainMesh(const char* pInName, float inWorldScale, float inHeight, const char* pHeightmapFileName);
		TerrainMesh(Mesh&) = delete;
		TerrainMesh& operator=(TerrainMesh&) = delete;
		virtual ~TerrainMesh();

		// Accessors
		float GetWorldScale() const { return worldScale; }
		float GetHalfWorldScale() const { return worldScale * .5f; }
		float GetHeightScale() const { return heightScale; }
		float GetSingleGridCellScale() const { return worldScale / static_cast<float>(vertexResolution); }
		unsigned int GetVertexResolution() const { return vertexResolution; }
		unsigned int GetTotalVertices() const { return vertexResolution * vertexResolution; }
		Vec3f* GetVertexGrid() const { return pVertexGrid; }
		float* GetHeightfieldTexture() const { return pHeightfieldTexture; }
		uint GetHeightfieldTextureSize() const { return sizeof(float) * numVerts; }
		Vec3 GetVertex(unsigned int i, unsigned int j) const;
		const Vec3f& GetVertexf(unsigned int i, unsigned int j) const;
		const Vec3f& GetVertexf(unsigned int i) const;
		unsigned int GetGridIndex(unsigned int i, unsigned int j) const;
		const char* GetTerrainMeshName() const { return terrainName; }
		ID3D11ShaderResourceView* GetView() const { return pShaderResourceView; }
		ID3D11Texture2D* GetHeightfieldTextureResource() const { return pTexture; }

		// Serialize and deserialize from a protobuf file
		void Serialize(terrain_proto& out) const;
		void Deserialize(const terrain_proto& in);

		// Edit verticies
		void EditVertex(uint index, const Vec3f& inValue);
		void EditVertex(uint i, uint j, const Vec3f& inValue);
		void EditVertex(const TerrainCoordinate& coordinate, const Vec3f& inValue);

		// Change the texture tiling
		void UpdateTextureCoordinates(float tiling);

		// Used to update the terrain outside of brushes (ie undo, redo)
		void PushHeightfieldSlow();
		void PushSurfaceWeightsSlow();

		// Conversions between color type and float height value
		float GetElevationFromColor(ColorType color) const;
		ColorType GetColorFromElevation(float elevation) const;

		// Surface weights accessors
		float* GetSurfaceWeights() const { return pSurfaceWeights; }
		ID3D11Texture2D* GetSurfaceWeightsTexture() const { return pSurfaceLayerWeights; }
		ID3D11ShaderResourceView* GetSurfaceWeightsView() const { return pSurfaceLayerWeightsViews; }
		uint GetSurfaceWeightsTextureResolution() const { return vertexResolution / SPLATTING_RESOLUTION; }
		uint GetNumSurfaceWeightFloats() const { return GetSurfaceWeightsTextureResolution() * GetSurfaceWeightsTextureResolution() * 4u; }
		uint GetSurfaceWeightsSize() const { return GetSurfaceWeightsTextureResolution() * GetSurfaceWeightsTextureResolution() * sizeof(float) * 4u; }
		uint GetSurfaceWeightsRowPitch() const { return GetSurfaceWeightsTextureResolution() * sizeof(float) * 4u; }
		TextureObject* GetSurfaceTexture(uint index) const { return pSurfaceLayerTextures[index]; }
		void SetSurfaceTexture(uint index, TextureObject* pTex) { pSurfaceLayerTextures[index] = pTex; }

		// Accessor for foliage component
		TerrainFoliage* GetFoliage() const { return pTerrainFoliage; }

		// Public constants
		static constexpr uint NAME_SIZE = 32;
		static constexpr uint NUM_SURFACES = 4u;
		static constexpr uint SURFACES_WEIGHTS_REGISTER = 2u;
		static constexpr uint SURFACES_TEXTURE_REGISTER = 6u;
		static constexpr uint SURFACES_NORMALS_REGISTER = 10u;

	private:
		void GenerateGrid(const ColorType* pColorInput, float* pInSurfaceWeights);

		float* GetInitSurfaces();
		void SetSurfaces(float* pInSurfaceWeights);

		float worldScale;
		float heightScale;
		unsigned int vertexResolution;

		Vec3f* pVertexGrid;
		unsigned int* pIndicies;
		Vec2f* pTextureCoordinates;

		float* pHeightfieldTexture;
		ID3D11Texture2D* pTexture;
		ID3D11ShaderResourceView* pShaderResourceView;

		float* pSurfaceWeights;
		ID3D11Texture2D* pSurfaceLayerWeights;
		ID3D11ShaderResourceView* pSurfaceLayerWeightsViews;
		TextureObject* pSurfaceLayerTextures[NUM_SURFACES];

		TerrainFoliage* pTerrainFoliage;

		char terrainName[NAME_SIZE];

	private:
		constexpr static float INITIAL_HEIGHT = 0.f;
		constexpr static float INITIAL_TILING = 8.f;
		constexpr static uint SPLATTING_RESOLUTION = 4u;
	};
}

#endif
