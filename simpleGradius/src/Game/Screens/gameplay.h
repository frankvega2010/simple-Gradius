#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "raylib.h"

namespace Juego
{
	enum gameplayOptions {
		buttonContinue = 20,
		buttonRestart,
		buttonGameOver,
		buttonQuitToMenu
	};

	extern Texture2D asteroidExplosion;
	extern Texture2D ship;
	extern Texture2D shipMoving;
	extern Texture2D powerupMaxRapidFire;
	extern Texture2D powerupInvincibility;

	extern bool gameON;

	extern int scoreMultiplier;
	extern bool isExplosionActive;

	namespace Gameplay_Section
	{
		void InitGameplayVariables();
		void InitGameplayScreen();
		void UpdateGameplayScreen();
		void DrawGameplay();
		void DrawTimer(float widthvalue1, float widthvalue2, float heightvalue1);
		void RestartPhase();
		bool FinishGameplayScreen();
		void DeInitGameplayResources();
	}
}
#endif // GAMEPLAY_H