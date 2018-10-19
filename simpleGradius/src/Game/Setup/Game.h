#ifndef GAME_H
#define GAME_H

#include <string>

#include "raylib.h"

using namespace std;

namespace Juego
{
	enum Screens
	{
		Play = 1,
		Menu,
		Controls,
		GameOver,
		Quit,
		Credits,
		Settings
	};

	struct Buttons {
		Vector2 position;
		float width;
		float height;
		bool selected;
		Color messageColor;
		Color defaultColor;
	};

	#ifdef AUDIO

	extern Sound ship_explode01;
	extern Sound ship_shoot01;
	extern Sound asteroid_explode01;
	extern Sound powerup01;
	extern Sound points01;
	extern Sound button_navigate01;
	extern Sound button_select01;

	extern Music ship_rocket01;
	extern Music song_alert;

	#endif


	extern Buttons mouse;
	extern int buttonOption;

	extern int gameScreen;
	extern int screenWidth;
	extern int screenHeight;
	extern int defaultFontSize;

	extern bool isScreenFinished;

	void resolutionBackground();
	void checkAsteroidSprite();
	void DrawBackground();
	void Execute();
}
#endif // GAME_H