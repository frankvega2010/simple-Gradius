#include "Enemy.h"

#include "Setup\Game.h"
#include "Screens\settings.h"
#include "Screens\gameplay.h"

namespace Juego
{
	rocketShip enemy01;

	//static 

	rocketShip enemies[maxEnemies];

	static Rectangle enemyShipSource = { 0.0f,0.0f, 180,70 };
	static Rectangle enemyShipDestination;

	static Rectangle enemyShipDestinations[maxEnemies];

	static Vector2 enemyShipOrigin = { 0,15 };

	namespace Gameplay_Section
	{
		void createEnemy()
		{
			for (int i = 0; i < maxEnemies;i++)
			{
				enemies[i].size = { 180, 45 };
				enemies[i].position.y = GetRandomValue(0, screenHeight - (enemies[i].size.y/2));
				enemies[i].position.x = GetRandomValue((float)screenWidth + enemies[i].size.x, (float)screenWidth + (enemies[i].size.x * 10));
				enemies[i].defaultSpeed = GetRandomValue(400,700);
				enemies[i].isAlive = true;
				enemies[i].inputActive = false;
				enemies[i].textureTint = { 0, 0, 0, 0 };
			}
			
		}

		void EnemyUpdate()
		{
			for (int i = 0; i < maxEnemies; i++)
			{
				enemies[i].position.x -= enemies[i].defaultSpeed * GetFrameTime();

				if (enemies[i].position.x < 0 - enemies[i].size.x || !enemies[i].isAlive)
				{
					enemies[i].isAlive = true;
					enemies[i].position.x = GetRandomValue((float)screenWidth + enemies[i].size.x, (float)screenWidth + (enemies[i].size.x * 10));
					enemies[i].position.y = GetRandomValue(0, screenHeight - (enemies[i].size.y / 2));
				}
			}
		}

		void EnemyDraw()
		{
			for (int i = 0; i < maxEnemies; i++)
			{
				enemyShipDestination = { enemies[i].position.x,enemies[i].position.y, 180,70 };

				if (enemies[i].isAlive)
				{
					DrawRectangle(enemies[i].position.x, enemies[i].position.y, enemies[i].size.x, enemies[i].size.y, enemies[i].textureTint);
					DrawTexturePro(enemyShip, enemyShipSource, enemyShipDestination, enemyShipOrigin, 0, WHITE);
				}
			}
			
		}

	}
}