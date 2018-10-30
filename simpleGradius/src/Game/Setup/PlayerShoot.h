#ifndef PLAYERSHOOT_H
#define PLAYERSHOOT_H

#include "raylib.h"

namespace Juego
{
	struct Shoot {
		Vector2 position;
		Vector2 size;
		Vector2 speed;
		bool active;
		Color color;
	};

	const int maxShoots = 10;

	extern Shoot shoots[maxShoots];
	extern Shoot shoots2[maxShoots];

	extern float rapidFireTimer;
	extern float rapidFireRate;

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
