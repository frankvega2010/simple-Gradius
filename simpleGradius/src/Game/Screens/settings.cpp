#include "settings.h"

#include "raylib.h"
#include "Setup/Game.h"
#include "Screens/gameplay.h"
#include "Screens\controls.h"
#include "Setup\Player.h"
#include "Screens\menu.h"

using namespace Juego;
using namespace Gameplay_Section;
using namespace Menu_Section;

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

	static const int maxButtons = 11;
	static const int maxButtonsRight = 4;
	static const int maxSliders = 2;

	static int resolutionSettingsFontSize = defaultFontSize / 1.25f;

	static Buttons buttonsSettings[maxButtons];

	static int buttonSelect = 0;
	static int buttonDistanceSettings = 0;
	static int buttonDistanceKeys = 0;

	int resolutionPositionFix = 0;
	int gameTitleSizeFix = 0;
	bool is800x600ResActive = false;
	bool is1920x1080ResActive = false;
	bool is1600x900ResActive = false;

	static bool assignKeyUp = false;
	static bool assignKeyDown = false;
	static bool assignKeyShoot = false;

	static bool assignKeys[MAX] = { false };

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
				buttonsSettings[i].position.x = (float)screenWidth / 30.0f;
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
					buttonsSettings[i].position.x = (float)screenWidth / 25.0f;
					buttonsSettings[i].position.y = (float)screenHeight / 1.15f;
					buttonsSettings[i].width = (float)screenWidth / 5.0f;
				}

				if (i > 7)
				{
					buttonsSettings[i].position.x = (float)screenWidth / 3.3f + buttonDistanceKeys;
					buttonsSettings[i].position.y = (float)screenHeight / 1.35f;
					buttonsSettings[i].width = (float)screenWidth / 5.0f;
					if (resolutionNormal && !(resolutionBig))
					{
						if(is1600x900ResActive) buttonDistanceKeys = buttonDistanceKeys + 360;
						else buttonDistanceKeys = buttonDistanceKeys + 300;
					}
					else if (resolutionSmall)
					{
						if(is800x600ResActive) buttonDistanceKeys = buttonDistanceKeys + 170;//170
						else buttonDistanceKeys = buttonDistanceKeys + 220;//170
					}
					else if (resolutionBig && resolutionNormal)
					{
						if (is1920x1080ResActive) buttonDistanceKeys = buttonDistanceKeys + 400;
						else buttonDistanceKeys = buttonDistanceKeys + 350;
					}
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
			ship_rocket01 = LoadMusicStream("res/assets/sounds/soundtest.ogg");
			SetMusicVolume(ship_rocket01, soundVolume);

			SetSoundVolume(button_select01, soundVolume);
			SetSoundVolume(button_navigate01, soundVolume);
			#endif

			for (int i = 0; i < MAX ; i++)
			{
				assignKeys[i] = false;
			}

			createSettingsButtons();

			isScreenFinished = false;
		}

		static void ChangeResolutionSmall(int screenW,int screenH)
		{
			resolutionPositionFix = 0;
			buttonDistanceSettings = 0;
			buttonDistanceKeys = 0;
			resolutionSmall = true;
			resolutionNormal = false;
			resolutionBig = false;
			screenWidth = screenW;
			screenHeight = screenH;
			resolutionBackground();
			defaultFontSize = 60 / 1.6;
			resolutionSettingsFontSize = defaultFontSize / 1.3f;
			if (is800x600ResActive)
			{
				gameTitleSizeFix = 5;
				resolutionControlsFontSize = defaultFontSize / 2.2f;
			}
			else
			{
				gameTitleSizeFix = 0;
				resolutionControlsFontSize = defaultFontSize / 1.8f;
			}
			SetWindowSize(screenWidth, screenHeight);
			createSettingsButtons();
		}

		static void ChangeResolutionNormal(int screenW, int screenH)
		{
			//is800x600ResActive = false;
			resolutionPositionFix = 0;
			buttonDistanceSettings = 0;
			buttonDistanceKeys = 0;
			resolutionSmall = false;
			resolutionNormal = true;
			resolutionBig = false;
			screenWidth = screenW;
			screenHeight = screenH;
			resolutionBackground();
			defaultFontSize = 60;
			resolutionSettingsFontSize = defaultFontSize / 1.25f;
			resolutionControlsFontSize = defaultFontSize / 2.2f;
			SetWindowSize(screenWidth, screenHeight);
			createSettingsButtons();
		}

		static void ChangeResolutionBig(int screenW, int screenH)
		{
			//is800x600ResActive = false;
			resolutionPositionFix = 30;
			buttonDistanceSettings = 0;
			buttonDistanceKeys = 0;
			resolutionSmall = false;
			resolutionNormal = true;
			resolutionBig = true;
			screenWidth = screenW;
			screenHeight = screenH;
			resolutionBackground();
			defaultFontSize = 70;
			resolutionSettingsFontSize = defaultFontSize / 1.25f;
			resolutionControlsFontSize = defaultFontSize / 2.2f;
			SetWindowSize(screenWidth, screenHeight);
			createSettingsButtons();
		}

		static void SettingsInput()
		{

			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && volumeSliders[Music].Selected)
			{
				volumeSliders[Music].shape.x = mouse.position.x;

				SetMusicVolume(song_invasion, songVolume);
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

			for (int i = 0; i < MAX; i++)
			{
				if (assignKeys[i])
				{
					if (IsKeyPressed(GetKeyPressed()))
					{
						playerKeys[i] = GetKeyPressed();
						assignKeys[i] = false;
					}

					else if (IsKeyPressed(KEY_UP))
					{
						playerKeys[i] = KEY_UP;
						assignKeys[i] = false;
					}

					else if (IsKeyPressed(KEY_DOWN))
					{
						playerKeys[i] = KEY_DOWN;
						assignKeys[i] = false;
					}

					else if (IsKeyPressed(KEY_RIGHT))
					{
						playerKeys[i] = KEY_RIGHT;
						assignKeys[i] = false;
					}

					else if (IsKeyPressed(KEY_LEFT))
					{
						playerKeys[i] = KEY_LEFT;
						assignKeys[i] = false;
					}

					else if (IsKeyPressed(KEY_SPACE))
					{
						playerKeys[i] = KEY_SPACE;
						assignKeys[i] = false;
					}
				}
			}

			for (int i = 0; i < maxButtons; i++)
			{

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && buttonsSettings[i].selected || IsKeyPressed(KEY_ENTER) && buttonsSettings[i].selected && !(volumeSliders[Effects].Selected) && !(volumeSliders[Music].Selected))
				{
					PlaySound(button_select01);
					switch (i)
					{
					case 0:
						is1920x1080ResActive = true;
						ChangeResolutionBig(1920, 1080);
						break;
					case 1:
						is1920x1080ResActive = false;
						ChangeResolutionBig(1680, 1050);
						break;
					case 2:
						is1600x900ResActive = true;
						ChangeResolutionNormal(1600, 900);
						break;
					case 3:
						is1600x900ResActive = false;
						ChangeResolutionNormal(1440, 900);
						break;
					case 4:
						is1600x900ResActive = false;
						ChangeResolutionNormal(1300, 800);
						break;
					case 5:
						is800x600ResActive = false;
						ChangeResolutionSmall(1024, 768);
						break;
					case 6:
						is800x600ResActive = true;
						ChangeResolutionSmall(800, 600);
						break;
					case 7:
						buttonOption = buttonGoMenu;
						isScreenFinished = true;

						for (int i = 0; i < MAX; i++) assignKeys[i] = false;
						break;
					case 8:
						assignKeys[UP] = true;
						break;
					case 9:
						assignKeys[DOWN] = true;
						break;
					case 10:
						assignKeys[SHOOT] = true;
						break;
					}
					buttonsSettings[i].selected = false;
				}
			}
		}

		void UpdateSettingsScreen()
		{
			#ifdef AUDIO
			UpdateMusicStream(song_invasion);
			UpdateMusicStream(ship_rocket01);
			#endif

			volumeSliders[Music].shape.x = musicLine.PosStart.x + ((songVolume * 100) * 5);
			volumeSliders[Effects].shape.x = effectsLine.PosStart.x + ((soundVolume * 100) * 5);

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
						if(assignKeys[UP]) buttonsSettings[i].defaultColor = WHITE;
						else if (assignKeys[DOWN]) buttonsSettings[i].defaultColor = WHITE;
						else if (assignKeys[SHOOT]) buttonsSettings[i].defaultColor = WHITE;
						else buttonsSettings[i].defaultColor = GREEN;
						
						buttonsSettings[i].selected = true;
					}
					else
					{
						buttonsSettings[i].defaultColor = DARKGREEN;
						buttonsSettings[i].selected = false;
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

		void DrawSettings()
		{
			DrawBackground();

			DrawLineEx(musicLine.PosStart, musicLine.PosEnd, musicLine.Thick, musicLine.Color);
			DrawLineEx(effectsLine.PosStart, effectsLine.PosEnd, effectsLine.Thick, effectsLine.Color);
			DrawRectangleRec(volumeSliders[Music].shape, volumeSliders[Music].color);
			DrawRectangleRec(volumeSliders[Effects].shape, volumeSliders[Effects].color);

			for (int i = 0; i < maxButtons; i++)
			{
				DrawRectangleLines(buttonsSettings[i].position.x, buttonsSettings[i].position.y, buttonsSettings[i].width, buttonsSettings[i].height, buttonsSettings[i].defaultColor);

				if (CheckCollisionRecs({ mouse.position.x,  mouse.position.y, mouse.width, mouse.height }, { buttonsSettings[i].position.x, buttonsSettings[i].position.y, buttonsSettings[i].width, buttonsSettings[i].height }) || buttonSelect == i)
				{
					buttonsSettings[i].messageColor = GREEN;
				}
				else
				{
					buttonsSettings[i].messageColor = BLANK;
				}
			}

			DrawTextEx(sideFont, "1920x1080", { buttonsSettings[0].position.x + 10, buttonsSettings[0].position.y + 5 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[0].defaultColor);
			DrawTextEx(sideFont, "1680x1050", { buttonsSettings[1].position.x + 10, buttonsSettings[1].position.y + 5 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[1].defaultColor);
			DrawTextEx(sideFont, "1600x900",  { buttonsSettings[2].position.x + 10, buttonsSettings[2].position.y + 5 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[2].defaultColor);
			DrawTextEx(sideFont, "1440x900",  { buttonsSettings[3].position.x + 10, buttonsSettings[3].position.y + 5 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[3].defaultColor);
			DrawTextEx(sideFont, "1300x800",  { buttonsSettings[4].position.x + 10, buttonsSettings[4].position.y + 5 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[4].defaultColor);
			DrawTextEx(sideFont, "1024x768",  { buttonsSettings[5].position.x + 10, buttonsSettings[5].position.y + 5 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[5].defaultColor);
			DrawTextEx(sideFont, "800x600",   { buttonsSettings[6].position.x + 10, buttonsSettings[6].position.y + 5 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[6].defaultColor);
			DrawTextEx(mainFont, "MENU",	  { buttonsSettings[7].position.x + 35, buttonsSettings[7].position.y + 10 }, defaultFontSize / 1.5f, 1.0f, buttonsSettings[7].defaultColor);


			DrawTextEx(sideFont, "Music Volume", { musicLine.PosStart.x + 50, musicLine.PosStart.y - 100 }, resolutionSettingsFontSize, 1.0f, volumeSliders[Music].color);
			DrawTextEx(sideFont, FormatText("%f", songVolume), { musicLine.PosStart.x + 100, musicLine.PosStart.y + 50 }, resolutionSettingsFontSize, 1.0f, volumeSliders[Music].color);

			DrawTextEx(sideFont, "Sound Volume", { effectsLine.PosStart.x + 50, effectsLine.PosStart.y - 100 }, resolutionSettingsFontSize, 1.0f, volumeSliders[Effects].color);
			DrawTextEx(sideFont, FormatText("%f", soundVolume), { effectsLine.PosStart.x + 100, effectsLine.PosStart.y + 50 }, resolutionSettingsFontSize, 1.0f, volumeSliders[Effects].color);


			DrawTextEx(sideFont, "PLAYER CONTROLS CONFIG", { buttonsSettings[8].position.x + 120, buttonsSettings[8].position.y - 40 - resolutionPositionFix }, defaultFontSize / 1.5f, 1.0f, GREEN);
			
			for (int i = 0; i < MAX; i++)
			{
				switch (playerKeys[i])
				{
				case KEY_UP:
					DrawTextEx(sideFont, "UP", { buttonsSettings[i].position.x + 35, buttonsSettings[i].position.y + 10 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[i].defaultColor);
					break;
				case KEY_DOWN:
					DrawTextEx(sideFont, "DOWN", { buttonsSettings[i].position.x + 35, buttonsSettings[i].position.y + 10 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[i].defaultColor);
					break;
				case KEY_LEFT:
					DrawTextEx(sideFont, "LEFT", { buttonsSettings[i].position.x + 35, buttonsSettings[i].position.y + 10 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[i].defaultColor);
					break;
				case KEY_RIGHT:
					DrawTextEx(sideFont, "RIGHT", { buttonsSettings[i].position.x + 35, buttonsSettings[i].position.y + 10 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[i].defaultColor);
					break;
				case KEY_SPACE:
					DrawTextEx(sideFont, "SPACE", { buttonsSettings[i].position.x + 35, buttonsSettings[i].position.y + 10 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[i].defaultColor);
					break;
				default:
					DrawTextEx(sideFont, FormatText("%c", playerKeys[i]), { buttonsSettings[i].position.x + 35, buttonsSettings[i].position.y + 10 }, resolutionSettingsFontSize, 1.0f, buttonsSettings[i].defaultColor);
					break;
				}
			}

			DrawTextEx(sideFont, "MOVE UP", { buttonsSettings[8].position.x + 35, buttonsSettings[8].position.y + 70 + resolutionPositionFix }, defaultFontSize / 1.5f, 1.0f, buttonsSettings[8].defaultColor);
			DrawTextEx(sideFont, "MOVE DOWN", { buttonsSettings[9].position.x + 5, buttonsSettings[9].position.y + 70 + resolutionPositionFix }, defaultFontSize / 1.5f, 1.0f, buttonsSettings[9].defaultColor);
			DrawTextEx(sideFont, "SHOOT", { buttonsSettings[10].position.x + 60, buttonsSettings[10].position.y + 70 + resolutionPositionFix }, defaultFontSize / 1.5f, 1.0f, buttonsSettings[10].defaultColor);

			DrawTextEx(sideFont, "ONLY UPPERCASE", { buttonsSettings[8].position.x + 200, buttonsSettings[8].position.y + 100 + resolutionPositionFix }, defaultFontSize / 1.5f, 1.0f, GREEN);
		}

		bool FinishSettingsScreen()
		{
			return isScreenFinished;
		}

		void DeInitSettingsResources()
		{
			buttonDistanceSettings = 0;
			buttonDistanceKeys = 0;
			#ifdef AUDIO
			StopMusicStream(ship_rocket01);
			UnloadMusicStream(ship_rocket01);
			#endif
		}
	}
}