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
		void DrawMenu();
		bool FinishMenuScreen();
	}
}
#endif // MENU_H