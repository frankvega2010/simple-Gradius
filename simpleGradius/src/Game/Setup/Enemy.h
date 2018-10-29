#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "Setup\Player.h"

namespace Juego
{
	const int maxEnemies = 5;

	extern rocketShip enemy01;
	extern rocketShip enemies[maxEnemies];

	namespace Gameplay_Section
	{
		void createEnemy();
		void EnemyUpdate();
		void EnemyDraw();
	}
}
#endif // ENEMY_H
