#include "PlayerShoot.h"

#include "Setup/Powerups.h"
#include "Setup/Player.h"
#include "Setup/Game.h"
#include "Screens/gameplay.h"
#include "Screens/settings.h"
#include "Setup/Asteroid.h"

using namespace Juego;
using namespace Gameplay_Section;

namespace Juego
{
	int maxShoots = 10;

	Shoot shoots[maxShootsSpecial];
	
	float rapidFireTimer = 0;
	float rapidFireRate = 0.15;

	int gameScore = 0;
	int destroyedAsteroidsCount = 0;
	int asteroidsLeft = 28;

	static int midAsteroidsCount = 0;
	static int smallAsteroidsCount = 0;

	namespace Gameplay_Section
	{
		void createShoot()
		{
			gameScore = 0;
			
			for (int i = 0; i < maxShootsSpecial; i++)
			{
				shoots[i].position.x = 0;
				shoots[i].position.y = 0;
				shoots[i].speed.x = 0;
				shoots[i].speed.y = 0;
				shoots[i].rotation = 0;

				if (resolutionNormal) shoots[i].radius = 2;
				else if (resolutionSmall) shoots[i].radius = 1;
				
				shoots[i].color = WHITE;
				shoots[i].active = false;
			}

			midAsteroidsCount = 0;
			smallAsteroidsCount = 0;
			destroyedAsteroidsCount = 0;
			asteroidsLeft = 28;
		}

		void shootInput()
		{
			// Player shoot logic
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				rapidFireTimer += 1 * GetFrameTime();

				for (int i = 0; i < maxShoots; i++)
				{
					if (rapidFireTimer > rapidFireRate)
					{
						if (!shoots[i].active)
						{
							#ifdef AUDIO
							PlaySound(ship_shoot01);
							#endif
							shoots[i].position = { player.position.x + sinf(player.rotation)*(shipHeight), player.position.y - cosf(player.rotation)*(shipHeight) };

							if (resolutionNormal && !(resolutionBig))
							{
								shoots[i].speed.x = 2.0*sin(player.rotation)*player.defaultSpeed;
								shoots[i].speed.y = 2.0*cos(player.rotation)*player.defaultSpeed;
							}
							else if (resolutionSmall)
							{
								shoots[i].speed.x = 1.5*sin(player.rotation)*player.defaultSpeed;
								shoots[i].speed.y = 1.5*cos(player.rotation)*player.defaultSpeed;
							}
							else if (resolutionNormal && resolutionBig)
							{
								shoots[i].speed.x = 2.5*sin(player.rotation)*player.defaultSpeed;
								shoots[i].speed.y = 2.5*cos(player.rotation)*player.defaultSpeed;
							}
							
							shoots[i].rotation = player.rotation;
							rapidFireTimer = 0;
							shoots[i].active = true;
							break;
						}
					}		
				}
			}
		}

		void shootUpdate()
		{
			// Shot logic
			for (int i = 0; i < maxShoots; i++)
			{
				if (shoots[i].active)
				{
					// Movement
					shoots[i].position.x += shoots[i].speed.x * GetFrameTime();
					shoots[i].position.y -= shoots[i].speed.y * GetFrameTime();

					// Collision logic: shoot vs walls
					if (shoots[i].position.x > screenWidth + shoots[i].radius)
					{
						shoots[i].active = false;
					}
					else if (shoots[i].position.x < 0 - shoots[i].radius)
					{
						shoots[i].active = false;
					}

					if (shoots[i].position.y > screenHeight + shoots[i].radius)
					{
						shoots[i].active = false;
					}
					else if (shoots[i].position.y < 0 - shoots[i].radius)
					{
						shoots[i].active = false;
					}
				}
			}

			// Collision logic: player-shoots vs meteors
			for (int i = 0; i < maxShoots; i++)
			{
				if ((shoots[i].active))
				{
					for (int a = 0; a < asteroidsBigLimit; a++)
					{
						if (asteroidsBig[a].active && CheckCollisionCircles(shoots[i].position, shoots[i].radius, asteroidsBig[a].position, asteroidsBig[a].radius))
						{
							#ifdef AUDIO
							PlaySound(asteroid_explode01);
							#endif

							asteroidsBig[a].isExplosionActive = true;
							shoots[i].active = false;
							asteroidsBig[a].active = false;
							destroyedAsteroidsCount++;
							asteroidsLeft--;
							gameScore = gameScore + 25;

							for (int j = 0; j < 2; j++)
							{
								if (midAsteroidsCount % 2 == 0)
								{
									asteroidsMedium[midAsteroidsCount].position = { asteroidsBig[a].position.x, asteroidsBig[a].position.y };
									asteroidsMedium[midAsteroidsCount].speed = { cos(shoots[i].rotation)*asteroidsSpeed*-1, sin(shoots[i].rotation)*asteroidsSpeed*-1 };
								}
								else
								{
									asteroidsMedium[midAsteroidsCount].position = { asteroidsBig[a].position.x, asteroidsBig[a].position.y };
									asteroidsMedium[midAsteroidsCount].speed = { cos(shoots[i].rotation)*asteroidsSpeed, sin(shoots[i].rotation)*asteroidsSpeed };
								}

								asteroidsMedium[midAsteroidsCount].active = true;
								midAsteroidsCount++;
							}
							a = asteroidsBigLimit;
						}
					}

					for (int b = 0; b < asteroidsMediumLimit; b++)
					{
						if (asteroidsMedium[b].active && CheckCollisionCircles(shoots[i].position, shoots[i].radius, asteroidsMedium[b].position, asteroidsMedium[b].radius))
						{
							#ifdef AUDIO
							PlaySound(asteroid_explode01);
							#endif

							asteroidsMedium[b].isExplosionActive = true;
							shoots[i].active = false;
							asteroidsMedium[b].active = false;
							destroyedAsteroidsCount++;
							asteroidsLeft--;
							gameScore = gameScore + 50;

							for (int j = 0; j < 2; j++)
							{
								if (smallAsteroidsCount % 2 == 0)
								{
									asteroidsSmall[smallAsteroidsCount].position = { asteroidsMedium[b].position.x, asteroidsMedium[b].position.y };
									asteroidsSmall[smallAsteroidsCount].speed = { cos(shoots[i].rotation)*asteroidsSpeed*-1, sin(shoots[i].rotation)*asteroidsSpeed*-1 };
								}
								else
								{
									asteroidsSmall[smallAsteroidsCount].position = { asteroidsMedium[b].position.x, asteroidsMedium[b].position.y };
									asteroidsSmall[smallAsteroidsCount].speed = { cos(shoots[i].rotation)*asteroidsSpeed, sin(shoots[i].rotation)*asteroidsSpeed };
								}

								asteroidsSmall[smallAsteroidsCount].active = true;
								smallAsteroidsCount++;
							}
							b = asteroidsMediumLimit;
						}
					}

					for (int c = 0; c < asteroidsSmallLimit; c++)
					{
						if (asteroidsSmall[c].active && CheckCollisionCircles(shoots[i].position, shoots[i].radius, asteroidsSmall[c].position, asteroidsSmall[c].radius))
						{
							#ifdef AUDIO
							PlaySound(asteroid_explode01);
							#endif

							asteroidsSmall[c].isExplosionActive = true;
							shoots[i].active = false;
							asteroidsSmall[c].active = false;
							destroyedAsteroidsCount++;
							asteroidsLeft--;
							gameScore = gameScore + 100;
							c = asteroidsSmallLimit;
						}
					}
				}
			}
		}

		void shootDraw()
		{
			// Draw shoot
			for (int i = 0; i < maxShoots; i++)
			{
				if (shoots[i].active) DrawCircleV(shoots[i].position, shoots[i].radius, WHITE);
			}
		}
	}
}