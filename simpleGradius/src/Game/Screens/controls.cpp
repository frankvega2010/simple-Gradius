#include "controls.h"

#include "raylib.h"

#include "Setup/Game.h"
#include "Screens/gameplay.h"
#include "Screens/settings.h"
#include "Screens\menu.h"

using namespace Juego;
using namespace Gameplay_Section;
using namespace Menu_Section;

namespace Juego
{
	static Image controlSchemeImage;
	static Texture2D controlScheme;

	static const int maxButtonsControls = 5;
	static const int maxButtonsControlsKeys = 3;
	static Buttons buttonsControls[maxButtonsControls];
	static int buttonSelect = 0;
	static int buttonDistanceControls = 0;
	static int buttonDistanceControlsKeys = 0;

	static bool moreControls = false;
	int resolutionControlsFontSize = defaultFontSize / 2.2f;

	static bool isButtonSoundPlaying = false;
	static int buttonSelectSaveNumber = 0;

	namespace Controls_Section
	{
		static void createControlsButtons()
		{
			for (int i = 0; i < maxButtonsControls; i++)
			{
				buttonsControls[i].position.x = (float)screenWidth / 25.0f;
				buttonsControls[i].position.y = (float)screenHeight / 1.4f + buttonDistanceControls;
				buttonsControls[i].width = (float)screenWidth / 5.0f;
				buttonsControls[i].height = (float)screenHeight / 12.0f;
				buttonsControls[i].selected = false;
				buttonsControls[i].defaultColor = DARKGREEN;
				buttonsControls[i].messageColor = BLANK;

				if (i > 1)
				{
					buttonsControls[i].position.x = (float)screenWidth / 2.5f;
					buttonsControls[i].position.y = (float)screenHeight / 1.55f + buttonDistanceControlsKeys;
					buttonDistanceControlsKeys = buttonDistanceControlsKeys + 100;
				}

				if (i > 3)
				{
					buttonsControls[i].position.x = (float)screenWidth / 1.5f;
					buttonsControls[i].position.y = (float)screenHeight / 1.2f;
				}

				buttonDistanceControls = buttonDistanceControls + 100;
			}
		}

		void InitControlsScreen()
		{
			createControlsButtons();

			controlSchemeImage = LoadImage("res/assets/textures/controlscheme01.png");
			ImageResize(&controlSchemeImage, screenWidth, screenHeight);
			controlScheme = LoadTextureFromImage(controlSchemeImage);
			UnloadImage(controlSchemeImage);

			#ifdef AUDIO
			//SetSoundVolume(button_select01, soundVolume);
			//SetSoundVolume(button_navigate01, soundVolume);
			#endif

			isScreenFinished = false;
		}

		static void ControlsInput()
		{
			if (IsKeyPressed(KEY_DOWN))
			{
				mouse.selected = false;
				buttonSelect++;
				//PlaySound(button_navigate01);
				if (buttonSelect > maxButtonsControls - 1)
				{
					buttonSelect--;
				}
			}

			if (IsKeyPressed(KEY_UP))
			{
				mouse.selected = false;
				buttonSelect--;
				//PlaySound(button_navigate01);
				if (buttonSelect < 0)
				{
					buttonSelect++;
				}
			}

			for (int i = 0; i < maxButtonsControls; i++)
			{
				if (moreControls && i > 1) i = maxButtonsControls;
				else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && buttonsControls[i].selected || IsKeyPressed(KEY_ENTER) && buttonsControls[i].selected)
				{
					//	PlaySound(button_select01);
					switch (i)
					{
					case 0:
						moreControls = !moreControls;
						break;
					case 1:
						buttonOption = buttonGoMenu;
						isScreenFinished = true;
						break;
					}
					buttonsControls[i].selected = false;
				}
				
			}
		}

		void UpdateControlsScreen()
		{
			#ifdef AUDIO
			UpdateMusicStream(song_invasion);
			#endif

			//asteroidUpdate();
			mouse.position = { (float)GetMouseX(),(float)GetMouseY() };

			ControlsInput();
			for (int i = 0; i < maxButtonsControls; i++)
			{
				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttonsControls[i].position.x, buttonsControls[i].position.y, buttonsControls[i].width, buttonsControls[i].height }) || buttonSelect == i)
				{
					buttonSelect = i;
					buttonsControls[i].defaultColor = GREEN;
					buttonsControls[i].selected = true;
				}
				else
				{
					buttonsControls[i].defaultColor = DARKGREEN;
					buttonsControls[i].selected = false;
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
						//PlaySound(button_navigate01);
						isButtonSoundPlaying = true;
						buttonSelectSaveNumber = i;
					}
				}
			}
		}
		
		void DrawControls()
		{
			//asteroidDraw();
			DrawBackground();

			for (int i = 0; i < maxButtonsControls; i++)
			{
				if (moreControls && i > 1) i = maxButtonsControls;
				else DrawRectangleLines(buttonsControls[i].position.x, buttonsControls[i].position.y, buttonsControls[i].width, buttonsControls[i].height, buttonsControls[i].defaultColor);

				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttonsControls[i].position.x, buttonsControls[i].position.y, buttonsControls[i].width, buttonsControls[i].height }) || buttonSelect == i)
				{
					buttonsControls[i].messageColor = GREEN;
				}
				else
				{
					buttonsControls[i].messageColor = BLANK;
				}
			}

			//More Controls Text
			if (moreControls)
			{
				DrawTextEx(mainFont, "Objetive", { screenWidth / 18.0f, 60 }, defaultFontSize, 1.0f, WHITE);
				DrawTextEx(sideFont, "Complete the mission by destroying", { screenWidth / 18.0f, screenHeight / 6.0f }, defaultFontSize / 1.8, 1.0f, GREEN);
				DrawTextEx(sideFont, "the designated amount of targets.", { screenWidth / 18.0f, screenHeight / 5.0f }, defaultFontSize / 1.8, 1.0f, GREEN);

				DrawTextEx(mainFont, "Enemies", { screenWidth / 18.0f,screenHeight / 3.0f }, defaultFontSize/1.5f, 1.0f, WHITE);
				DrawTextEx(sideFont, "You will be facing slow and fast enemies.", { screenWidth / 18.0f, screenHeight / 2.5f }, defaultFontSize / 1.8, 1.0f, GREEN);
				DrawTextEx(sideFont, "We haven't located any enemy Commander yet....", { screenWidth / 18.0f, screenHeight / 1.9f }, defaultFontSize / 1.8, 1.0f, GREEN);
			}
			else
			{
				backgroundMenuSource = { 0.0f,0.0f, (float)screenWidth,(float)screenHeight };
				backgroundMenuDestination = { 0,0, (float)screenWidth,(float)screenHeight };
				backgroundMenuOrigin = { 0,0 };

				DrawTexturePro(controlScheme, backgroundMenuSource, backgroundMenuDestination, backgroundMenuOrigin, 0, WHITE);
				for (int i = 2; i < maxButtonsControls; i++)
				{
					DrawTextEx(sideFont, "Default-UP",    { buttonsControls[2].position.x + 5, buttonsControls[2].position.y + 10 }, resolutionControlsFontSize, 1.0f, buttonsControls[2].defaultColor);
					DrawTextEx(sideFont, "Default-DOWN",  { buttonsControls[3].position.x + 5, buttonsControls[3].position.y + 10 }, resolutionControlsFontSize, 1.0f, buttonsControls[3].defaultColor);
					DrawTextEx(sideFont, "Default-SPACE", { buttonsControls[4].position.x + 5, buttonsControls[4].position.y + 10 }, resolutionControlsFontSize, 1.0f, buttonsControls[4].defaultColor);
				}
			}

			DrawTextEx(mainFont, "MORE", { buttonsControls[0].position.x + 35, buttonsControls[0].position.y + 10 }, defaultFontSize / 1.5f, 1.0f, buttonsControls[0].defaultColor);
			DrawTextEx(mainFont, "MENU", { buttonsControls[1].position.x + 35, buttonsControls[1].position.y + 10 }, defaultFontSize / 1.5f, 1.0f, buttonsControls[1].defaultColor);
		}

		bool FinishControlsScreen()
		{
			return isScreenFinished;
		}

		void DeInitControlsResources()
		{
			UnloadTexture(controlScheme);
			buttonDistanceControls = 0;
			buttonDistanceControlsKeys = 0;
			moreControls = false;
		}
	}
}
