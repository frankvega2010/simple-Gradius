#include "Game.h"

#include "Setup/Player.h"
#include "Screens/gameplay.h"
#include "Screens/menu.h"
#include "Screens/gameover.h"
#include "Screens/settings.h"
#include "Screens/credits.h"
#include "Screens/controls.h"

using namespace Juego;
using namespace Gameplay_Section;
using namespace GameOver_Section;
using namespace Menu_Section;
using namespace Settings_Section;
using namespace Credits_Section;
using namespace Controls_Section;

namespace Juego
{
	int gameScreen = Menu;

	int screenWidth;
	int screenHeight;

	Buttons mouse;
	int buttonOption = 0;
	int defaultFontSize = 60;
	bool isScreenFinished;

	Rectangle backgroundMenuSource;
	Vector2 backgroundMenuOrigin;
	Rectangle backgroundMenuDestination;

	static Image backgroundMenuImage;
	static Image backgroundMenuBrokenImage;
	Texture2D backgroundMenu;
	Texture2D backgroundMenuBroken;

	Font mainFont;
	Font sideFont;

	#ifdef AUDIO

	Sound ship_explode01;
	Sound ship_shoot01;
	Sound enemy_explode01;
	Sound button_navigate01;
	Sound button_select01;

	Music ship_rocket01;
	Music song_invasion;

	#endif

	static void createMouse()
	{
		mouse.position = { (float)GetMouseX(),(float)GetMouseY() };
		mouse.width = 0;
		mouse.height = 0;
		mouse.selected = false;
		mouse.defaultColor = BLANK;
	}

	void resolutionBackground()
	{
		backgroundMenuBrokenImage = LoadImage("res/assets/textures/backgroundmenubroken.png");
		ImageResize(&backgroundMenuBrokenImage, screenWidth, screenHeight);
		backgroundMenuBroken = LoadTextureFromImage(backgroundMenuBrokenImage);
		UnloadImage(backgroundMenuBrokenImage);

		backgroundMenuImage = LoadImage("res/assets/textures/backgroundmenu.png");
		ImageResize(&backgroundMenuImage, screenWidth, screenHeight);
		backgroundMenu = LoadTextureFromImage(backgroundMenuImage);
		UnloadImage(backgroundMenuImage);
	}

	void checkAsteroidSprite()
	{
		if (resolutionNormal)
		{
		}
		else if (resolutionSmall)
		{
		}
	}

	static void Init()
	{
		SetExitKey(0);

		screenWidth = 1300;//1300
		screenHeight = 800;//800

		createMouse();

		InitWindow(screenWidth, screenHeight, "Simple! Gradius");

		mainFont = LoadFont("res/assets/fonts/bigmacca.ttf");
		sideFont = LoadFont("res/assets/fonts/Reality_Pursuit_NC.ttf");

		resolutionBackground();

		playerKeys[UP] = KEY_UP;
		playerKeys[DOWN] = KEY_DOWN;
		playerKeys[SHOOT] = KEY_SPACE;

		#ifdef AUDIO
		InitAudioDevice();
		song_invasion = LoadMusicStream("res/assets/music/invasion.ogg");
		SetMusicVolume(song_invasion , songVolume);

		button_navigate01 = LoadSound("res/assets/sounds/select1.wav");
		SetSoundVolume(button_navigate01, soundVolume);

		button_select01 = LoadSound("res/assets/sounds/select2.wav");
		SetSoundVolume(button_select01, soundVolume);
		#endif

		InitMenuScreen();
	}

	static void Update()
	{
		switch (gameScreen)
		{
		case Play:
		{
			UpdateGameplayScreen();

			if (FinishGameplayScreen())
			{
				DeInitGameplayResources();
				switch (buttonOption)
				{
				case buttonQuitToMenu:
				{
					DeInitGameplayParallax();
					gameScreen = Menu;
					InitMenuScreen();
					break;
				}
				case buttonGameOver:
				{
					gameScreen = GameOver;
					InitGameOverScreen();
					break;
				}
				}
			}
		}
		break;
		case Menu:
		{
			UpdateMenuScreen();

			if (FinishMenuScreen())
			{
				DeInitMenuResources();
				switch (buttonOption)
				{
				case buttonPlay:
				{
					#ifdef AUDIO
					StopMusicStream(song_invasion);
					#endif
					InitGameplayParallax();
					RestartPhase();
					gameScreen = Play;
					InitGameplayScreen();
					break;
				}
				case buttonControls:
				{
					gameScreen = Controls;
					InitControlsScreen();
					break;
				}
				case buttonSettings:
				{
					gameScreen = Settings;
					InitSettingsScreen();
					break;
				}
				case buttonCredits:
				{
					gameScreen = Credits;
					InitCreditsScreen();
					break;
				}
				case buttonQuit:
				{
					#ifdef AUDIO
					StopMusicStream(song_invasion);		//StopMusicStream(pong_menu_song);
					#endif

					gameScreen = 0;
					return;
					break;
				}
				}
			}
		}
		break;
		case Controls:
		{
			UpdateControlsScreen();

			if (FinishControlsScreen())
			{
				DeInitControlsResources();
				gameScreen = Menu;
				InitMenuScreen();
			}
		}
		break;
		case Credits:
		{
			UpdateCreditsScreen();

			if (FinishCreditsScreen())
			{
				gameScreen = Menu;
				InitMenuScreen();
			}
		}
		break;
		case Settings:
		{
			UpdateSettingsScreen();

			if (FinishSettingsScreen())
			{
				DeInitSettingsResources();
				gameScreen = Menu;
				InitMenuScreen();
			}
		}
		break;
		case GameOver:
		{
			UpdateGameOverScreen();

			if (FinishGameOverScreen())
			{
				DeInitGameOverResources();
				switch (buttonOption)
				{
				case buttonRestart:
				{
					RestartPhase();
					gameScreen = Play;
					InitGameplayScreen();
					break;
				}
				case buttonQuitToMenu:
				{
					DeInitGameplayParallax();
					gameScreen = Menu;
					InitMenuScreen();
					break;
				}
				}
			}
		}
		break;
		}
	}

	static void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);
		

		switch (gameScreen)
		{
		case Play: Gameplay_Section::DrawGameplay(); break;
		case GameOver: GameOver_Section::DrawGameOver(); break;
		case Menu: Menu_Section::DrawMenu(); break;
		case Controls: Controls_Section::DrawControls(); break;
		case Credits: Credits_Section::DrawCredits(); break;
		case Settings: Settings_Section::DrawSettings(); break;
		}

		
		EndDrawing();
	}

	static void DeInit()
	{
#ifdef AUDIO
		StopSound(button_navigate01);
		StopSound(button_select01);
		UnloadSound(button_navigate01);
		UnloadSound(button_select01);
		UnloadMusicStream(song_invasion);
		CloseAudioDevice();
#endif
		UnloadFont(mainFont);
		UnloadFont(sideFont);
		UnloadTexture(backgroundMenu);
		UnloadTexture(backgroundMenuBroken);
		CloseWindow();
	}

	void Execute()
	{
		// Game Loop
		Init();
		while (!WindowShouldClose())
		{
			Update();
			if (gameScreen == 0) return;
			Draw();
		}
		DeInit();
		
	}
}