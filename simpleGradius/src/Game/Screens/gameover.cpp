#include "gameover.h"

#include "Setup/Game.h"
#include "Screens/gameplay.h"
#include "Setup/Player.h"
#include "Screens/settings.h"
#include "Setup\PlayerShoot.h"
#include "Screens\menu.h"

using namespace Juego;
using namespace Gameplay_Section;
using namespace Menu_Section;

namespace Juego
{
	Texture2D shipExplosion;

	static const int maxButtons = 2;

	static Buttons buttons[maxButtons];
	static int buttonDistance = 0;
	static int buttonSelect = 0;
	static Color optionColor = RED;

	static bool isButtonSoundPlaying = false;
	static int buttonSelectSaveNumber = 0;

	namespace GameOver_Section
	{
		static void createGameOverButtons()
		{
			for (int i = 0; i < maxButtons; i++)
			{
				buttons[i].position.x = (float)screenWidth / 2.5f;
				buttons[i].position.y = (float)screenHeight / 3.0f + buttonDistance;
				buttons[i].width = (float)screenWidth / 5.0f;

				if (resolutionNormal) buttons[i].height = (float)screenHeight / 12.0f;
				else if (resolutionSmall) buttons[i].height = (float)screenHeight / 14.0f;

				buttons[i].selected = false;
				buttons[i].defaultColor = GREEN;
				buttons[i].messageColor = BLANK;

				if (resolutionNormal && !(resolutionBig)) buttonDistance = buttonDistance + 100;
				else if (resolutionSmall) buttonDistance = buttonDistance + 60;
				else if (resolutionBig && resolutionNormal) buttonDistance = buttonDistance + 125;
			}
		}

		void InitGameOverScreen()
		{
			if (resolutionNormal)
			{
			}
			else if (resolutionSmall)
			{
			}
			

			#ifdef AUDIO
			ship_explode01 = LoadSound("res/sounds/ship_explode01fix.wav");
			SetSoundVolume(ship_explode01, soundVolume);
			PlaySound(ship_explode01);

			points01 = LoadSound("res/sounds/points01.wav");
			SetSoundVolume(points01, soundVolume);
			SetSoundVolume(button_select01, soundVolume);
			SetSoundVolume(button_navigate01, soundVolume);
			#endif

			ShowCursor();
			createGameOverButtons();
			isScreenFinished = false;
		}

		static void GameOverInput()
		{
			if (IsKeyPressed(KEY_DOWN))
			{
				mouse.selected = false;
				buttonSelect++;
				if (buttonSelect > maxButtons - 1)
				{
					buttonSelect--;
				}
			}

			if (IsKeyPressed(KEY_UP))
			{
				mouse.selected = false;
				buttonSelect--;
				if (buttonSelect < 0)
				{
					buttonSelect++;
				}
			}

			for (int i = 0; i < maxButtons; i++)
			{
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && buttons[i].selected || IsKeyPressed(KEY_ENTER) && buttons[i].selected)
				{
					switch (i)
					{
					case 0:
						buttonOption = buttonRestart;
						break;
					case 1:
						buttonOption = buttonQuitToMenu;
						break;
					}
					buttons[i].selected = false;
					isScreenFinished = true;
				}
			}
		}

		void UpdateGameOverScreen()
		{
			GameOverInput(); 
			mouse.position = { (float)GetMouseX(),(float)GetMouseY() };
			

			for (int i = 0; i < maxButtons; i++)
			{
				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height }) || buttonSelect == i)
				{
					buttonSelect = i;
					buttons[i].defaultColor = GREEN;
					buttons[i].selected = true;
				}
				else
				{
					buttons[i].defaultColor = DARKGREEN;
					buttons[i].selected = false;
					buttonSelect = -1;
				}

				if (buttonSelect != buttonSelectSaveNumber)
				{
					isButtonSoundPlaying = false;
				}

				if (buttonSelect == i)
				{
					if (!(isButtonSoundPlaying))
					{
						isButtonSoundPlaying = true;
						buttonSelectSaveNumber = i;
					}
				}
			}
		}

		void DrawGameOver()
		{
			if (player.isAlive) DrawBackground();
			else DrawBackgroundBroken();
			

			for (int i = 0; i < maxButtons; i++)
			{
				DrawRectangleLines(buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height, buttons[i].defaultColor);
			}

			DrawTextEx(sideFont, "RESTART", { buttons[0].position.x + 10, buttons[0].position.y + 5 }, defaultFontSize / 1.3, 1.0f, buttons[0].defaultColor);
			DrawTextEx(sideFont, "MENU", { buttons[1].position.x + 10, buttons[1].position.y + 5 }, defaultFontSize / 1.3, 1.0f, buttons[1].defaultColor);

			if (targetsLeft == 0 && player.isAlive)
			{
				DrawTextEx(mainFont, "MISSION SUCCESSFUL", { buttons[0].position.x - screenWidth/5.0f, buttons[0].position.y - 60 }, defaultFontSize / 1.2, 1.0f, GOLD);
			}
			else
			{
				DrawTextEx(mainFont, "MISSION FAILED", { buttons[0].position.x - screenWidth / 10.0f, buttons[0].position.y - 60 }, defaultFontSize / 1.2, 1.0f, GREEN);
			}

			//if (isExplosionActive && !(player.isAlive))
			//{
			//	shipExplosionDestination = { player.position.x,player.position.y, increasingExplosionSize,increasingExplosionSize };

			//	if(resolutionNormal) DrawTexturePro(shipExplosion, shipExplosionSource, shipExplosionDestination, shipExplosionOrigin, 0, Fade(WHITE, increasingExplosionFade));
			//	else if(resolutionSmall ) DrawTexturePro(shipExplosion, shipExplosionSourceSmall, shipExplosionDestination, shipExplosionOrigin, 0, Fade(WHITE, increasingExplosionFade));
			//}
			
		}

		bool FinishGameOverScreen()
		{
			buttonDistance = 0;
			return isScreenFinished;
		}

		void DeInitGameOverResources()
		{
			#ifdef AUDIO
			StopSound(points01);
			StopSound(ship_explode01);
			UnloadSound(ship_explode01);
			UnloadSound(points01);
			#endif
		}
	}
}