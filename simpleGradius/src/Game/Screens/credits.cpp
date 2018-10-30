#include "credits.h"

#include "raylib.h"

#include "Setup/Game.h"
#include "Screens/gameplay.h"
#include "Screens/controls.h"
#include "Screens/settings.h"
#include "Screens\menu.h"

using namespace Juego;
using namespace Gameplay_Section;
using namespace Menu_Section;

namespace Juego
{
	static const int maxButtons = 2;

	static Buttons buttons[maxButtons];
	static int buttonSelect = 0;

	static bool isButtonSoundPlaying = false;
	static int buttonSelectSaveNumber = 0;

	static bool moreCredits = false;
	static int buttonDistanceCredits = 0;

	namespace Credits_Section
	{
		static void createCreditsButtons()
		{
			for (int i = 0; i < maxButtons; i++)
			{
				buttons[i].position.x = (float)screenWidth / 25.0f;
				buttons[i].position.y = (float)screenHeight / 1.15f - buttonDistanceCredits;
				buttons[i].width = (float)screenWidth / 5.0f;
				buttons[i].height = (float)screenHeight / 12.0f;
				buttons[i].selected = false;
				buttons[i].defaultColor = RED;
				buttons[i].messageColor = BLANK;

				buttonDistanceCredits = buttonDistanceCredits + 100;
			}

			buttonDistanceCredits = 0;
		}

		void InitCreditsScreen()
		{
			#ifdef AUDIO
			SetSoundVolume(button_select01, soundVolume);
			SetSoundVolume(button_navigate01, soundVolume);
			#endif

			createCreditsButtons();
			isScreenFinished = false;
		}

		static void CreditosInput()
		{

			for (int i = 0; i < maxButtons; i++)
			{
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && buttons[i].selected || IsKeyPressed(KEY_ENTER) && buttons[i].selected)
				{
					PlaySound(button_select01);
					switch (i)
					{
					case 0:
						buttonOption = buttonGoMenu;
						isScreenFinished = true;
						break;
					case 1:
						moreCredits =! moreCredits;
						break;
					}
					buttons[i].selected = false;
				}
			}
		}

		void UpdateCreditsScreen()
		{
			#ifdef AUDIO
			UpdateMusicStream(song_invasion);
			#endif

			mouse.position = { (float)GetMouseX(),(float)GetMouseY() };

			CreditosInput();
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
		
		void DrawCredits()
		{
			DrawBackground();

			for (int i = 0; i < maxButtons; i++)
			{
				DrawRectangleLines(buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height, buttons[i].defaultColor);

				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height }) || buttonSelect == i)
				{
					buttons[i].messageColor = GREEN;
				}
				else
				{
					buttons[i].messageColor = BLANK;
				}
			}

			DrawTextEx(mainFont, "Simple Gradius", { (float)screenWidth / 2.5f * 0.263f, screenHeight / 10.0f }, defaultFontSize + 20.0f - gameTitleSizeFix, 2, RAYWHITE);
			DrawTextEx(sideFont, "Version 1.0", { (float)screenWidth / 2.5f * 0.263f, screenHeight / 5.0f }, defaultFontSize / 2, 2, GREEN);

			if (!moreCredits)
			{
				DrawTextEx(mainFont, "Game made by", { (float)screenWidth / 3.0f, screenHeight / 3.3f }, defaultFontSize / 1.5, 2, WHITE);
				DrawTextEx(sideFont, "Franco Vega aka frankvega", { (float)screenWidth / 4.0f, screenHeight / 2.8f }, defaultFontSize / 1.5, 2, GREEN);

				DrawTextEx(mainFont, "Player and Enemies Designs", { (float)screenWidth / 8.0f, screenHeight / 2.2f }, defaultFontSize / 1.5, 2, WHITE);
				DrawTextEx(sideFont, "Lautaro Cabrini", { (float)screenWidth / 3.0f, screenHeight / 2.0f }, defaultFontSize / 1.5, 2, GREEN);

				DrawTextEx(mainFont, "Tools Used", { (float)screenWidth / 3.0f, screenHeight / 1.7f }, defaultFontSize / 1.5, 2, WHITE);
				DrawTextEx(sideFont, "Raylib", { (float)screenWidth / 2.4f, screenHeight / 1.57f }, defaultFontSize / 1.5, 2, GREEN);
				DrawTextEx(sideFont, "Adobe Illustrator", { (float)screenWidth / 3.2f, screenHeight / 1.47f }, defaultFontSize / 1.5, 2, GREEN);
				DrawTextEx(sideFont, "Adobe Photoshop", { (float)screenWidth / 3.0f, screenHeight / 1.37f }, defaultFontSize / 1.5, 2, GREEN);
			}
			else
			{
				DrawTextEx(mainFont, "FONTS", { (float)screenWidth / 2.4f, screenHeight / 3.3f }, defaultFontSize / 1.5, 2, WHITE);

				DrawTextEx(sideFont, "'BIGMACCA' by BIGMACCA", { (float)screenWidth / 4.0f, screenHeight / 2.8f }, defaultFontSize / 1.5, 2, GREEN);
				DrawTextEx(sideFont, "http://fontstruct.com/fontstructions/show/420423", { (float)screenWidth / 8.0f, screenHeight / 2.5f }, defaultFontSize / 2, 2, WHITE);

				DrawTextEx(sideFont, "'Reality Pursuit NC' by Jayvee Enaguas", { (float)screenWidth / 8.0f, screenHeight / 2.2f }, defaultFontSize / 1.5, 2, GREEN);
				DrawTextEx(sideFont, "https://www.dafont.com/es/reality-pursuit-nc.font", { (float)screenWidth / 8.0f, screenHeight / 2.0f }, defaultFontSize / 2, 2, WHITE);


				DrawTextEx(mainFont, "Testers", { (float)screenWidth / 2.6f, screenHeight / 1.7f }, defaultFontSize / 1.5, 2, WHITE);
				DrawTextEx(sideFont, "Dylan Piserchia", { (float)screenWidth / 2.8f, screenHeight / 1.57f }, defaultFontSize / 1.5, 2, GREEN);
				DrawTextEx(sideFont, "Patricio Perrin  aka orly", { (float)screenWidth / 4.0f, screenHeight / 1.47f }, defaultFontSize / 1.5, 2, GREEN);
			}

			DrawTextEx(mainFont, "MENU", { buttons[0].position.x + 35, buttons[0].position.y + 10 }, defaultFontSize / 1.5f, 2, buttons[0].defaultColor);
			DrawTextEx(mainFont, "MORE", { buttons[1].position.x + 35, buttons[1].position.y + 10 }, defaultFontSize / 1.5f, 2, buttons[1].defaultColor);
		}

		bool FinishCreditsScreen()
		{
			return isScreenFinished;
		}
	}
}