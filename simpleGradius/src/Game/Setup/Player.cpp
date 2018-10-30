#include "Player.h"

#include "Setup\Game.h"
#include "Setup\PlayerShoot.h"
#include "Screens\settings.h"
#include "Screens\gameplay.h"

namespace Juego
{
	rocketShip player;

	static Rectangle shipSource = { 0.0f,0.0f, 150,70 };
	static Rectangle shipDestination;

	static Vector2 shipOrigin = { 0,15 };
	static int currentFrame = 0;

	namespace Gameplay_Section
	{
		void createPlayer()
		{
			player.position = { (float)screenWidth / 30, (float)screenHeight / 2 };
			if (resolutionNormal) 
			{
				player.size = { 150, 50 };
				player.defaultSpeed = 500.0f;
			}
			else if (resolutionSmall) 
			{
				player.size = { 150 / resolutionSizeFix, 50 / resolutionSizeFix };
				player.defaultSpeed = 500.0f / resolutionSizeFix;
			}
			player.isAlive = true;
			player.inputActive = false;
			player.rotation = 0;
			player.textureTint = { 0, 0, 0, 0 };
		}
		
		void playerInput()
		{
			shootInput();

			if (IsKeyDown(playerKeys[UP]))
			{
				if (player.position.y < 0) player.position.y = 0;
				else
				{
					player.position.y -= player.defaultSpeed * GetFrameTime();
					player.rotation = 350;
					currentFrame = 1;
				}					
			}
			else if (IsKeyDown(playerKeys[DOWN]))
			{
				if (player.position.y + player.size.y > screenHeight) player.position.y = screenHeight - player.size.y;
				else
				{
					player.position.y += player.defaultSpeed * GetFrameTime();
					player.rotation = 10;
					currentFrame = 1;
				}	
			}
			else 
			{
				player.rotation = 0;
				currentFrame = 0;
			}
		}

		void playerUpdate()
		{
			shipSource.x = (float)currentFrame*(float)ship.width / 2;
		}

		void playerDraw()
		{
			if (resolutionNormal)
			{
				shipSource = { 0.0f,0.0f, 150,70 };
				shipDestination = { player.position.x,player.position.y, 150,70 };

				DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, player.textureTint);
				DrawTexturePro(ship, shipSource, shipDestination, shipOrigin, player.rotation, WHITE);
			}
			else if (resolutionSmall)
			{
				shipSource = { 0.0f,0.0f, 150 / resolutionSizeFix,70 / resolutionSizeFix };
				shipDestination = { player.position.x,player.position.y, 150 / resolutionSizeFix,70 / resolutionSizeFix };

				DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, player.textureTint);
				DrawTexturePro(ship, shipSource, shipDestination, shipOrigin, player.rotation, WHITE);
			}
		}
	}
}