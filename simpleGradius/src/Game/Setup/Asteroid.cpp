#include "Asteroid.h"

#include <cmath>

#include "Setup/Game.h"
#include "Setup/Player.h"
#include "Screens/gameplay.h"
#include "Screens/settings.h"

namespace Juego
{
	int asteroidsSpeed = 0;
	static float posX = 0;
	static float posY = 0;
	static float speedX = 0;
	static float speedY = 0;
	static bool correctRange = false;
	static float rotationTimerBigAsteroids = 0;
	static float rotationTimerMediumAsteroids = 0;
	static float rotationTimerSmallAsteroids = 0;

	static const int maxTimer = 1000;
	static float explosionTimer = 0;
	static bool timerExplosionON = true;
	
	static Rectangle bigAsteroidSource = { 0.0f,0.0f, 90,90 };
	static Rectangle mediumAsteroidSource = { 0.0f,0.0f, 45,45 };
	static Rectangle smallAsteroidSource = { 0.0f,0.0f, 22.5f,22.5f };

	static Rectangle bigAsteroidSourceSmall = { 0.0f,0.0f, 90/2,90/2 };
	static Rectangle mediumAsteroidSourceSmall = { 0.0f,0.0f, 45/2,45/2 };
	static Rectangle smallAsteroidSourceSmall = { 0.0f,0.0f, 22.5f/2,22.5f/2 };

	static Rectangle bigAsteroidDestination;
	static Rectangle mediumAsteroidDestination;
	static Rectangle smallAsteroidDestination;

	static Rectangle bigAsteroidDestinationSmall;
	static Rectangle mediumAsteroidDestinationSmall;
	static Rectangle smallAsteroidDestinationSmall;

	static Vector2 bigAsteroidOrigin = { 45,45 };
	static Vector2 mediumAsteroidOrigin = { 22.5f,22.5f };
	static Vector2 smallAsteroidOrigin = { 11.25f,11.25f };

	static Vector2 bigAsteroidOriginSmall = { 45/2,45/2 };
	static Vector2 mediumAsteroidOriginSmall = { 22.5f/2,22.5f/2 };
	static Vector2 smallAsteroidOriginSmall = { 11.25f/2,11.25f/2 };

	static Rectangle explosionSpriteNormal = { 0.0f,0.0f, 50,50 };
	static Rectangle explosionSpriteSmall = { 0.0f,0.0f, 50/2,50/2 };
	static Rectangle explosionSprite;

	static const float ExplosionFadeAmount = 0.005;
	static const int asteroidMaxExplosionSize = 100;
	static const int asteroidBigMaxExplosionSize = 150;
	
	static const int defaultAsteroidBigRotationSpeed = 40;
	static const int defaultAsteroidMediumRotationSpeed = 60;
	static const int defaultAsteroidSmallRotationSpeed = 80;


	Asteroid asteroidsSmall[asteroidsSmallLimit];
	Asteroid asteroidsMedium[asteroidsMediumLimit];
	Asteroid asteroidsBig[asteroidsBigLimit];

	namespace Gameplay_Section
	{
		void createAsteroid()
		{
			if (resolutionNormal && !(resolutionBig))
			{
				asteroidsSpeed = 200;
				explosionSprite = explosionSpriteNormal;
			}
			else if (resolutionSmall)
			{
				asteroidsSpeed = 100;
				explosionSprite = explosionSpriteSmall;
			}
			else if (resolutionNormal && resolutionBig)
			{
				asteroidsSpeed = 250;
				explosionSprite = explosionSpriteNormal;
			}

			rotationTimerBigAsteroids = 0;
			rotationTimerMediumAsteroids = 0;
			rotationTimerSmallAsteroids = 0;

			for (int i = 0; i < asteroidsBigLimit; i++)
			{
				posX = GetRandomValue(0, screenWidth);

				while (!correctRange)
				{
					if (posX > screenWidth / 2 - 150 && posX < screenWidth / 2 + 150) posX = GetRandomValue(0, screenWidth);
					else correctRange = true;
				}

				correctRange = false;

				posY = GetRandomValue(0, screenHeight);

				while (!correctRange)
				{
					if (posY > screenHeight / 2 - 150 && posY < screenHeight / 2 + 150)  posY = GetRandomValue(0, screenHeight);
					else correctRange = true;
				}

				asteroidsBig[i].position = { posX, posY };

				correctRange = false;
				speedX = GetRandomValue(-asteroidsSpeed, asteroidsSpeed);
				speedY = GetRandomValue(-asteroidsSpeed, asteroidsSpeed);

				while (!correctRange)
				{
					if (speedX == 0 && speedY == 0)
					{
						speedX = GetRandomValue(-asteroidsSpeed, asteroidsSpeed);
						speedY = GetRandomValue(-asteroidsSpeed, asteroidsSpeed);
					}
					else correctRange = true;
				}

				asteroidsBig[i].speed = { speedX, speedY};

				if (resolutionNormal) asteroidsBig[i].radius = 40;
				else if (resolutionSmall) asteroidsBig[i].radius = 20;

				asteroidsBig[i].active = true;
				asteroidsBig[i].isExplosionActive = false;
				asteroidsBig[i].increasingExplosionSize = 0;
				asteroidsBig[i].increasingExplosionFade = 1;
			}


			for (int i = 0; i < asteroidsMediumLimit; i++)
			{
				asteroidsMedium[i].position = { -100, -100 };
				asteroidsMedium[i].speed = { 0, 0 };

				if (resolutionNormal) asteroidsMedium[i].radius = 20;
				else if (resolutionSmall) asteroidsMedium[i].radius = 10;

				asteroidsMedium[i].active = false;
				asteroidsMedium[i].isExplosionActive = false;
				asteroidsMedium[i].increasingExplosionSize = 0;
				asteroidsMedium[i].increasingExplosionFade = 1;
			}

			for (int i = 0; i < asteroidsSmallLimit; i++)
			{
				asteroidsSmall[i].position = { -100, -100 };
				asteroidsSmall[i].speed = { 0, 0 };

				if (resolutionNormal) asteroidsSmall[i].radius = 10;
				else if (resolutionSmall) asteroidsSmall[i].radius = 5;

				asteroidsSmall[i].active = false;
				asteroidsSmall[i].isExplosionActive = false;
				asteroidsSmall[i].increasingExplosionSize = 0;
				asteroidsSmall[i].increasingExplosionFade = 1;
			}

		}

		void asteroidUpdate()
		{
			if (timerExplosionON)
			{
				explosionTimer += 1 * GetFrameTime();
			}

			if (explosionTimer > 0.01)
			{
				explosionTimer = 0;
				for (int i = 0; i < asteroidsBigLimit; i++)
				{
					if (asteroidsBig[i].isExplosionActive)
					{
						asteroidsBig[i].increasingExplosionSize++;
						asteroidsBig[i].increasingExplosionFade = asteroidsBig[i].increasingExplosionFade - ExplosionFadeAmount;
					}

					if (asteroidsBig[i].increasingExplosionSize >= asteroidBigMaxExplosionSize)
					{
						asteroidsBig[i].isExplosionActive = false;
					}
				}

				for (int i = 0; i < asteroidsMediumLimit; i++)
				{
					if (asteroidsMedium[i].isExplosionActive)
					{
						asteroidsMedium[i].increasingExplosionSize++;
						asteroidsMedium[i].increasingExplosionFade = asteroidsMedium[i].increasingExplosionFade - ExplosionFadeAmount;
					}

					if (asteroidsMedium[i].increasingExplosionSize >= asteroidMaxExplosionSize)
					{
						asteroidsMedium[i].isExplosionActive = false;
					}
				}

				for (int i = 0; i < asteroidsSmallLimit; i++)
				{
					if (asteroidsSmall[i].isExplosionActive)
					{
						asteroidsSmall[i].increasingExplosionSize++;
						asteroidsSmall[i].increasingExplosionFade = asteroidsSmall[i].increasingExplosionFade - ExplosionFadeAmount;
					}

					if (asteroidsSmall[i].increasingExplosionSize >= asteroidMaxExplosionSize)
					{
						asteroidsSmall[i].isExplosionActive = false;
					}
				}
			}

			if (rotationTimerBigAsteroids < maxTimer) rotationTimerBigAsteroids += defaultAsteroidBigRotationSpeed * GetFrameTime();
			else rotationTimerBigAsteroids = 0;

			if (rotationTimerMediumAsteroids < maxTimer) rotationTimerMediumAsteroids += defaultAsteroidMediumRotationSpeed * GetFrameTime();
			else rotationTimerMediumAsteroids = 0;

			if (rotationTimerSmallAsteroids < maxTimer) rotationTimerSmallAsteroids += defaultAsteroidSmallRotationSpeed * GetFrameTime();
			else rotationTimerSmallAsteroids = 0;
			
			// Meteors logic: big meteors
			for (int i = 0; i < asteroidsBigLimit; i++)
			{
				if (asteroidsBig[i].active)
				{
					// Movement
					asteroidsBig[i].position.x += asteroidsBig[i].speed.x * GetFrameTime();
					asteroidsBig[i].position.y += asteroidsBig[i].speed.y * GetFrameTime();

					// Collision logic: meteor vs wall
					if (asteroidsBig[i].position.x > screenWidth + asteroidsBig[i].radius) asteroidsBig[i].position.x = -(asteroidsBig[i].radius);
					else if (asteroidsBig[i].position.x < 0 - asteroidsBig[i].radius) asteroidsBig[i].position.x = screenWidth + asteroidsBig[i].radius;
					if (asteroidsBig[i].position.y > screenHeight + asteroidsBig[i].radius) asteroidsBig[i].position.y = -(asteroidsBig[i].radius);
					else if (asteroidsBig[i].position.y < 0 - asteroidsBig[i].radius) asteroidsBig[i].position.y = screenHeight + asteroidsBig[i].radius;
				}
			}

			// Meteors logic: medium meteors
			for (int i = 0; i < asteroidsMediumLimit; i++)
			{
				if (asteroidsMedium[i].active)
				{
					// Movement
					asteroidsMedium[i].position.x += asteroidsMedium[i].speed.x * GetFrameTime();
					asteroidsMedium[i].position.y += asteroidsMedium[i].speed.y * GetFrameTime();

					// Collision logic: meteor vs wall
					if (asteroidsMedium[i].position.x > screenWidth + asteroidsMedium[i].radius) asteroidsMedium[i].position.x = -(asteroidsMedium[i].radius);
					else if (asteroidsMedium[i].position.x < 0 - asteroidsMedium[i].radius) asteroidsMedium[i].position.x = screenWidth + asteroidsMedium[i].radius;
					if (asteroidsMedium[i].position.y > screenHeight + asteroidsMedium[i].radius) asteroidsMedium[i].position.y = -(asteroidsMedium[i].radius);
					else if (asteroidsMedium[i].position.y < 0 - asteroidsMedium[i].radius) asteroidsMedium[i].position.y = screenHeight + asteroidsMedium[i].radius;
				}
			}

			// Meteors logic: small meteors
			for (int i = 0; i < asteroidsSmallLimit; i++)
			{
				if (asteroidsSmall[i].active)
				{
					// Movement
					asteroidsSmall[i].position.x += asteroidsSmall[i].speed.x * GetFrameTime();
					asteroidsSmall[i].position.y += asteroidsSmall[i].speed.y * GetFrameTime();

					// Collision logic: meteor vs wall
					if (asteroidsSmall[i].position.x > screenWidth + asteroidsSmall[i].radius)asteroidsSmall[i].position.x = -(asteroidsSmall[i].radius);
					else if (asteroidsSmall[i].position.x < 0 - asteroidsSmall[i].radius)asteroidsSmall[i].position.x = screenWidth + asteroidsSmall[i].radius;
					if (asteroidsSmall[i].position.y > screenHeight + asteroidsSmall[i].radius)asteroidsSmall[i].position.y = -(asteroidsSmall[i].radius);
					else if (asteroidsSmall[i].position.y < 0 - asteroidsSmall[i].radius)asteroidsSmall[i].position.y = screenHeight + asteroidsSmall[i].radius;
				}
			}
		}

		void asteroidDraw()
		{
			// Draw meteors
			for (int i = 0; i < asteroidsBigLimit; i++)
			{
				bigAsteroidDestination = { asteroidsBig[i].position.x,asteroidsBig[i].position.y, 90,90 };
				bigAsteroidDestinationSmall = { asteroidsBig[i].position.x,asteroidsBig[i].position.y, 90/2,90/2 };

				if (asteroidsBig[i].active)
				{
					DrawCircleV(asteroidsBig[i].position, asteroidsBig[i].radius, DARKGRAY);

					if (resolutionNormal) DrawTexturePro(asteroid, bigAsteroidSource, bigAsteroidDestination, bigAsteroidOrigin, rotationTimerBigAsteroids, BLUE);
					else if (resolutionSmall) DrawTexturePro(asteroid, bigAsteroidSourceSmall, bigAsteroidDestinationSmall, bigAsteroidOriginSmall, rotationTimerBigAsteroids, BLUE);
				}
				else 
				{
					DrawCircleV(asteroidsBig[i].position, asteroidsBig[i].radius, Fade(LIGHTGRAY, 0.3f));

					if (resolutionNormal) DrawTexturePro(asteroid, bigAsteroidSource, bigAsteroidDestination, bigAsteroidOrigin, 0, DARKGRAY);
					else if (resolutionSmall) DrawTexturePro(asteroid, bigAsteroidSourceSmall, bigAsteroidDestinationSmall, bigAsteroidOriginSmall, 0, DARKGRAY);
				} 
			}

			for (int i = 0; i < asteroidsMediumLimit; i++)
			{
				mediumAsteroidDestination = { asteroidsMedium[i].position.x,asteroidsMedium[i].position.y, 45,45 };
				mediumAsteroidDestinationSmall = { asteroidsMedium[i].position.x,asteroidsMedium[i].position.y, 45/2,45/2 };

				if (asteroidsMedium[i].active)
				{
					DrawCircleV(asteroidsMedium[i].position, asteroidsMedium[i].radius, GRAY);

					if (resolutionNormal) DrawTexturePro(asteroidMedium, mediumAsteroidSource, mediumAsteroidDestination, mediumAsteroidOrigin, rotationTimerMediumAsteroids, BLUE);
					else if (resolutionSmall) DrawTexturePro(asteroidMedium, mediumAsteroidSourceSmall, mediumAsteroidDestinationSmall, mediumAsteroidOriginSmall, rotationTimerMediumAsteroids, BLUE);
				}
				else
				{
					DrawCircleV(asteroidsMedium[i].position, asteroidsMedium[i].radius, Fade(LIGHTGRAY, 0.3f));

					if (resolutionNormal) DrawTexturePro(asteroidMedium, mediumAsteroidSource, mediumAsteroidDestination, mediumAsteroidOrigin, 0, DARKGRAY);
					else if (resolutionSmall) DrawTexturePro(asteroidMedium, mediumAsteroidSourceSmall, mediumAsteroidDestinationSmall, mediumAsteroidOriginSmall, 0, DARKGRAY);
				}
			}

			for (int i = 0; i < asteroidsSmallLimit; i++)
			{
				smallAsteroidDestination = { asteroidsSmall[i].position.x,asteroidsSmall[i].position.y, 22.5f,22.5f };
				smallAsteroidDestinationSmall = { asteroidsSmall[i].position.x,asteroidsSmall[i].position.y, 22.5f/2,22.5f/2 };

				if (asteroidsSmall[i].active)
				{
					DrawCircleV(asteroidsSmall[i].position, asteroidsSmall[i].radius, GRAY);

					if (resolutionNormal) DrawTexturePro(asteroidSmall, smallAsteroidSource, smallAsteroidDestination, smallAsteroidOrigin, rotationTimerSmallAsteroids, BLUE);
					else if (resolutionSmall) DrawTexturePro(asteroidSmall, smallAsteroidSourceSmall, smallAsteroidDestinationSmall, smallAsteroidOriginSmall, rotationTimerSmallAsteroids, BLUE);
				}
				else
				{
					DrawCircleV(asteroidsSmall[i].position, asteroidsSmall[i].radius, Fade(LIGHTGRAY, 0.3f));

					if (resolutionNormal) DrawTexturePro(asteroidSmall, smallAsteroidSource, smallAsteroidDestination, smallAsteroidOrigin, 0, DARKGRAY);
					else if (resolutionSmall) DrawTexturePro(asteroidSmall, smallAsteroidSourceSmall, smallAsteroidDestinationSmall, smallAsteroidOriginSmall, 0, DARKGRAY);
				}
			}

			//EXPLOSION SPRITES DRAW
			for (int i = 0; i < asteroidsBigLimit; i++)
			{
				if (asteroidsBig[i].isExplosionActive && gameON)
				{
					if(resolutionNormal) DrawTexturePro(asteroidExplosion, explosionSprite, { asteroidsBig[i].position.x,asteroidsBig[i].position.y, asteroidsBig[i].increasingExplosionSize,asteroidsBig[i].increasingExplosionSize }, { 0,0 }, 0, Fade(RED, asteroidsBig[i].increasingExplosionFade));
					else if(resolutionSmall) DrawTexturePro(asteroidExplosion, explosionSpriteSmall, { asteroidsBig[i].position.x,asteroidsBig[i].position.y, asteroidsBig[i].increasingExplosionSize,asteroidsBig[i].increasingExplosionSize }, { 0,0 }, 0, Fade(RED, asteroidsBig[i].increasingExplosionFade));
				}
			}

			for (int i = 0; i < asteroidsMediumLimit; i++)
			{
				if (asteroidsMedium[i].isExplosionActive && gameON)
				{
					if(resolutionNormal) DrawTexturePro(asteroidExplosion, explosionSprite, { asteroidsMedium[i].position.x,asteroidsMedium[i].position.y, asteroidsMedium[i].increasingExplosionSize,asteroidsMedium[i].increasingExplosionSize }, { 0,0 }, 0, Fade(RED, asteroidsMedium[i].increasingExplosionFade));
					else if(resolutionSmall) DrawTexturePro(asteroidExplosion, explosionSpriteSmall, { asteroidsMedium[i].position.x,asteroidsMedium[i].position.y, asteroidsMedium[i].increasingExplosionSize,asteroidsMedium[i].increasingExplosionSize }, { 0,0 }, 0, Fade(RED, asteroidsMedium[i].increasingExplosionFade));
					
				}
			}

			for (int i = 0; i < asteroidsSmallLimit; i++)
			{
				if (asteroidsSmall[i].isExplosionActive && gameON)
				{
					if (resolutionNormal) DrawTexturePro(asteroidExplosion, explosionSprite, { asteroidsSmall[i].position.x,asteroidsSmall[i].position.y, asteroidsSmall[i].increasingExplosionSize,asteroidsSmall[i].increasingExplosionSize }, { 0,0 }, 0, Fade(RED, asteroidsSmall[i].increasingExplosionFade));
					else if (resolutionSmall) DrawTexturePro(asteroidExplosion, explosionSpriteSmall, { asteroidsSmall[i].position.x,asteroidsSmall[i].position.y, asteroidsSmall[i].increasingExplosionSize,asteroidsSmall[i].increasingExplosionSize }, { 0,0 }, 0, Fade(RED, asteroidsSmall[i].increasingExplosionFade));
				}
			}
		}
	}
}