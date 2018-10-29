#ifndef PLAYERSHOOT_H
#define PLAYERSHOOT_H

#include "raylib.h"

namespace Juego
{
	struct Shoot {
		Vector2 position;
		Vector2 size;
		Vector2 speed;
		//float rotation;
		//int radius;
		bool active;
		Color color;
	};

	const int maxShoots = 10;
	//const int maxShootsSpecial = 30;

	extern Shoot shoots[maxShoots];
	extern Shoot shoots2[maxShoots];

	extern float rapidFireTimer;
	extern float rapidFireRate;

	extern int gameScore;
	extern int destroyedAsteroidsCount;
	extern int targetsLeft;

	namespace Gameplay_Section
	{
		void createShoot();
		void shootInput();
		void shootUpdate();
		void shootDraw();
	}
}

#endif // PLAYERSHOOT_H
