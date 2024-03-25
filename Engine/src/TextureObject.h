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
			Desert,
			Crate,
			Frigate,
			Bracket,
			Fish,
			DogHouse,
			Duck,
			R2D2,
			Corset,
			AntiqueCameraTripod,
			AntiqueCamera,
			Dog,
			WesternTownHouse,
			WesternTownHouse1,
			DesertRock0,
			DesertRock1,
			DesertRock2,
			DesertRock3,
			DesertRock4,
			DesertRock5,
			DesertRock6,
			DesertRock7,
			DesertRock8,
			DesertRock9,
			DesertRock10,
			DesertRock11,
			DesertRock12,
			DesertRock13,
			Tattoine0,
			Tattoine1,
			Tattoine2,
			Tattoine3,
			Tattoine4,
			Tattoine5,
			Tattoine6,
			Tattoine7,
			Tattoine8,
			Tattoine9,
			Tattoine10,
			Tattoine11,
			Tattoine12,
			Tattoine13,
			Tattoine14,
			Tattoine15,
			Tattoine16,
			Tattoine17,
			Tattoine18,
			Tattoine19,
			Tattoine20,
			Tattoine21,
			Tattoine22,
			Tattoine23,
			Tattoine24,
			Tattoine25,
			Tattoine26,
			Tattoine27,
			Tattoine28,
			Tattoine29,
			Tattoine30,
			Tattoine31,
			Tattoine32,
			Tattoine33,
			Tattoine34,
			Tattoine35,
			Tattoine36,
			Tattoine37,
			Tattoine38,
			Tattoine39,
			Bone,
			Bone_001,
			Bone_L,
			Bone_L_001,
			Bone_L_002,
			Bone_R,
			Bone_R_001,
			Bone_R_002,
			ChickenBot,
			Red,
			Blue,
			Yellow,
			Green,
			Hovered,
			Disabled,
			Birds,
			CenturyFont,
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

		ID3D11ShaderResourceView* GetResourceView() const { return poTextureRV; }
		int GetWidth() const { return width; }
		int GetHeight() const { return height; }

	protected:
		TextureObject::Name name;
		ID3D11ShaderResourceView* poTextureRV;
		ID3D11SamplerState* poSampler;

		int TexResourceSlot;
		int SamplerSlot;
		int refCount = 0;
		int width;
		int height;
	};
}

#endif