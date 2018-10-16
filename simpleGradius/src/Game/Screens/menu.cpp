#include "menu.h"

#include "raylib.h"
#include "Setup/Game.h"
#include "Setup/Player.h"
#include "Setup/Asteroid.h"
#include "Screens/settings.h"

using namespace Juego;
using namespace Gameplay_Section;

namespace Juego
{
	static const int maxButtons = 5;

	static Buttons buttons[maxButtons];
	static int buttonDistance = 0;
	static int buttonSelect = 0;
	static Color optionColor = RED;

	static bool isButtonSoundPlaying = false;
	static int buttonSelectSaveNumber = 0;

	namespace Menu_Section
	{
		static void createMenuButtons()
		{
			for (int i = 0; i < maxButtons; i++)
			{
				buttons[i].position.x = (float)screenWidth / 2.5f; //3.8f
				buttons[i].position.y = (float)screenHeight / 3.0f + buttonDistance;
				buttons[i].width = (float)screenWidth / 5.0f;

				if(resolutionNormal) buttons[i].height = (float)screenHeight / 12.0f;
				else if(resolutionSmall) buttons[i].height = (float)screenHeight / 14.0f;
				
				buttons[i].selected = false;
				buttons[i].defaultColor = RED;
				buttons[i].messageColor = BLANK;
				

				if (resolutionNormal && !(resolutionBig)) buttonDistance = buttonDistance + 100;
				else if (resolutionSmall) buttonDistance = buttonDistance + 60;
				else if (resolutionBig && resolutionNormal) buttonDistance = buttonDistance + 125;
			}
		}

		void InitMenuScreen()
		{
			#ifdef AUDIO
			PlayMusicStream(song_alert);
			SetSoundVolume(button_select01, soundVolume);
			SetSoundVolume(button_navigate01, soundVolume);
			#endif

			createMenuButtons();
			isScreenFinished = false;
		}

		static void MenuInput()
		{
			if (IsKeyPressed(KEY_DOWN))
			{
				mouse.selected = false;
				buttonSelect++;
				PlaySound(button_navigate01);
				if (buttonSelect > maxButtons - 1)
				{
					buttonSelect--;
				}
			}

			if (IsKeyPressed(KEY_UP))
			{
				mouse.selected = false;
				buttonSelect--;
				PlaySound(button_navigate01);
				if (buttonSelect < 0)
				{
					buttonSelect++;
				}
			}

			for (int i = 0; i < maxButtons; i++)
			{
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && buttons[i].selected || IsKeyPressed(KEY_ENTER) && buttons[i].selected)
				{
					PlaySound(button_select01);
					switch (i)
					{
					case 0:
						buttonOption = buttonPlay;
						break;
					case 1:
						buttonOption = buttonControls;
						break;
					case 2:
						buttonOption = buttonSettings;
						break;
					case 3:
						buttonOption = buttonCredits;
						break;
					case 4:
						buttonOption = buttonQuit;
						break;
					}
					buttons[i].selected = false;
					isScreenFinished = true;
				}
			}
		}

		void UpdateMenuScreen()
		{
			#ifdef AUDIO
			UpdateMusicStream(song_alert);
			#endif
			asteroidUpdate();
			mouse.position = { (float)GetMouseX(),(float)GetMouseY() };

			MenuInput();
			for (int i = 0; i < maxButtons; i++)
			{

				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height }) || buttonSelect == i)
				{
						buttonSelect = i;
						buttons[i].defaultColor = WHITE;
						buttons[i].selected = true;
						
				}
				else
				{
					buttons[i].defaultColor = RED;
					buttons[i].selected = false;
					
				}
				
				if (buttonSelect != buttonSelectSaveNumber)
				{
					isButtonSoundPlaying = false;
				}

				if (buttonSelect == i)
				{
					if (!(isButtonSoundPlaying))
					{
						PlaySound(button_navigate01);
						isButtonSoundPlaying = true;
						buttonSelectSaveNumber = i;
					}
				}
			}
			
		}

		void DrawMenu()
		{
			DrawBackground();
			asteroidDraw();

			for (int i = 0; i < maxButtons; i++)
			{
				DrawRectangleLines(buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height, buttons[i].defaultColor);

				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height }) || buttonSelect == i)
				{
					buttons[i].messageColor = WHITE;

					switch (i)
					{
					case 0:
						DrawText("Start playing a match", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y, defaultFontSize / 2, buttons[i].messageColor);
						DrawText("of Simple! Asteroids!", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y + 50, defaultFontSize / 2, buttons[i].messageColor);
						break;
					case 1:
						DrawText("Learn how to play ", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y, defaultFontSize / 2, buttons[i].messageColor);
						DrawText("here!", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y + 50, defaultFontSize / 2, buttons[i].messageColor);
						break;
					case 2:
						DrawText("Change different settings", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y, defaultFontSize / 2, buttons[i].messageColor);
						DrawText("to your liking", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y + 50, defaultFontSize / 2, buttons[i].messageColor);
						break;
					case 3:
						DrawText("Get to know who made", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y, defaultFontSize / 2, buttons[i].messageColor);
						DrawText("this game!", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y + 50, defaultFontSize / 2, buttons[i].messageColor);
						break;
					case 4:
						DrawText("Stop playing the game", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y, defaultFontSize / 2, buttons[i].messageColor);
						DrawText("", buttons[i].position.x + (buttons[i].position.x * 0.55), buttons[i].position.y + 50, defaultFontSize / 2, buttons[i].messageColor);
						break;
					}
				}
				else
				{
					buttons[i].messageColor = BLANK;
				}
			}

			DrawText(FormatText("Welcome to.."), buttons[0].position.x + 10, screenHeight / 20, defaultFontSize / 2, WHITE);
			DrawText(FormatText("Simple! Asteroids"), buttons[0].position.x * 0.63f, screenHeight / 10, defaultFontSize + 20, WHITE);
			DrawText(FormatText("By frankvega"), buttons[0].position.x * 1.4f, screenHeight / 5, defaultFontSize / 2, WHITE);
			DrawText(FormatText("Version 1.0"), buttons[0].position.x * 0.63f, screenHeight / 5, defaultFontSize / 2, WHITE);
			DrawText(FormatText("PLAY"), buttons[0].position.x + 10, buttons[0].position.y + 5, defaultFontSize, buttons[0].defaultColor);
			DrawText(FormatText("CONTROLS"), buttons[1].position.x + 8, buttons[1].position.y + 5, defaultFontSize / 1.3, buttons[1].defaultColor);
			DrawText(FormatText("SETTINGS"), buttons[2].position.x + 10, buttons[2].position.y + 5, defaultFontSize / 1.3, buttons[2].defaultColor);
			DrawText(FormatText("CREDITS"), buttons[3].position.x + 10, buttons[3].position.y + 5, defaultFontSize / 1.1, buttons[3].defaultColor);
			DrawText(FormatText("QUIT"), buttons[4].position.x + 10, buttons[4].position.y + 5, defaultFontSize, buttons[4].defaultColor);
		}

		bool FinishMenuScreen()
		{
			buttonDistance = 0;
			return isScreenFinished;
		}
	}
}