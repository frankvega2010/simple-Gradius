#ifndef SETTINGS_H
#define SETTINGS_H
namespace Juego
{
	extern bool resolutionSmall;
	extern bool resolutionNormal;
	extern bool resolutionBig;

	extern int resolutionPositionFix;
	extern int gameTitleSizeFix;
	extern bool is800x600ResActive;
	extern bool is1920x1080ResActive;
	extern bool is1600x900ResActive;
	extern float songVolume;
	extern float soundVolume;

	namespace Settings_Section
	{
		void InitSettingsScreen();
		void UpdateSettingsScreen();
		void DrawSettings();
		bool FinishSettingsScreen();
		void DeInitSettingsResources();
	}
}
#endif // SETTINGS_H