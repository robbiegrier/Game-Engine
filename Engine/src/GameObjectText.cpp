#include "GameObjectText.h"
#include "Font.h"
#include "Glyph.h"
#include "GOSprite.h"
#include "ImageManager.h"

Azul::GameObjectText::GameObjectText(GraphicsObject* graphicsObject, Font* pInFont)
	: GameObjectSprite(graphicsObject), pFont(pInFont)
{
	typeName = GameObject::Name::GameObjectText;

	memset(message, 0, MaxMessageSize);

	GOSprite* pSprite = (GOSprite*)pGraphicsObject;

	pSprite->OverwriteImageProperties(pFont->GetTexture(), Rect());

	SetMessage("Call SetMessage() to change this message!");
}

Azul::GameObjectText::GameObjectText(Font* pInFont, const char* inMessage)
	: GameObjectText(new GOSprite(ImageManager::Find(Image::Name::Text), Rect(0, 0, 0, 0)), pInFont)
{
	SetMessage(inMessage);
}

void Azul::GameObjectText::SetMessage(const char* inMessage)
{
	memset(message, 0, MaxMessageSize);
	memcpy(message, inMessage, MaxMessageSize);
}

void Azul::GameObjectText::Draw()
{
	GOSprite* pSprite = (GOSprite*)pGraphicsObject;

	Vec3 spriteTmpPos = GetRelativeLocation();

	const float spacing = 1.f;

	for (int i = 0; i < (int)MaxMessageSize && message[i] != '\0'; i++)
	{
		char character = message[i];
		Glyph* pGlyph = pFont->GetGlyph(character);

		pSprite->OverwriteImageProperties(pFont->GetTexture(), pGlyph->GetRect());

		spriteTmpPos += Vec3(pGlyph->GetRect().width / 2, 0.f, 0.f);
		Vec3 spriteTmpScale = Vec3(pGlyph->GetRect().width, pGlyph->GetRect().height, 1.f);

		Trans t(spriteTmpPos);
		Scale s(spriteTmpScale);
		Rot r = Rot(*pRotation);
		Mat4 spriteTmpWorld = s * r * t;

		pSprite->SetWorld(spriteTmpWorld);
		pSprite->Render();

		spriteTmpPos += Vec3(spacing + pGlyph->GetRect().width * 0.5f, 0.f, 0.f);
	}
}