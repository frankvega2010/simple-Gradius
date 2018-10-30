#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

namespace Juego
{
	struct rocketShip
	{
		Vector2 position;
		Vector2 size;
		int rotation;
		float defaultSpeed;
		bool isAlive;
		bool inputActive;
		Color textureTint;
	};

	struct Circle {
		Vector2 position;
		Vector2 speed;
		int radius;
		bool active;
	};

	extern rocketShip player;

	namespace Gameplay_Section
	{
		void createPlayer();
		void playerInput();
		void playerUpdate();
		void playerDraw();
	}
}
#endif // PLAYER_H
