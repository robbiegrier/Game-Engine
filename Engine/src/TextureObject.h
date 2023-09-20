#ifndef _TextureObject
#define _TextureObject

#include "DLink.h"
#include <d3d11.h>

namespace Azul
{
	// A Texture Object encapsulates a 2D texture file and can be activated
	// to decorate a piece of geometry.
	class TextureObject : public DLink
	{
	public:
		enum class Name
		{
			NullTexture,
			Duckweed,
			Brick,
			Stone,
			Rocks,
			None
		};

		// Big four
		TextureObject(TextureObject::Name _name, LPCWSTR filepath, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);
		TextureObject();
		TextureObject(const TextureObject&) = delete;
		TextureObject& operator = (const TextureObject&) = delete;
		virtual ~TextureObject();

		// Called when used in rendering
		void ActivateTexture();

		// Ref counting
		int GetRefCount() const;
		void IncrementRefCount();
		void DecrementRefCount();

		// As a DLink node
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;
		Name GetName() const;
		void SetName(TextureObject::Name _name);
		const char* NameToString();

	private:
		TextureObject::Name name;
		ID3D11ShaderResourceView* poTextureRV;
		ID3D11SamplerState* poSampler;

		int TexResourceSlot;
		int SamplerSlot;
		int refCount = 0;
	};
}

#endif