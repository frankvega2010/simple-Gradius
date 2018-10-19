#include "Player.h"

#include "Setup\Game.h"
#include "Screens\settings.h"
#include "Screens\gameplay.h"

namespace Juego
{
	rocketShip player;
	Circle collisionCircle;

	static int defaultRotationSpeed = 280;
	static int defaultAcceleration = 1.0f;
	static int defaultDeacceleration = 0.01f;
	static int defaultBreakSpeed = 2.0f;

	
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
			player.position = { (float)screenWidth / 30, (float)screenHeight / 2 };
			player.size = { 100, 100 };
			player.defaultSpeed = 500.0f;
			player.isAlive = true;
			player.inputActive = false;
			player.textureTint = WHITE;
		}
		
		void playerInput()
		{
			if (IsKeyDown(KEY_UP)) player.position.y -= player.defaultSpeed * GetFrameTime();
			if (IsKeyDown(KEY_DOWN)) player.position.y += player.defaultSpeed * GetFrameTime();
		}

		void playerUpdate()
		{

			//player.rotation = rotationAngle;

			// Player logic: movement
			//player.position.x = player.position.x + player.acceleration.x * GetFrameTime();
			//player.position.y = player.position.y + player.acceleration.y * GetFrameTime();
			//player.position.y = player.position.y + player.defaultSpeed * GetFrameTime();
		}

		void collisionCircleUpdate()
		{
			// Players Collision Circle logic: movement
			if (resolutionNormal)
			{
				//collisionCircle.position.x = player.position.x + sinf(player.rotation)*(shipHeightCircle);
				//collisionCircle.position.y = player.position.y - cosf(player.rotation)*(shipHeightCircle);
			}
			else if (resolutionSmall)
			{
				//collisionCircle.position.x = player.position.x + sinf(player.rotation)*(shipHeightCircleSmall);
				//collisionCircle.position.y = player.position.y - cosf(player.rotation)*(shipHeightCircleSmall);
			}
		}

		void playerDraw()
		{

			DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, RED);

			if (resolutionNormal)
			{
			}
			else if (resolutionSmall)
			{
			}

			if (player.inputActive)
			{
				if (resolutionSmall)
				{
					//DrawTexturePro(shipMoving, shipMovingSourceSmall, shipMovingDestinationSmall, shipOriginSmall, rotationAngle*RAD2DEG, player.textureTint);
				}
				else if (resolutionNormal)
				{//
					//DrawTexturePro(shipMoving, shipMovingSource, shipMovingDestination, shipOrigin, rotationAngle*RAD2DEG, player.textureTint);
				}
			}
			else
			{
				if (resolutionSmall)
				{
					//DrawTexturePro(ship, shipSourceSmall, shipDestinationSmall, shipOriginSmall, rotationAngle*RAD2DEG, player.textureTint);
				}
				else if (resolutionNormal)
				{
					//DrawTexturePro(ship, shipSource, shipDestination, shipOrigin, rotationAngle*RAD2DEG, player.textureTint);
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