#include "PlayerShoot.h"

//#include "Setup/Powerups.h"
#include "Setup/Player.h"
#include "Setup/Game.h"
#include "Screens/gameplay.h"
#include "Screens/settings.h"
#include "Setup\Enemy.h"
//#include "Setup/Asteroid.h"

using namespace Juego;
using namespace Gameplay_Section;

namespace Juego
{

	Shoot shoots[maxShoots];
	Shoot shoots2[maxShoots];
	
	float rapidFireTimer = 0;
	float rapidFireRate = 0.2;

	int gameScore = 0;
	int destroyedAsteroidsCount = 0;
	int targetsLeft = 3;

	static int midAsteroidsCount = 0;
	static int smallAsteroidsCount = 0;

	namespace Gameplay_Section
	{
		void createShoot()
		{
			gameScore = 0;
			
			for (int i = 0; i < maxShoots; i++)
			{
				shoots[i].position.x = 0;
				shoots[i].position.y = 0;
				shoots[i].size.x = 60;
				shoots[i].size.y = 5;
				shoots[i].speed.x = 0;
				shoots[i].speed.y = 0;
				shoots[i].color = GREEN;
				shoots[i].active = false;

				shoots2[i].position.x = 0;
				shoots2[i].position.y = 0;
				shoots2[i].size.x = 60;
				shoots2[i].size.y = 5;
				shoots2[i].speed.x = 0;
				shoots2[i].speed.y = 0;
				shoots2[i].color = GREEN;
				shoots2[i].active = false;
			}

			/*midAsteroidsCount = 0;
			smallAsteroidsCount = 0;
			destroyedAsteroidsCount = 0;
			asteroidsLeft = 28;*/
		}

		void shootInput()
		{
			// Player shoot logic
			if (IsKeyDown(playerKeys[SHOOT]))
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
							shoots[i].position = { player.position.x + player.size.x - 15, player.position.y + player.size.y/2 };
							shoots2[i].position = { player.position.x + player.size.x - 10, player.position.y + player.size.y / 1.2f };

							if (resolutionNormal && !(resolutionBig))
							{
								shoots[i].speed.x = 2.0*player.defaultSpeed;
								shoots[i].speed.y = 2.0*player.defaultSpeed;
								shoots2[i].speed.x = 2.0*player.defaultSpeed;
								shoots2[i].speed.y = 2.0*player.defaultSpeed;
							}/*
							else if (resolutionSmall)
							{
								shoots[i].speed.x = 1.5*sin(player.rotation)*player.defaultSpeed;
								shoots[i].speed.y = 1.5*cos(player.rotation)*player.defaultSpeed;
							}
							else if (resolutionNormal && resolutionBig)
							{
								shoots[i].speed.x = 2.5*sin(player.rotation)*player.defaultSpeed;
								shoots[i].speed.y = 2.5*cos(player.rotation)*player.defaultSpeed;
							}*/
							
							//shoots[i].rotation = player.rotation;
							rapidFireTimer = 0;
							shoots[i].active = true;
							shoots2[i].active = true;
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
					shoots2[i].position.x += shoots[i].speed.x * GetFrameTime();
					//shoots[i].position.y -= shoots[i].speed.y * GetFrameTime();

					// Collision logic: shoot vs walls
					if (shoots[i].position.x > screenWidth)
					{
						shoots[i].active = false;
						shoots2[i].active = false;
					}
					/*else if (shoots[i].position.x < 0 - shoots[i].radius)
					{
						shoots[i].active = false;
					}*/

					if (shoots[i].position.y > screenHeight)
					{
						shoots[i].active = false;
						shoots2[i].active = false;
					}
					else if (shoots[i].position.y < 0)
					{
						shoots[i].active = false;
						shoots2[i].active = false;
					}
				}
			}

			// Collision logic: player-shoots vs meteors
			for (int i = 0; i < maxShoots; i++)
			{
				if ((shoots[i].active))
				{	
					if (CheckCollisionRecs({ shoots[i].position.x,shoots[i].position.y,shoots[i].size.x,shoots[i].size.y }, { enemy01.position.x,enemy01.position.y,enemy01.size.x,enemy01.size.y }))
					{
						enemy01.isAlive = false;
						shoots[i].active = false;
						shoots2[i].active = false;
						targetsLeft--;	
					}
				}

				if ((shoots2[i].active))
				{
					if (CheckCollisionRecs({ shoots2[i].position.x,shoots2[i].position.y,shoots2[i].size.x,shoots2[i].size.y }, { enemy01.position.x,enemy01.position.y,enemy01.size.x,enemy01.size.y }))
					{
						enemy01.isAlive = false;
						shoots[i].active = false;
						shoots2[i].active = false;
						targetsLeft--;
					}
				}
			}
		}

		void shootDraw()
		{
			// Draw shoot
			for (int i = 0; i < maxShoots; i++)
			{
				if (shoots[i].active)DrawRectangle(shoots[i].position.x,shoots[i].position.y,shoots[i].size.x,shoots[i].size.y, shoots[i].color);
				if (shoots2[i].active)DrawRectangle(shoots2[i].position.x, shoots2[i].position.y, shoots2[i].size.x, shoots2[i].size.y, shoots2[i].color);// DrawCircleV(shoots[i].position, shoots[i].radius, WHITE);
			}
		}
	}
}