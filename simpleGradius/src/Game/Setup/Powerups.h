#ifndef POWERUPS_H
#define POWERUPS_H

#include "raylib.h"

namespace Juego
{
	enum PowerupsNames
	{
		Invincibility,
		MaxRapidFire
	};

	struct Powerup {
		Vector2 position;
		Vector2 speed;
		int radius;
		bool activated;
		bool onScreen;
		float useTimer;
		int useCountdown;
		float spawnTimer;
		int spawnCountdown;
	};

	const int maxPowerups = 2;
	extern Powerup powerups[maxPowerups];

	namespace Gameplay_Section
	{
		void createPowerups();
		void powerupsUpdate();
		void powerupsDraw();
	}
}

#endif //POWERUPS_H