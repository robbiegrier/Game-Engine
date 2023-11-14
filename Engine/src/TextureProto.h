#ifndef TEXTURE_PROTO_H
#define TEXTURE_PROTO_H

#include "TextureObject.h"
#include "azulModel.h"

namespace Azul
{
	// A texture loaded from an azul.proto file
	class TextureProto : public TextureObject
	{
	public:
		// Big four
		TextureProto() = delete;
		TextureProto(const TextureProto&) = delete;
		TextureProto& operator = (const TextureProto&) = delete;
		virtual ~TextureProto();
		TextureProto(const char* const pMeshFileName, unsigned int index = 0, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		TextureProto(const meshData& mB, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	private:
		void SetFromMeshData(const meshData& mB, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		ID3D11Texture2D* poTex;
	};
}

#endif
