#include "Player.h"
#include "GONull.h"
#include "CameraManager.h"
#include "Engine.h"
#include <math.h>
#include "Terrain.h"
#include "GameObjectManager.h"
#include "TerrainSystem.h"
#include "PlayerScriptFps.h"
#include "PlayerScriptAdventure.h"

namespace Azul
{
	Player::Player()
		: GameObject(new GONull())
	{
		typeName = Name::Player;
		alwaysRenderShell = true;

		SetShellColor(Vec4(0.f, .8f, .8f, 1.f));

		pScript = new PlayerScriptAdventure();
		pScript->SetPlayer(this);
	}

	Player::~Player()
	{
		delete pScript;
	}

	void Player::Start()
	{
		pScript->Start();
	}

	void Player::Tick(float deltaTime)
	{
		pScript->Update(deltaTime);
	}

	void Player::Draw()
	{
		GameObject::Draw();

		pScript->Draw();
	}
}