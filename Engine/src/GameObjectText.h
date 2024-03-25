#ifndef _GameObjectText
#define _GameObjectText

#include "GameObjectSprite.h"

namespace Azul
{
	class Font;

	class GameObjectText : public GameObjectSprite
	{
	public:
		static constexpr unsigned int MaxMessageSize = 256;

		// Big four
		GameObjectText(GraphicsObject* graphicsObject, Font* pInFont);
		GameObjectText(Font* pInFont, const char* inMessage);
		GameObjectText() = delete;
		GameObjectText(const GameObjectText&) = delete;
		GameObjectText& operator=(GameObjectText&) = delete;
		virtual ~GameObjectText() = default;

		void SetMessage(const char* inMessage);

		virtual void Draw() override;

		Font* GetFont() const { return pFont; }
		const char* GetMessagePtr() { return message; }

	private:
		Font* pFont;
		char message[MaxMessageSize];
	};
}

#endif