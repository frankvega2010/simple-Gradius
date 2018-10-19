#include "Player.h"

#include "Setup\Game.h"
#include "Screens\settings.h"
#include "Screens\gameplay.h"

namespace Juego
{
	rocketShip enemy01;

	namespace Gameplay_Section
	{
		void createEnemy()
		{
			enemy01.position = { (float)screenWidth + enemy01.size.x, (float)screenHeight / 2 };
			enemy01.position.y = GetRandomValue(0, screenHeight);
			enemy01.size = { 100, 100 };
			enemy01.defaultSpeed = 500.0f;
			enemy01.isAlive = true;
			enemy01.inputActive = false;
			enemy01.textureTint = YELLOW;
		}

		void EnemyUpdate()
		{
			enemy01.position.x -= player.defaultSpeed * GetFrameTime();

			if (enemy01.position.x < 0 - enemy01.size.x) 
			{
				enemy01.position.x = (float)screenWidth + enemy01.size.x;
				enemy01.position.y = GetRandomValue(0, screenHeight);
			}
		}

		void EnemyDraw()
		{
			DrawRectangle(enemy01.position.x, enemy01.position.y, enemy01.size.x, enemy01.size.y, enemy01.textureTint);
		}

	}
}