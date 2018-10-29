#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "raylib.h"

namespace Juego
{
	enum gameplayOptions 
	{
		buttonContinue = 20,
		buttonRestart,
		buttonGameOver,
		buttonQuitToMenu
	};

	enum playerKeysNumber
	{
		UP = 8,
		DOWN,
		SHOOT,
		MAX
	};

	extern bool gameON;
	extern int playerKeys[MAX];
	/*extern int moveUPKey;
	extern int moveDOWNKey;
	extern int shootKey;*/

	extern int scoreMultiplier;
	extern bool isExplosionActive;

	const int maxLayers = 6;
	extern Texture2D backgroundLayers[maxLayers];

	//extern Texture2D ship;
	//extern Texture2D enemyShip;
	//extern Texture2D pauseMenu;

	extern Texture2D backgroundLayers[maxLayers];
	extern Texture2D backgroundOpacity;
	extern Texture2D backgroundLayerWaterEffect;
	extern Texture2D pauseMenu;
	extern Texture2D ship;
	extern Texture2D enemyShip;

	namespace Gameplay_Section
	{
		void InitGameplayVariables();
		void InitGameplayParallax();
		void InitGameplayScreen();
		void UpdateGameplayScreen();
		void DrawGameplay();
		//void DrawTimer(float widthvalue1, float widthvalue2, float heightvalue1);
		void RestartPhase();
		bool FinishGameplayScreen();
		void DeInitGameplayParallax();
		void DeInitGameplayResources();
	}
}
#endif // GAMEPLAY_H