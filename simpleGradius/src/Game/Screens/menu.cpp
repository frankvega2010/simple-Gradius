#include "menu.h"

#include "raylib.h"
#include "Setup/Game.h"
#include "Setup/Player.h"
#include "Screens/settings.h"
#include "Screens\gameplay.h"

using namespace Juego;
using namespace Gameplay_Section;

namespace Juego
{
	static const int maxButtons = 5;

	static Buttons buttons[maxButtons];
	static int buttonDistance = 0;
	static int buttonDistanceBelow = 0;
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
				
				if (i <= 2)
				{
					buttons[i].position.x = (float)screenWidth / 6 + buttonDistance; //3.8f
					buttons[i].position.y = (float)screenHeight / 3.0f;
				}
				else
				{
					buttons[i].position.x = (float)screenWidth / 3.5f + buttonDistanceBelow; //3.8f
					buttons[i].position.y = (float)screenHeight / 1.8f;
					if (resolutionNormal && !(resolutionBig))
					{
						if (is1600x900ResActive)buttonDistanceBelow = buttonDistanceBelow + 400;
						else buttonDistanceBelow = buttonDistanceBelow + 300;
					}
					else if (resolutionSmall)
					{
						if (is800x600ResActive)buttonDistance = buttonDistanceBelow = buttonDistanceBelow + 200;
						else buttonDistance = buttonDistanceBelow = buttonDistanceBelow + 250;
					}
					else if (resolutionBig && resolutionNormal)
					{
						buttonDistanceBelow = buttonDistanceBelow + 400;
					}
					
				}
				
				buttons[i].width = (float)screenWidth / 5.0f;

				if(resolutionNormal) buttons[i].height = (float)screenHeight / 12.0f;
				else if(resolutionSmall) buttons[i].height = (float)screenHeight / 14.0f;
				
				buttons[i].selected = false;
				buttons[i].defaultColor = RED;
				buttons[i].messageColor = BLANK;
				
				
				if (resolutionNormal && !(resolutionBig))
				{
					if (is1600x900ResActive)buttonDistance = buttonDistance + 400;
					else buttonDistance = buttonDistance + 300;
				}
				else if (resolutionSmall)
				{
					if (is800x600ResActive)buttonDistance = buttonDistance + 200;
					else buttonDistance = buttonDistance + 250;
				}
				else if (resolutionBig && resolutionNormal)
				{
					buttonDistance = buttonDistance + 400;
				}
				
			}
		}

		void InitMenuScreen()
		{
			#ifdef AUDIO
			PlayMusicStream(song_invasion);
			SetSoundVolume(button_select01, soundVolume);
			SetSoundVolume(button_navigate01, soundVolume);
			#endif

			createMenuButtons();
			isScreenFinished = false;
		}

		static void MenuInput()
		{

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
			UpdateMusicStream(song_invasion);
			#endif

			mouse.position = { (float)GetMouseX(),(float)GetMouseY() };

			MenuInput();
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
				
				if (buttonSelect != buttonSelectSaveNumber && buttonSelect != -1)
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

		void DrawBackground()
		{
			backgroundMenuSource = { 0.0f,0.0f, (float)screenWidth,(float)screenHeight };
			backgroundMenuDestination = { 0,0, (float)screenWidth,(float)screenHeight };
			backgroundMenuOrigin = { 0,0 };

			DrawTexturePro(backgroundMenu, backgroundMenuSource, backgroundMenuDestination, backgroundMenuOrigin, 0, WHITE);		
		}

		void DrawBackgroundBroken()
		{
			backgroundMenuSource = { 0.0f,0.0f, (float)screenWidth,(float)screenHeight };
			backgroundMenuDestination = { 0,0, (float)screenWidth,(float)screenHeight };
			backgroundMenuOrigin = { 0,0 };

			DrawTexturePro(backgroundMenuBroken, backgroundMenuSource, backgroundMenuDestination, backgroundMenuOrigin, 0, WHITE);

		}

		void DrawMenu()
		{
			DrawBackground();
			for (int i = 0; i < maxButtons; i++)
			{
				DrawRectangleLines(buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height, buttons[i].defaultColor);

				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height }) || buttonSelect == i)
				{
					buttons[i].messageColor = GREEN;

					switch (i)
					{
					case 0:
						DrawTextEx(sideFont, "Start playing!", { buttons[i].position.x, buttons[i].position.y + (buttons[i].position.y * 0.28f) }, defaultFontSize / 2, 0.1f, buttons[i].messageColor);
						DrawTextEx(sideFont, "New Assets will be loaded", { buttons[i].position.x, buttons[i].position.y + 50 + (buttons[i].position.y * 0.28f) }, defaultFontSize / 2, 0.1f, buttons[i].messageColor);
						break;
					case 1:
						DrawTextEx(sideFont, "Learn how to play", { buttons[i].position.x, buttons[i].position.y + (buttons[i].position.y * 0.28f) }, defaultFontSize / 2, 0.1f, buttons[i].messageColor);
						DrawTextEx(sideFont, "here!", { buttons[i].position.x, buttons[i].position.y + 50 + (buttons[i].position.y * 0.28f) }, defaultFontSize / 2, 0.1f, buttons[i].messageColor);
						break;
					case 2:
						DrawTextEx(sideFont, "Change different ", { buttons[i].position.x, buttons[i].position.y + (buttons[i].position.y * 0.28f) }, defaultFontSize / 2, 0.1f, buttons[i].messageColor);
						DrawTextEx(sideFont, "settings to your liking", { buttons[i].position.x, buttons[i].position.y + 50 + (buttons[i].position.y * 0.28f) }, defaultFontSize / 2, 0.1f, buttons[i].messageColor);
						break;
					case 3:
						DrawTextEx(sideFont, "Get to know who made", { buttons[i].position.x, buttons[i].position.y + (buttons[i].position.y * 0.18f) }, defaultFontSize / 2, 0.1f, buttons[i].messageColor);
						DrawTextEx(sideFont, "this game!", { buttons[i].position.x, buttons[i].position.y + 50 + (buttons[i].position.y * 0.18f) }, defaultFontSize / 2, 0.1f, buttons[i].messageColor);
						break;
					case 4:
						DrawTextEx(sideFont, "Closes the game", { buttons[i].position.x, buttons[i].position.y + (buttons[i].position.y * 0.18f) }, defaultFontSize / 2, 0.1f, buttons[i].messageColor);
						break;
					}
				}
				else
				{
					buttons[i].messageColor = BLANK;
				}
			}

			DrawTextEx(sideFont, "Welcome to..", { buttons[0].position.x - 80, screenHeight / 20.0f }, defaultFontSize / 2, 2, GREEN);
			DrawTextEx(mainFont, "Simple Gradius", { buttons[0].position.x - 80, screenHeight / 10.0f }, defaultFontSize + 20.0f, 2, RAYWHITE);
			DrawTextEx(sideFont, "By frankvega", { buttons[0].position.x * 3.4f, screenHeight / 5.0f }, defaultFontSize / 2, 2, GREEN);
			DrawTextEx(sideFont, "Version 1.0", { buttons[0].position.x - 80, screenHeight / 5.0f }, defaultFontSize / 2, 2, GREEN);
			DrawTextEx(mainFont, "PLAY", { buttons[0].position.x + 20, buttons[0].position.y + 10 }, defaultFontSize/1.5f, 2, buttons[0].defaultColor);
			DrawTextEx(mainFont, "TUTORIAL", { buttons[1].position.x + 5, buttons[2].position.y + 15 }, defaultFontSize / 2.05f, 2, buttons[1].defaultColor);
			DrawTextEx(mainFont, "SETTINGS", { buttons[2].position.x + 15, buttons[2].position.y + 15 }, defaultFontSize / 2.05f, 2, buttons[2].defaultColor);
			DrawTextEx(mainFont, "CREDITS", { buttons[3].position.x + 30, buttons[3].position.y + 15 }, defaultFontSize / 2.05f, 2, buttons[3].defaultColor);
			DrawTextEx(mainFont, "QUIT", { buttons[4].position.x + 30, buttons[4].position.y + 10 }, defaultFontSize / 1.5f, 2, buttons[4].defaultColor);

		}

		bool FinishMenuScreen()
		{
			return isScreenFinished;
		}

		void DeInitMenuResources()
		{
			buttonDistance = 0;
			buttonDistanceBelow = 0;
		}
	}
}