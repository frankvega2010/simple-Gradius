#include "Player.h"

#include <cmath>

#include "Setup\Game.h"
#include "Setup\Asteroid.h"
#include "Setup\PlayerShoot.h"
#include "Screens\settings.h"
#include "Screens\gameplay.h"

namespace Juego
{
	rocketShip player;
	Circle collisionCircle;

	float shipHeight = (playerBaseSize / 2) / tanf(20 * DEG2RAD);
	float shipHeightCircle = (playerBaseSize / 2) / tanf(38 * DEG2RAD);

	static float shipHeightCircleSmall = (playerBaseSize / 2) / tanf(62 * DEG2RAD);

	static int defaultRotationSpeed = 280;
	static int defaultAcceleration = 1.0f;
	static int defaultDeacceleration = 0.01f;
	static int defaultBreakSpeed = 2.0f;

	static Rectangle shipSource = { 0.0f,0.0f, 50,50 };
	static Rectangle shipMovingSource = { 0.0f,0.0f, 50,58 };

	static Rectangle shipSourceSmall = { 0.0f,0.0f, 50/2,50/2 };
	static Rectangle shipMovingSourceSmall = { 0.0f,0.0f, 50/2,58/2 };

	static Rectangle shipDestination;
	static Rectangle shipMovingDestination;

	static Rectangle shipDestinationSmall;
	static Rectangle shipMovingDestinationSmall;

	static Vector2 shipOrigin = { 25,48 };
	static Vector2 shipOriginSmall = { 25/2,48/2 };
	
	//Player movement with mouse
	static Vector2 rotationDirection;
	static float rotationAngle = 0.0f;
	static Vector2 normalizedDirection = {0,0};
	static Vector2 playerNewPosition = { 0,0 };
	static const float angleFix = 1.57f;
	static const int accelerationDefaultSpeed = 250;

	namespace Gameplay_Section
	{
		void createPlayer()
		{
			player.position = { (float)screenWidth / 2, (float)screenHeight / 2 };
			player.score = 0;
			player.acceleration = {10,10};
			player.rotation = 0.0f;
			player.defaultSpeed = 300.0f;
			player.isAlive = true;
			player.inputActive = false;
			player.textureTint = WHITE;
		}

		void createCollisionCircle()
		{
			collisionCircle.position.x = player.position.x;
			collisionCircle.position.y = player.position.y;

			if(resolutionNormal) collisionCircle.radius = 15;
			else if(resolutionSmall) collisionCircle.radius = 15/2;

			collisionCircle.speed = { 0,0 };
			collisionCircle.active = true;
		}
		
		void playerInput()
		{
			shootInput();

			//Player logic: acceleration
			if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
			{
				#ifdef AUDIO
				PlayMusicStream(ship_rocket01);
				#endif
				
				player.inputActive = true;

				normalizedDirection.x = rotationDirection.x / sqrt(pow(rotationDirection.x,2.0) + pow(rotationDirection.y, 2.0));
				normalizedDirection.y = rotationDirection.y / sqrt(pow(rotationDirection.x, 2.0) + pow(rotationDirection.y, 2.0));

				player.acceleration.x += normalizedDirection.x * accelerationDefaultSpeed * GetFrameTime();
				player.acceleration.y += normalizedDirection.y * accelerationDefaultSpeed * GetFrameTime();
			}
			else
			{
				#ifdef AUDIO
				StopMusicStream(ship_rocket01);
				#endif
			}
		}

		void playerUpdate()
		{
			rotationDirection.x = (float)mouse.position.x - (float)player.position.x;
			rotationDirection.y = (float)mouse.position.y - (float)player.position.y;

			rotationAngle = atan2(rotationDirection.y, rotationDirection.x) + angleFix;

			player.rotation = rotationAngle;

			// Player logic: movement
			player.position.x = player.position.x + player.acceleration.x * GetFrameTime();
			player.position.y = player.position.y + player.acceleration.y * GetFrameTime();
		}

		void collisionCircleUpdate()
		{
			// Players Collision Circle logic: movement
			if (resolutionNormal)
			{
				collisionCircle.position.x = player.position.x + sinf(player.rotation)*(shipHeightCircle);
				collisionCircle.position.y = player.position.y - cosf(player.rotation)*(shipHeightCircle);
			}
			else if (resolutionSmall)
			{
				collisionCircle.position.x = player.position.x + sinf(player.rotation)*(shipHeightCircleSmall);
				collisionCircle.position.y = player.position.y - cosf(player.rotation)*(shipHeightCircleSmall);
			}
		}

		void playerDraw()
		{
			shipDestination = { player.position.x,player.position.y, 50,50 };
			shipDestinationSmall = { player.position.x,player.position.y, 50/2,50/2 };

			shipMovingDestination = { player.position.x,player.position.y, 50,58 };
			shipMovingDestinationSmall = { player.position.x,player.position.y, 50/2,58/2 };

			if (resolutionNormal)
			{
				Vector2 v1 = { player.position.x + sinf(player.rotation)*(shipHeight), player.position.y - cosf(player.rotation)*(shipHeight) };
				Vector2 v2 = { player.position.x - cosf(player.rotation)*(playerBaseSize / 2), player.position.y - sinf(player.rotation)*(playerBaseSize / 2) };
				Vector2 v3 = { player.position.x + cosf(player.rotation)*(playerBaseSize / 2), player.position.y + sinf(player.rotation)*(playerBaseSize / 2) };

				DrawTriangle(v1, v2, v3, MAROON);
			}
			else if (resolutionSmall)
			{
				Vector2 v1 = { player.position.x + sinf(player.rotation)*(shipHeight / 2), player.position.y - cosf(player.rotation)*(shipHeight / 2) };
				Vector2 v2 = { player.position.x - cosf(player.rotation)*((playerBaseSize / 2) / 2), player.position.y - sinf(player.rotation)*((playerBaseSize / 2) / 2) };
				Vector2 v3 = { player.position.x + cosf(player.rotation)*((playerBaseSize / 2) / 2), player.position.y + sinf(player.rotation)*((playerBaseSize / 2) / 2) };

				DrawTriangle(v1, v2, v3, MAROON);
			}

			if (player.inputActive)
			{
				if (resolutionSmall)
				{
					DrawTexturePro(shipMoving, shipMovingSourceSmall, shipMovingDestinationSmall, shipOriginSmall, rotationAngle*RAD2DEG, player.textureTint);
				}
				else if (resolutionNormal)
				{
					DrawTexturePro(shipMoving, shipMovingSource, shipMovingDestination, shipOrigin, rotationAngle*RAD2DEG, player.textureTint);
				}
			}
			else
			{
				if (resolutionSmall)
				{
					DrawTexturePro(ship, shipSourceSmall, shipDestinationSmall, shipOriginSmall, rotationAngle*RAD2DEG, player.textureTint);
				}
				else if (resolutionNormal)
				{
					DrawTexturePro(ship, shipSource, shipDestination, shipOrigin, rotationAngle*RAD2DEG, player.textureTint);
				}
			}
		}

		#ifdef TESTING
				void collisionCircleDraw() // only in DEBUG mode
				{
					if (resolutionNormal)
					{
						DrawCircleV({ player.position.x + sinf(player.rotation)*(shipHeightCircle), player.position.y - cosf(player.rotation)*(shipHeightCircle) }, collisionCircle.radius, { 100, 0, 0, 200 });
					}
					else if (resolutionSmall)
					{
						DrawCircleV({ player.position.x + sinf(player.rotation)*(shipHeightCircleSmall), player.position.y - cosf(player.rotation)*(shipHeightCircleSmall) }, collisionCircle.radius, { 100, 0, 0, 200 });
					}
					DrawLineEx(player.position, mouse.position, 1, YELLOW);
				}
		#endif
		#ifdef RELEASE
				void collisionCircleDraw() // only in RELEASE mode
				{
					if (resolutionNormal)
					{
						DrawCircleV({ player.position.x + sinf(player.rotation)*(shipHeightCircle), player.position.y - cosf(player.rotation)*(shipHeightCircle) }, collisionCircle.radius, { 0, 0, 0,0 });
					}
					else if (resolutionSmall)
					{
						DrawCircleV({ player.position.x + sinf(player.rotation)*(shipHeightCircleSmall), player.position.y - cosf(player.rotation)*(shipHeightCircleSmall) }, collisionCircle.radius, { 0, 0, 0, 0 });
					}
				}
		#endif
	}
}