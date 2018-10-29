#include "gameplay.h"

#include "Setup/Game.h"
#include "Setup/Player.h"
#include "Setup\Enemy.h"
#include "Setup\PlayerShoot.h"
#include "Screens/settings.h"
#include "Screens\menu.h"

namespace Juego
{

	bool gameON = true;

	int playerKeys[MAX];

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

	static Rectangle pauseBoxRec;

	static bool isButtonSoundPlaying = false;
	static int buttonSelectSaveNumber = 0;

	Rectangle backgroundGameSource;
	Vector2 backgroundGameOrigin;

	Rectangle backgroundGameDestinationLayers[maxLayers];

	static Image backgroundLayer1Image;
	static Image backgroundLayer2Image;
	static Image backgroundLayer3Image;
	static Image backgroundLayer4Image;
	static Image backgroundLayer5Image;
	static Image backgroundLayer6Image;

	static Image backgroundLayerWaterEffectImage;
	static Image backgroundOpacityImage;

	static Image pauseMenuImage;

	static Image shipImage;
	static Image enemyShipImage;


	static float parallaxLayersPosition[maxLayers];
	static float parallaxLayersSpeed[maxLayers];

	static int parallaxLayersSpeedDecreaser;


	Texture2D backgroundLayers[maxLayers];
	Texture2D backgroundOpacity;
	Texture2D backgroundLayerWaterEffect;
	Texture2D pauseMenu;
	Texture2D ship;
	Texture2D enemyShip;

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
			pauseButton.defaultColor = DARKGREEN;
			pauseButton.messageColor = BLANK;
		}

		void InitGameplayVariables()
		{
			timerON = true;

			//HideCursor();
			createPauseButtons();
			createPlayer();
			createEnemy();
			createShoot();
		}

		void InitGameplayParallax()
		{
			if (resolutionNormal)
			{
				backgroundLayer1Image = LoadImage("res/assets/textures/background_layer1v2.png");
				ImageResize(&backgroundLayer1Image, screenWidth * 2, screenHeight);
				backgroundLayers[0] = LoadTextureFromImage(backgroundLayer1Image);

				backgroundLayer2Image = LoadImage("res/assets/textures/background_layer2v2.png");
				ImageResize(&backgroundLayer2Image, screenWidth * 2, screenHeight);
				backgroundLayers[1] = LoadTextureFromImage(backgroundLayer2Image);

				backgroundLayer3Image = LoadImage("res/assets/textures/background_layer3.png");
				ImageResize(&backgroundLayer3Image, screenWidth * 2, screenHeight);
				backgroundLayers[2] = LoadTextureFromImage(backgroundLayer3Image);

				backgroundLayer4Image = LoadImage("res/assets/textures/background_layer4.png");
				ImageResize(&backgroundLayer4Image, screenWidth * 2, screenHeight);
				backgroundLayers[3] = LoadTextureFromImage(backgroundLayer4Image);

				backgroundLayer5Image = LoadImage("res/assets/textures/background_layer5.png");
				ImageResize(&backgroundLayer5Image, screenWidth * 2, screenHeight);
				backgroundLayers[4] = LoadTextureFromImage(backgroundLayer5Image);

				backgroundLayer6Image = LoadImage("res/assets/textures/background_layer6.png");
				ImageResize(&backgroundLayer6Image, screenWidth * 2, screenHeight);
				backgroundLayers[5] = LoadTextureFromImage(backgroundLayer6Image);

				backgroundLayerWaterEffectImage = LoadImage("res/assets/textures/background_layer2v2effect.png");
				ImageResize(&backgroundLayerWaterEffectImage, screenWidth * 2, screenHeight);
				backgroundLayerWaterEffect = LoadTextureFromImage(backgroundLayerWaterEffectImage);

				backgroundOpacityImage = LoadImage("res/assets/textures/background_opacity.png");
				ImageResize(&backgroundOpacityImage, screenWidth * 2, screenHeight);
				backgroundOpacity = LoadTextureFromImage(backgroundOpacityImage);

				shipImage = LoadImage("res/assets/textures/player_ship01v2.png");
				ImageResize(&shipImage, 300, 70);// 150 70
				ship = LoadTextureFromImage(shipImage);

				enemyShipImage = LoadImage("res/assets/textures/enemy01.png");
				ImageResize(&enemyShipImage, 180, 70);
				enemyShip = LoadTextureFromImage(enemyShipImage);

				pauseMenuImage = LoadImage("res/assets/textures/pausemenu.png");
				ImageResize(&pauseMenuImage, pauseBoxRec.width, pauseBoxRec.height);
				pauseMenu = LoadTextureFromImage(pauseMenuImage);

				UnloadImage(backgroundLayer1Image);
				UnloadImage(backgroundLayer2Image);
				UnloadImage(backgroundLayer3Image);
				UnloadImage(backgroundLayer4Image);
				UnloadImage(backgroundLayer5Image);
				UnloadImage(backgroundLayer6Image);
				UnloadImage(backgroundLayerWaterEffectImage);
				UnloadImage(backgroundOpacityImage);
				UnloadImage(pauseMenuImage);
				UnloadImage(shipImage);
				UnloadImage(enemyShipImage);

				parallaxLayersSpeedDecreaser = 0;

				for (int i = maxLayers - 1; i >= 0; i--)
				{
					parallaxLayersPosition[i] = 0;
					backgroundGameDestinationLayers[i] = { parallaxLayersPosition[i],0, (float)screenWidth * 2,(float)screenHeight };
				}

				for (int i = maxLayers - 1; i >= 0; i--)
				{
					//parallaxLayersSpeed[i] = 0;
					parallaxLayersSpeed[i] = 50 + parallaxLayersSpeedDecreaser;
					parallaxLayersSpeedDecreaser = parallaxLayersSpeedDecreaser + 25; //250
					if (i == 0)
					{
						parallaxLayersSpeed[i] = 900;
					}
				}

				backgroundGameSource = { 0.0f,0.0f, (float)screenWidth * 2,(float)screenHeight };
				backgroundGameOrigin = { 0,0 };
			}
			else if (resolutionSmall)
			{
			}
		}

		void InitGameplayScreen()
		{
			if (resolutionNormal)
			{
				shipImage = LoadImage("res/assets/textures/player_ship01v2.png");
				ImageResize(&shipImage, 300, 70);// 150 70
				ship = LoadTextureFromImage(shipImage);

				enemyShipImage = LoadImage("res/assets/textures/enemy01.png");
				ImageResize(&enemyShipImage, 180, 70);
				enemyShip = LoadTextureFromImage(enemyShipImage);

				pauseMenuImage = LoadImage("res/assets/textures/pausemenu.png");
				ImageResize(&pauseMenuImage, pauseBoxRec.width, pauseBoxRec.height);
				pauseMenu = LoadTextureFromImage(pauseMenuImage);

				UnloadImage(pauseMenuImage);
				UnloadImage(shipImage);
				UnloadImage(enemyShipImage);

			}
			else if (resolutionSmall)
			{
			}
			//InitGameplayParallax();

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
				//crosshairColor = BLANK;
				gamePaused = true;
				gameON = false;
			}

			// Player Input
			if (gameON)
			{
				timerON = true;
				//crosshairColor = WHITE;
				//HideCursor();
				playerInput();
			}
			else ShowCursor();

			if (!gamePaused)
			{
				if (IsKeyPressed(KEY_ESCAPE))
				{
					//PlaySound(button_select01);
					//crosshairColor = BLANK;
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
				pauseButton.defaultColor = GREEN;
				pauseButton.selected = true;
			}
			else
			{
				pauseButton.defaultColor = DARKGREEN;
				pauseButton.selected = false;
			} 

			if (gameON)
			{
				for (int i = 0; i < maxLayers; i++)
				{
					backgroundGameDestinationLayers[i] = { parallaxLayersPosition[i],0, (float)screenWidth * 2,(float)screenHeight };
					parallaxLayersPosition[i] -= parallaxLayersSpeed[i] * GetFrameTime();

					if (parallaxLayersPosition[i] < 0 - screenWidth)
					{
						parallaxLayersPosition[i] = 0;
					}
				}

				for (int i = 0; i < maxEnemies; i++)
				{
					if (CheckCollisionRecs({ player.position.x,player.position.y,player.size.x,player.size.y }, { enemies[i].position.x,enemies[i].position.y,enemies[i].size.x,enemies[i].size.y }))
					{
						gameON = false;
						player.isAlive = false;
						buttonOption = buttonGameOver;
						isScreenFinished = true;
					}
				}

				

				playerUpdate();
				EnemyUpdate();
				shootUpdate();

			}
			else if (gamePaused)
			{
				timerON = false;
				//matchTimer = 0;

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


			if (targetsLeft == 0)
			{
				gameON = false;
				timerON = false;
				//matchTimer = 0;
				buttonOption = buttonGameOver;
				isScreenFinished = true;
			}
		}

		void DrawGameplay()
		{
			for (int i = maxLayers - 1; i >= 0; i--)
			{
				if (i == 1)
				{
					DrawTexturePro(backgroundLayerWaterEffect, backgroundGameSource, backgroundGameDestinationLayers[2], backgroundGameOrigin, 0, WHITE);
				}
				DrawTexturePro(backgroundLayers[i], backgroundGameSource, backgroundGameDestinationLayers[i], backgroundGameOrigin, 0, WHITE);
			}
			DrawTexturePro(backgroundOpacity, backgroundGameSource, backgroundGameDestinationLayers[0], backgroundGameOrigin, 0, WHITE);


			shootDraw();
			playerDraw();
			EnemyDraw();
			

			DrawRectangleLines(pauseButton.position.x, pauseButton.position.y, pauseButton.width, pauseButton.height, pauseButton.defaultColor);

			DrawTextEx(sideFont, FormatText("Targets:%i", targetsLeft), { 20, 20 }, defaultFontSize / 1.5f, 1.0f, GREEN);

			DrawTextEx(mainFont,"II", { pauseButton.position.x + 13, pauseButton.position.y + 7 }, defaultFontSize, 1.0f, pauseButton.defaultColor);
			//if (resolutionNormal && !(resolutionBig)) DrawText(FormatText(" ||"), pauseButton.position.x, pauseButton.position.y + 5, defaultFontSize, pauseButton.defaultColor);
			//else if (resolutionSmall) DrawText(FormatText(" ||"), pauseButton.position.x + 2, pauseButton.position.y + 5, defaultFontSize, pauseButton.defaultColor);
			//else if (resolutionBig && resolutionNormal) DrawText(FormatText(" ||"), pauseButton.position.x + 12, pauseButton.position.y + 15, defaultFontSize, pauseButton.defaultColor);
			

			if (!(gameON))
			{
				if (gamePaused)
				{
					DrawTexturePro(pauseMenu, { 0, 0, (float)screenWidth / 4.2f, (float)screenHeight / 2.5f }, pauseBoxRec, { 0,0 }, 0, WHITE);
					for (int i = 0; i < maxButtons; i++)
					{
						DrawRectangleLines(buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height, buttons[i].defaultColor);
					}
					DrawTextEx(mainFont, "PAUSED", { buttons[0].position.x - (screenWidth * 0.04f), 20 }, defaultFontSize, 1.0f, GREEN);
					DrawTextEx(sideFont, "CONTINUE", { buttons[0].position.x + 10, buttons[0].position.y + 5 }, defaultFontSize / 1.3, 1.0f, buttons[0].defaultColor);
					DrawTextEx(sideFont, "RESTART", { buttons[1].position.x + 8, buttons[1].position.y + 5 }, defaultFontSize / 1.3, 1.0f, buttons[1].defaultColor);
					DrawTextEx(sideFont, "MENU", { buttons[2].position.x + 10, buttons[2].position.y + 5 }, defaultFontSize / 1.3, 1.0f, buttons[2].defaultColor);
				}
			}
		}

		void RestartPhase()
		{
			InitGameplayVariables();
			player.isAlive = true;
			buttonDistance = 0;
			gameON = true;
			timerON = true;
			//matchSeconds = 0;
			//matchMinutes = 0;
			//matchHours = 0;
			scoreMultiplier = 5;
			targetsLeft = 50;
		}

		bool FinishGameplayScreen()
		{
			buttonDistance = 0;
			return isScreenFinished;
		}

		void DeInitGameplayParallax()
		{
			for (int i = 0; i < maxLayers; i++)
			{
				UnloadTexture(backgroundLayers[i]);
			}
		}

		void DeInitGameplayResources()
		{
			UnloadTexture(ship);
			UnloadTexture(enemyShip);
			UnloadTexture(pauseMenu);

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