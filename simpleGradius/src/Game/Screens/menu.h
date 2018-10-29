#ifndef MENU_H
#define MENU_H
namespace Juego
{
	enum menuOptions {
		buttonPlay = 10,
		buttonControls,
		buttonSettings,
		buttonCredits,
		buttonQuit
	};

	namespace Menu_Section
	{
		void InitMenuScreen();
		void UpdateMenuScreen();
		void DrawBackground();
		void DrawBackgroundBroken();
		void DrawMenu();
		bool FinishMenuScreen();
		void DeInitMenuResources();
	}
}
#endif // MENU_H