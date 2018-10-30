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

	extern Rectangle backgroundMenuSource;
	extern Vector2 backgroundMenuOrigin;
	extern Rectangle backgroundMenuDestination;

	extern Texture2D backgroundMenu;
	extern Texture2D backgroundMenuBroken;

	#ifdef AUDIO

	//extern Sound ship_explode01;
	//extern Sound ship_shoot01;
	//extern Sound asteroid_explode01;
	//extern Sound powerup01;
	//extern Sound points01;
	//extern Sound button_navigate01;
	//extern Sound button_select01;

	//extern Music ship_rocket01;
	extern Music song_invasion;;

	#endif


	extern Buttons mouse;
	extern int buttonOption;

	extern int gameScreen;
	extern int screenWidth;
	extern int screenHeight;
	extern int defaultFontSize;

	extern bool isScreenFinished;

	extern Font mainFont;
	extern Font sideFont;

	void resolutionBackground();
	void checkAsteroidSprite();
	void Execute();
}
#endif // GAME_H