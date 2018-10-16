#include "settings.h"

#include "raylib.h"
#include "Setup/Game.h"
#include "Screens/gameplay.h"
#include "Setup/Asteroid.h"
#include "Screens\controls.h"
#include "Setup\Player.h"

using namespace Juego;
using namespace Gameplay_Section;

namespace Juego
{
	bool resolutionSmall = false;
	bool resolutionNormal = true;
	bool resolutionBig = false;

	float songVolume = 0.80;
	float soundVolume = 0.60;

	enum SlidersNames
	{
		Music,
		Effects
	};

	struct volumeLine 
	{
		Vector2 PosStart;
		Vector2 PosEnd;
		int Thick;
		Color Color;
	};

	struct Slider
	{
		Rectangle shape;
		Color color;
		bool Selected;
	};

	static const int maxButtons = 8;
	static const int maxButtonsRight = 4;
	static const int maxSliders = 2;

	static Buttons buttonsSettings[maxButtons];

	static int buttonSelect = 0;
	static int buttonDistanceSettings = 0;

	static int musicLineCounter = 0;
	static float musicLineCounterVolume = 0.8;

	static int effectsLineCounter = 0;
	static float effectsLineCounterVolume = 0.6;

	static Slider volumeSliders[maxSliders];
	static int maxVolumeValues = 101;
	static int sliderDistance = 5;
	static float sliderVolumeAmount = 0.01;

	static volumeLine musicLine;
	static volumeLine effectsLine;

	static bool isButtonSoundPlaying = false;
	static int buttonSelectSaveNumber = 0;

	static Image asteroidImage;

	namespace Settings_Section
	{
		static void createSettingsButtons()
		{
			musicLine.PosStart = { (float)screenWidth / 3.1f,(float)screenHeight / 4 };
			musicLine.PosEnd = { (float)screenWidth / 3.1f + 500,(float)screenHeight / 4 };
			musicLine.Thick = 10;
			musicLine.Color = WHITE;

			effectsLine.PosStart = { (float)screenWidth / 3.1f,(float)screenHeight / 1.8f };
			effectsLine.PosEnd = { (float)screenWidth / 3.1f + 500,(float)screenHeight / 1.8f };
			effectsLine.Thick = 10;
			effectsLine.Color = WHITE;


			volumeSliders[Music].shape = { musicLine.PosStart.x,musicLine.PosStart.y - 50,25,100 };
			volumeSliders[Effects].shape = { effectsLine.PosStart.x,effectsLine.PosStart.y - 50,25,100 };

			for (int i = 0; i < maxSliders; i++)
			{
				volumeSliders[i].color = WHITE;
				volumeSliders[i].Selected = false;
			}

			for (int i = 0; i < maxButtons; i++)
			{
				buttonsSettings[i].position.x = (float)screenWidth / 90.0f;
				buttonsSettings[i].position.y = (float)screenHeight / 10.0f + buttonDistanceSettings;
				if (resolutionNormal)
				{
					buttonsSettings[i].width = (float)screenWidth / 4.0f;
					buttonsSettings[i].height = (float)screenHeight / 12.0f;
				}
				else if (resolutionSmall)
				{
					buttonsSettings[i].width = (float)screenWidth / 4.0f;
					buttonsSettings[i].height = (float)screenHeight / 14.0f;//18.0f default
				}
				buttonsSettings[i].selected = false;
				buttonsSettings[i].defaultColor = RED;
				buttonsSettings[i].messageColor = BLANK;

				if (i == 7)
				{
					buttonsSettings[i].position.y = (float)screenHeight / 1.1f;
					buttonsSettings[i].width = (float)screenWidth / 5.0f;
				}

				
				if (resolutionNormal && !(resolutionBig))
				{
					buttonDistanceSettings = buttonDistanceSettings + 85;
				}
				else if (resolutionSmall)
				{
					buttonDistanceSettings = buttonDistanceSettings + 60;
				}
				else if (resolutionBig && resolutionNormal)
				{
					buttonDistanceSettings = buttonDistanceSettings + 100;
				}
				
				
			}
		}

		void InitSettingsScreen()
		{
			#ifdef AUDIO
			ship_rocket01 = LoadMusicStream("res/sounds/ship_rocket01.ogg");
			SetMusicVolume(ship_rocket01, soundVolume);

			SetSoundVolume(button_select01, soundVolume);
			SetSoundVolume(button_navigate01, soundVolume);
			#endif

			checkAsteroidSprite();
			createSettingsButtons();

			isScreenFinished = false;
		}

		static void ChangeResolutionSmall(int screenW,int screenH)
		{
			resolutionSmall = true;
			resolutionNormal = false;
			resolutionBig = false;
			screenWidth = screenW;
			screenHeight = screenH;
			resolutionBackground();
			defaultFontSize = 60 / 1.6;
			SetWindowSize(screenWidth, screenHeight);
			createSettingsButtons();
			checkAsteroidSprite();
			createAsteroid();
		}

		static void ChangeResolutionNormal(int screenW, int screenH)
		{
			resolutionSmall = false;
			resolutionNormal = true;
			resolutionBig = false;
			screenWidth = screenW;
			screenHeight = screenH;
			resolutionBackground();
			defaultFontSize = 60;
			SetWindowSize(screenWidth, screenHeight);
			createSettingsButtons();
			checkAsteroidSprite();
			createAsteroid();
		}

		static void ChangeResolutionBig(int screenW, int screenH)
		{
			resolutionSmall = false;
			resolutionNormal = true;
			resolutionBig = true;
			screenWidth = screenW;
			screenHeight = screenH;
			resolutionBackground();
			defaultFontSize = 60;
			SetWindowSize(screenWidth, screenHeight);
			createSettingsButtons();
			checkAsteroidSprite();
			createAsteroid();
		}

		static void SettingsInput()
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

			//Music Volume Settings
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && volumeSliders[Music].Selected)
			{
				volumeSliders[Music].shape.x = mouse.position.x;

				SetMusicVolume(song_alert, songVolume);
				for (int i = 0; i < maxVolumeValues; i++)
				{
					if (volumeSliders[Music].shape.x >= musicLine.PosStart.x + musicLineCounter) songVolume = musicLineCounterVolume;
					musicLineCounter = musicLineCounter + sliderDistance;
					musicLineCounterVolume = musicLineCounterVolume + sliderVolumeAmount;
				}
				musicLineCounter = 0;
				musicLineCounterVolume = 0.0;
				

				if (volumeSliders[Music].shape.x < musicLine.PosStart.x) volumeSliders[Music].shape.x = musicLine.PosStart.x;
				else if (volumeSliders[Music].shape.x > musicLine.PosEnd.x) volumeSliders[Music].shape.x = musicLine.PosEnd.x;
			}


			//Sound Effects Volume Settings
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && volumeSliders[Effects].Selected)
			{
				volumeSliders[Effects].shape.x = mouse.position.x;
				
				for (int i = 0; i < maxVolumeValues; i++)
				{
					if (volumeSliders[Effects].shape.x >= effectsLine.PosStart.x + effectsLineCounter) soundVolume = effectsLineCounterVolume;
					effectsLineCounter = effectsLineCounter + sliderDistance;
					effectsLineCounterVolume = effectsLineCounterVolume + sliderVolumeAmount;
					PlayMusicStream(ship_rocket01);
					SetMusicVolume(ship_rocket01, soundVolume);
				}
				effectsLineCounter = 0;
				effectsLineCounterVolume = 0.0;

				SetSoundVolume(button_select01, soundVolume);
				SetSoundVolume(button_navigate01, soundVolume);

				if (volumeSliders[Effects].shape.x < effectsLine.PosStart.x) volumeSliders[Effects].shape.x = effectsLine.PosStart.x;
				else if (volumeSliders[Effects].shape.x > effectsLine.PosEnd.x) volumeSliders[Effects].shape.x = effectsLine.PosEnd.x;
			}
			else
			{
				StopMusicStream(ship_rocket01);
			}

			for (int i = 0; i < maxButtons; i++)
			{

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && buttonsSettings[i].selected || IsKeyPressed(KEY_ENTER) && buttonsSettings[i].selected && !(volumeSliders[Effects].Selected) && !(volumeSliders[Music].Selected))
				{
					PlaySound(button_select01);
					switch (i)
					{
					case 0:
						ChangeResolutionBig(1920, 1080);
						break;
					case 1:
						ChangeResolutionBig(1680, 1050);
						break;
					case 2:
						ChangeResolutionNormal(1600, 900);
						break;
					case 3:
						ChangeResolutionNormal(1440, 900);
						break;
					case 4:
						ChangeResolutionNormal(1300, 800);
						break;
					case 5:
						ChangeResolutionSmall(1024, 768);
						break;
					case 6:
						ChangeResolutionSmall(800, 600);
						break;
					case 7:
						buttonOption = buttonGoMenu;
						isScreenFinished = true;
						break;
					}
					buttonsSettings[i].selected = false;
				}
			}
		}

		void UpdateSettingsScreen()
		{
			#ifdef AUDIO
			UpdateMusicStream(song_alert);
			UpdateMusicStream(ship_rocket01);
			#endif

			volumeSliders[Music].shape.x = musicLine.PosStart.x + ((songVolume*100)*5);
			volumeSliders[Effects].shape.x = effectsLine.PosStart.x + ((soundVolume * 100) * 5);

			asteroidUpdate();
			mouse.position = { (float)GetMouseX(),(float)GetMouseY() };

			SettingsInput();

			for (int s = 0; s < maxSliders; s++)
			{
				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { volumeSliders[s].shape.x, volumeSliders[s].shape.y, volumeSliders[s].shape.width, volumeSliders[s].shape.height }))
				{
					volumeSliders[s].color = GREEN;
					volumeSliders[s].Selected = true;
				}
				else
				{
					volumeSliders[s].color = WHITE;
					volumeSliders[s].Selected = false;
				}
			}

			
				for (int i = 0; i < maxButtons; i++)
				{
					if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttonsSettings[i].position.x, buttonsSettings[i].position.y, buttonsSettings[i].width, buttonsSettings[i].height }) || buttonSelect == i && !(volumeSliders[Effects].Selected) && !(volumeSliders[Music].Selected))
					{
						buttonSelect = i;
						buttonsSettings[i].defaultColor = WHITE;
						buttonsSettings[i].selected = true;
					}
					else
					{
						buttonsSettings[i].defaultColor = RED;
						buttonsSettings[i].selected = false;
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

		void DrawSettings()
		{
			DrawBackground();
			asteroidDraw();

			DrawLineEx(musicLine.PosStart, musicLine.PosEnd, musicLine.Thick, musicLine.Color);
			DrawLineEx(effectsLine.PosStart, effectsLine.PosEnd, effectsLine.Thick, effectsLine.Color);
			DrawRectangleRec(volumeSliders[Music].shape, volumeSliders[Music].color);
			DrawRectangleRec(volumeSliders[Effects].shape, volumeSliders[Effects].color);

			for (int i = 0; i < maxButtons; i++)
			{
				DrawRectangleLines(buttonsSettings[i].position.x, buttonsSettings[i].position.y, buttonsSettings[i].width, buttonsSettings[i].height, buttonsSettings[i].defaultColor);

				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttonsSettings[i].position.x, buttonsSettings[i].position.y, buttonsSettings[i].width, buttonsSettings[i].height }) || buttonSelect == i)
				{
					buttonsSettings[i].messageColor = WHITE;

					switch (i)
					{
					case 7:
						DrawText("Click or Enter to go back!", buttonsSettings[i].position.x + (screenWidth / 4.5), buttonsSettings[i].position.y, defaultFontSize / 2, buttonsSettings[i].messageColor);
						DrawText("", buttonsSettings[i].position.x + (screenWidth / 4.5), buttonsSettings[i].position.y + 50, defaultFontSize / 2, buttonsSettings[i].messageColor);
						break;
					}
				}
				else
				{
					buttonsSettings[i].messageColor = BLANK;
				}
			}

			DrawText(FormatText("1920x1080"), buttonsSettings[0].position.x + 10, buttonsSettings[0].position.y + 5, defaultFontSize, buttonsSettings[0].defaultColor);
			DrawText(FormatText("1680x1050"), buttonsSettings[1].position.x + 10, buttonsSettings[1].position.y + 5, defaultFontSize, buttonsSettings[1].defaultColor);
			DrawText(FormatText("1600x900"), buttonsSettings[2].position.x + 10, buttonsSettings[2].position.y + 5, defaultFontSize, buttonsSettings[2].defaultColor);
			DrawText(FormatText("1440x900"), buttonsSettings[3].position.x + 10, buttonsSettings[3].position.y + 5, defaultFontSize, buttonsSettings[3].defaultColor);
			DrawText(FormatText("1300x800"), buttonsSettings[4].position.x + 10, buttonsSettings[4].position.y + 5, defaultFontSize, buttonsSettings[4].defaultColor);
			DrawText(FormatText("1024x768"), buttonsSettings[5].position.x + 10, buttonsSettings[5].position.y + 5, defaultFontSize, buttonsSettings[5].defaultColor);
			DrawText(FormatText("800x600"), buttonsSettings[6].position.x + 10, buttonsSettings[6].position.y + 5, defaultFontSize, buttonsSettings[6].defaultColor);

			DrawText(FormatText("MENU"), buttonsSettings[7].position.x + 25, buttonsSettings[7].position.y + 5, defaultFontSize, buttonsSettings[7].defaultColor);

			DrawText(FormatText("Music Volume"), musicLine.PosStart.x + 50, musicLine.PosStart.y - 100, defaultFontSize, volumeSliders[Music].color);
			DrawText(FormatText("%f",songVolume), musicLine.PosStart.x + 100, musicLine.PosStart.y + 50, defaultFontSize, volumeSliders[Music].color);

			DrawText(FormatText("Sound Volume"), effectsLine.PosStart.x + 50, effectsLine.PosStart.y - 100, defaultFontSize, volumeSliders[Effects].color);
			DrawText(FormatText("%f", soundVolume), effectsLine.PosStart.x + 100, effectsLine.PosStart.y + 50, defaultFontSize, volumeSliders[Effects].color);
		}

		bool FinishSettingsScreen()
		{
			buttonDistanceSettings = 0;
			return isScreenFinished;
		}

		void DeInitSettingsResources()
		{
			#ifdef AUDIO
			StopMusicStream(ship_rocket01);
			UnloadMusicStream(ship_rocket01);
			#endif
		}
	}
}