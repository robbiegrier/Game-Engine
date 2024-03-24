#include "Image.h"
#include "TextureObjectManager.h"

namespace Azul
{
	Image::Image()
	{
		imageName = Image::Name::Null;
		imageRect.Clear();
		pText = nullptr;
	}

	Image::Image(Image::Name _imageName, TextureObject* _pText, Rect _rect)
	{
		imageName = _imageName;
		pText = _pText;
		imageRect = _rect;
	}

	Image::Image(Image::Name _imageName, TextureObject::Name _textName, Rect _rect)
	{
		imageName = _imageName;
		pText = TextureObjectManager::Find(_textName);
		assert(pText);
		imageRect = _rect;
	}

	void Image::Set(Image::Name _imageName, TextureObject* _pText, Rect _rect)
	{
		imageName = _imageName;
		pText = _pText;
		imageRect = _rect;
	}

	void Image::SetTexture(TextureObject* pTexture)
	{
		pText = pTexture;
	}

	void Image::SetRect(Rect rect)
	{
		imageRect = rect;
	}

	void Image::Set(Image::Name _imageName, TextureObject::Name _textName, Rect _rect)
	{
		imageName = _imageName;
		pText = TextureObjectManager::Find(_textName);
		assert(pText);
		imageRect = _rect;
	}

	bool Image::Compare(DLink* pTargetNode)
	{
		Image* pOther = (Image*)pTargetNode;
		return pOther->GetName() == GetName();
	}

	void Image::Dump()
	{
		Trace::out("%s\n", NameToString());
	}

	void Image::Wash()
	{
		imageName = Image::Name::Null;
		imageRect.Clear();
		pText = nullptr;
	}

	const char* Image::NameToString()
	{
		switch (imageName)
		{
		case Name::ALLBirds:
			return "ALLBirds";
		case Name::GreenBird:
			return "GreenBird";
		case Name::WhiteBird:
			return "WhiteBird";
		case Name::YellowBird:
			return "YellowBird";
		case Name::Stitch:
			return "Stitch";
		case Name::Null:
			return "Null";
		default:
			return "None";
		}
	}

	void Image::SetName(Image::Name inName)
	{
		imageName = inName;
	}

	Image::Name Image::GetName() const
	{
		return imageName;
	}
}