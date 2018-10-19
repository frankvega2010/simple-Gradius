#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "Setup\Player.h"

namespace Juego
{
	extern rocketShip enemy01;

	namespace Gameplay_Section
	{
		void createEnemy();
		void EnemyUpdate();
		void EnemyDraw();
	}
}
#endif // ENEMY_H
