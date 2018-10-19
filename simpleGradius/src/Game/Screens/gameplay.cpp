#include "gameplay.h"

#include "Setup/Game.h"
#include "Setup/Player.h"
#include "Setup\Enemy.h"
#include "Screens/settings.h"

namespace Juego
{
	bool gameON = true;

	int scoreMultiplier = 5;
	bool isExplosionActive = false;

	static const int maxButtons = 3;
	static Buttons buttons[maxButtons];
	static Buttons pauseButton;
	static int buttonDistance = 0;
	static int buttonSelect = 0;
	static Color optionColor = RED;

	static bool gamePaused = false;
	
	static bool timerON = true;

	static float matchTimer = 0.0f;
	static int matchHours = 0;
	static int matchMinutes = 0;
	static int matchSeconds = 0;
	static Color crosshairColor = WHITE;

	static Rectangle pauseBoxRec;

	static bool isButtonSoundPlaying = false;
	static int buttonSelectSaveNumber = 0;

	namespace Gameplay_Section
	{
		static void createPauseButtons()
		{

			for (int i = 0; i < maxButtons; i++)
			{
				buttons[i].position.x = (float)screenWidth / 2.5f;
				buttons[i].position.y = (float)screenHeight / 3.0f + buttonDistance;
				buttons[i].width = (float)screenWidth / 5.0f;

				if (resolutionNormal) buttons[i].height = (float)screenHeight / 12.0f;
				else if (resolutionSmall) buttons[i].height = (float)screenHeight / 14.0f;

				buttons[i].selected = false;
				buttons[i].defaultColor = GOLD;
				buttons[i].messageColor = BLANK;

				if (resolutionNormal && !(resolutionBig)) buttonDistance = buttonDistance + 100;
				else if (resolutionSmall) buttonDistance = buttonDistance + 60;
				else if (resolutionBig && resolutionNormal) buttonDistance = buttonDistance + 125;
			}

			pauseButton.position.x = (float)screenWidth / 1.4f;
			pauseButton.position.y = (float)screenHeight / 14.0f;
			pauseButton.width = (float)screenWidth / 18.0f;

			if (resolutionNormal)
			{
				pauseBoxRec = { buttons[0].position.x - (screenWidth / 50), buttons[0].position.y - (screenHeight / 30), (float)screenWidth / 4.2f, (float)screenHeight / 2.5f };
				pauseButton.height = (float)screenHeight / 12.0f;
			}
			else if (resolutionSmall)
			{
				pauseBoxRec = { buttons[0].position.x - (screenWidth / 50), buttons[0].position.y - (screenHeight / 30), (float)screenWidth / 4.2f, (float)screenHeight / 3.3f };
				pauseButton.height = (float)screenHeight / 14.0f;
			}

			pauseButton.selected = false;
			pauseButton.defaultColor = GOLD;
			pauseButton.messageColor = BLANK;
		}

		void InitGameplayVariables()
		{
			timerON = true;

			HideCursor();
			createPauseButtons();
			createPlayer();
			createEnemy();
		}

		void InitGameplayScreen()
		{

			if (resolutionNormal)
			{
			}
			else if (resolutionSmall)
			{
			}

			#ifdef AUDIO
			ship_shoot01 = LoadSound("res/sounds/ship_shoot01.wav");
			SetSoundVolume(ship_shoot01, soundVolume);

			ship_rocket01 = LoadMusicStream("res/sounds/ship_rocket01.ogg");
			SetMusicVolume(ship_rocket01, soundVolume);

			asteroid_explode01 = LoadSound("res/sounds/asteroid_explode01.wav");
			SetSoundVolume(asteroid_explode01, soundVolume);

			powerup01 = LoadSound("res/sounds/powerup01.wav");
			SetSoundVolume(powerup01, soundVolume);

			SetSoundVolume(button_select01, soundVolume);
			SetSoundVolume(button_navigate01, soundVolume);
			#endif
			isScreenFinished = false;
		}

		static void GameplayInput()
		{
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pauseButton.selected)
			{
				//PlaySound(button_select01);
				crosshairColor = BLANK;
				gamePaused = true;
				gameON = false;
			}

			// Player Input
			if (gameON)
			{
				timerON = true;
				crosshairColor = WHITE;
				HideCursor();
				playerInput();
			}
			else ShowCursor();

			if (!gamePaused)
			{
				if (IsKeyPressed(KEY_ESCAPE))
				{
					//PlaySound(button_select01);
					crosshairColor = BLANK;
					gamePaused = true;
					gameON = false;
				}
			}
			else
			{
				if (IsKeyPressed(KEY_DOWN))
				{
					mouse.selected = false;
					buttonSelect++;
					//PlaySound(button_navigate01);
					if (buttonSelect > maxButtons - 1)
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

				for (int i = 0; i < maxButtons; i++)
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && buttons[i].selected || IsKeyPressed(KEY_ENTER) && buttons[i].selected)
					{
						//PlaySound(button_select01);
						switch (i)
						{
						case 0:
							gamePaused = false;
							timerON = true;
							gameON = true;
							break;
						case 1:
							RestartPhase();
							break;
						case 2:
							buttonOption = buttonQuitToMenu;
							isScreenFinished = true;
							break;
						}
						buttons[i].selected = false;
					}
				}

				if (IsKeyPressed(KEY_ESCAPE))
				{
					//PlaySound(button_select01);
					gamePaused = false;
					timerON = true;
					gameON = true;
				}
			}
		}

		void UpdateGameplayScreen()
		{
			//UpdateMusicStream(ship_rocket01);

			player.inputActive = false;

			mouse.position = { (float)GetMouseX(),(float)GetMouseY() };
			GameplayInput();

			if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { pauseButton.position.x, pauseButton.position.y, pauseButton.width, pauseButton.height }))
			{
				pauseButton.defaultColor = WHITE;
				pauseButton.selected = true;
			}
			else
			{
				pauseButton.defaultColor = GOLD;
				pauseButton.selected = false;
			}

			if (gameON)
			{
				if (timerON)
				{
					matchTimer += 1 * GetFrameTime();
				}

				if (matchTimer > 1)
				{
					matchTimer = 0;
					if (matchSeconds < 58)
					{
						if (matchSeconds == 29)
						{
							if (scoreMultiplier > 1)
							{
								scoreMultiplier--;
							}
							else
							{
								scoreMultiplier = 1;
							}
						}
						matchSeconds++;
					}
					else
					{
						matchSeconds = 0;
						if (matchMinutes < 58)
						{
							matchMinutes++;
						}
						else
						{
							matchMinutes = 0;
							matchHours++;
						}
					}
				}

				playerUpdate();
				EnemyUpdate();
				//asteroidUpdate();


			}
			else if (gamePaused)
			{
				timerON = false;
				matchTimer = 0;

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
						buttons[i].defaultColor = GOLD;
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
							isButtonSoundPlaying = true;
							buttonSelectSaveNumber = i;
						}
					}
				}
			}


	/*		if (destroyedAsteroidsCount >= (asteroidsSmallLimit + asteroidsMediumLimit + asteroidsBigLimit) && asteroidsLeft == 0)
			{
				gameON = false;
				timerON = false;
				matchTimer = 0;
				buttonOption = buttonGameOver;
				isScreenFinished = true;
			}*/
		}

		void DrawGameplay()
		{
			DrawBackground();
			playerDraw();
			EnemyDraw();

			DrawRectangleLines(pauseButton.position.x, pauseButton.position.y, pauseButton.width, pauseButton.height, pauseButton.defaultColor);

			if (resolutionNormal && !(resolutionBig)) DrawText(FormatText(" ||"), pauseButton.position.x, pauseButton.position.y + 5, defaultFontSize, pauseButton.defaultColor);
			else if (resolutionSmall) DrawText(FormatText(" ||"), pauseButton.position.x + 2, pauseButton.position.y + 5, defaultFontSize, pauseButton.defaultColor);
			else if (resolutionBig && resolutionNormal) DrawText(FormatText(" ||"), pauseButton.position.x + 12, pauseButton.position.y + 15, defaultFontSize, pauseButton.defaultColor);
			

			if (!(gameON))
			{
				if (gamePaused)
				{
					DrawRectangleRec(pauseBoxRec, DARKPURPLE);
					for (int i = 0; i < maxButtons; i++)
					{
						DrawRectangleLines(buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height, buttons[i].defaultColor);
					}
					DrawText("PAUSED", buttons[0].position.x + (screenWidth * 0.02), buttons[0].position.y - (screenHeight * 0.1), defaultFontSize / 1.2, GOLD);
					DrawText(FormatText("CONTINUE"), buttons[0].position.x + 10, buttons[0].position.y + 5, defaultFontSize / 1.3, buttons[0].defaultColor);
					DrawText(FormatText("RESTART"), buttons[1].position.x + 8, buttons[1].position.y + 5, defaultFontSize / 1.3, buttons[1].defaultColor);
					DrawText(FormatText("MENU"), buttons[2].position.x + 10, buttons[2].position.y + 5, defaultFontSize / 1.3, buttons[2].defaultColor);
				}
			}
		}

		void DrawTimer(float widthvalue1, float widthvalue2, float heightvalue1)
		{
			if (matchHours > 0)
			{
				DrawText(FormatText("%i:", matchHours), screenWidth / (widthvalue1 + 0.25), screenHeight / heightvalue1, defaultFontSize / 1.2, YELLOW);
			}

			if (matchMinutes > 0)
			{
				if (matchMinutes > 9)
				{
					DrawText(FormatText("%i:", matchMinutes), screenWidth / widthvalue1, screenHeight / heightvalue1, defaultFontSize / 1.2, YELLOW);
				}
				else
				{
					DrawText(FormatText("0"), screenWidth / widthvalue1, screenHeight / heightvalue1, defaultFontSize / 1.2, YELLOW);
					DrawText(FormatText("%i:", matchMinutes), screenWidth / (widthvalue2 + 0.15), screenHeight / heightvalue1, defaultFontSize / 1.2, YELLOW);
				}
			}
			else
			{
				DrawText(FormatText("00:"), screenWidth / widthvalue1, screenHeight / heightvalue1, defaultFontSize / 1.2, YELLOW);
			}

			if (matchSeconds > 0)
			{
				if (matchSeconds > 9)
				{
					DrawText(FormatText("%i", matchSeconds), screenWidth / widthvalue2, screenHeight / heightvalue1, defaultFontSize / 1.2, YELLOW);
				}
				else
				{
					DrawText(FormatText("0"), screenWidth / widthvalue2, screenHeight / heightvalue1, defaultFontSize / 1.2, YELLOW);
					DrawText(FormatText("%i", matchSeconds), screenWidth / (widthvalue2 - 0.10), screenHeight / heightvalue1, defaultFontSize / 1.2, YELLOW);
				}
			}
			else
			{
				DrawText(FormatText("00"), screenWidth / widthvalue2, screenHeight / heightvalue1, defaultFontSize / 1.2, YELLOW);
			}
		}

		void RestartPhase()
		{
			InitGameplayVariables();
			buttonDistance = 0;
			gameON = true;
			timerON = true;
			matchSeconds = 0;
			matchMinutes = 0;
			matchHours = 0;
			scoreMultiplier = 5;
		}

		bool FinishGameplayScreen()
		{
			buttonDistance = 0;
			return isScreenFinished;
		}

		void DeInitGameplayResources()
		{

			#ifdef AUDIO
			StopSound(asteroid_explode01);
			StopSound(ship_shoot01);
			StopSound(powerup01);
			StopMusicStream(ship_rocket01);
			UnloadSound(asteroid_explode01);
			UnloadSound(ship_shoot01);
			UnloadSound(powerup01);
			UnloadMusicStream(ship_rocket01);
			#endif
		}
	}
}