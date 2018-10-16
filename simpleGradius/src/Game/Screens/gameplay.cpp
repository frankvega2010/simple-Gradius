#include "gameplay.h"

#include "Setup/Game.h"
#include "Setup/Player.h"
#include "Setup/Asteroid.h"
#include "Setup/PlayerShoot.h"
#include "Setup/Powerups.h"
#include "Screens/settings.h"

namespace Juego
{
	Image explosionImage;

	Texture2D asteroidExplosion;
	Texture2D ship;
	Texture2D shipMoving;
	Texture2D powerupMaxRapidFire;
	Texture2D powerupInvincibility;

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

	//Resources
	static Image shipImage;
	static Image shipMovingImage;
	static Image crosshairImage;
	static Image powerupMaxRapidFireImage;
	static Image powerupInvincibilityImage;

	static Texture2D crosshair;

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
			createAsteroid();
			createPlayer();
			createShoot();
			createCollisionCircle();
			createPowerups();
		}

		void InitGameplayScreen()
		{
			//Textures Load
			crosshairImage = LoadImage("res/textures/crosshair01.png");
			ImageResize(&crosshairImage, 30, 30);
			crosshair = LoadTextureFromImage(crosshairImage);

			if (resolutionNormal)
			{
				shipImage = LoadImage("res/textures/nave01.png");
				ImageResize(&shipImage, 50, 50);
				ship = LoadTextureFromImage(shipImage);

				shipMovingImage = LoadImage("res/textures/nave01_moving.png");
				ImageResize(&shipMovingImage, 50, 58);//58
				shipMoving = LoadTextureFromImage(shipMovingImage);

				explosionImage = LoadImage("res/textures/explosion01.png");
				ImageResize(&explosionImage, 50, 50);

				asteroidExplosion = LoadTextureFromImage(explosionImage);

				powerupMaxRapidFireImage = LoadImage("res/textures/powerup02.png");
				ImageResize(&powerupMaxRapidFireImage, 30, 30);
				powerupMaxRapidFire = LoadTextureFromImage(powerupMaxRapidFireImage);

				powerupInvincibilityImage = LoadImage("res/textures/powerup01.png");
				ImageResize(&powerupInvincibilityImage, 30, 30);
				powerupInvincibility = LoadTextureFromImage(powerupInvincibilityImage);
			}
			else if (resolutionSmall)
			{
				shipImage = LoadImage("res/textures/nave01.png");
				ImageResize(&shipImage, 50/2, 50/2);
				ship = LoadTextureFromImage(shipImage);

				shipMovingImage = LoadImage("res/textures/nave01_moving.png");
				ImageResize(&shipMovingImage, 50/2, 58/2);
				shipMoving = LoadTextureFromImage(shipMovingImage);

				explosionImage = LoadImage("res/textures/explosion01.png");
				ImageResize(&explosionImage, 50/2, 50/2);

				asteroidExplosion = LoadTextureFromImage(explosionImage);

				powerupMaxRapidFireImage = LoadImage("res/textures/powerup02.png");
				ImageResize(&powerupMaxRapidFireImage, 30/2, 30/2);
				powerupMaxRapidFire = LoadTextureFromImage(powerupMaxRapidFireImage);

				powerupInvincibilityImage = LoadImage("res/textures/powerup01.png");
				ImageResize(&powerupInvincibilityImage, 30/2, 30/2);
				powerupInvincibility = LoadTextureFromImage(powerupInvincibilityImage);
			}

			UnloadImage(crosshairImage);
			UnloadImage(shipImage);
			UnloadImage(explosionImage);
			UnloadImage(shipMovingImage);
			UnloadImage(powerupMaxRapidFireImage);
			UnloadImage(powerupInvincibilityImage);

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
				PlaySound(button_select01);
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
					PlaySound(button_select01);
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
					PlaySound(button_select01);
					gamePaused = false;
					timerON = true;
					gameON = true;
				}
			}
		}

		void UpdateGameplayScreen()
		{
			UpdateMusicStream(ship_rocket01);

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
				collisionCircleUpdate();
				powerupsUpdate();
				shootUpdate();
				asteroidUpdate();

				// Collision logic: player vs walls
				if (player.position.x > screenWidth + shipHeight) player.position.x = -(shipHeight);
				else if (player.position.x < -(shipHeight)) player.position.x = screenWidth + shipHeight;
				if (player.position.y >(screenHeight + shipHeight)) player.position.y = -(shipHeight);
				else if (player.position.y < -(shipHeight)) player.position.y = screenHeight + shipHeight;

				for (int i = 0; i < asteroidsBigLimit; i++)
				{
					if (CheckCollisionCircles(collisionCircle.position, collisionCircle.radius, asteroidsBig[i].position, asteroidsBig[i].radius) && asteroidsBig[i].active && !powerups[Invincibility].activated)
					{
						gameON = false;
						buttonOption = buttonGameOver;
						isScreenFinished = true;
						scoreMultiplier = 1;
						isExplosionActive = true;
						player.isAlive = false;
						break;
					}
				}

				for (int i = 0; i < asteroidsMediumLimit; i++)
				{
					if (CheckCollisionCircles(collisionCircle.position, collisionCircle.radius, asteroidsMedium[i].position, asteroidsMedium[i].radius) && asteroidsMedium[i].active && !powerups[Invincibility].activated)
					{
						gameON = false;
						buttonOption = buttonGameOver;
						isScreenFinished = true;
						scoreMultiplier = 1;
						isExplosionActive = true;
						player.isAlive = false;
						break;
					}
				}

				for (int i = 0; i < asteroidsSmallLimit; i++)
				{
					if (CheckCollisionCircles(collisionCircle.position, collisionCircle.radius, asteroidsSmall[i].position, asteroidsSmall[i].radius) && asteroidsSmall[i].active && !powerups[Invincibility].activated)
					{
						gameON = false;
						buttonOption = buttonGameOver;
						isScreenFinished = true;
						scoreMultiplier = 1;
						isExplosionActive = true;
						player.isAlive = false;
						break;
					}
				}
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
							PlaySound(button_navigate01);
							isButtonSoundPlaying = true;
							buttonSelectSaveNumber = i;
						}
					}
				}
			}


			if (destroyedAsteroidsCount >= (asteroidsSmallLimit + asteroidsMediumLimit + asteroidsBigLimit) && asteroidsLeft == 0)
			{
				gameON = false;
				timerON = false;
				matchTimer = 0;
				buttonOption = buttonGameOver;
				isScreenFinished = true;
			}
		}

		void DrawGameplay()
		{
			DrawBackground();
			collisionCircleDraw();
			asteroidDraw();
			shootDraw();
			playerDraw();
			powerupsDraw();

			DrawText(FormatText("Asteroids:%i",asteroidsLeft), screenWidth / 80, screenHeight / 14, defaultFontSize / 1.2, YELLOW);
			DrawText(FormatText("Time: "), screenWidth / 3.40, screenHeight / 14, defaultFontSize / 1.2, YELLOW);
			DrawTimer(2.5f, 2.2f,14.0f);

			DrawTexturePro(crosshair, { 0,0,30,30 }, { mouse.position.x,mouse.position.y,30,30 }, {15,15}, 0, crosshairColor);
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
			maxShoots = 10;
			rapidFireRate = 0.15f;
			InitGameplayVariables();
			createPowerups();
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
			UnloadTexture(ship);
			UnloadTexture(shipMoving);
			UnloadTexture(crosshair);
			UnloadTexture(asteroidExplosion);
			UnloadTexture(powerupMaxRapidFire);

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