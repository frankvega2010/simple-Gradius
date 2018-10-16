#ifndef PLAYERSHOOT_H
#define PLAYERSHOOT_H

#include "raylib.h"

namespace Juego
{
	struct Shoot {
		Vector2 position;
		Vector2 speed;
		float rotation;
		int radius;
		bool active;
		Color color;
	};

	extern int maxShoots;
	const int maxShootsSpecial = 30;

	extern Shoot shoots[maxShootsSpecial];

	extern float rapidFireTimer;
	extern float rapidFireRate;

	extern int gameScore;
	extern int destroyedAsteroidsCount;
	extern int asteroidsLeft;

	namespace Gameplay_Section
	{
		void createShoot();
		void shootInput();
		void shootUpdate();
		void shootDraw();
	}
}

#endif // PLAYERSHOOT_H
