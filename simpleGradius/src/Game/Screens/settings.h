#ifndef SETTINGS_H
#define SETTINGS_H
namespace Juego
{
	extern bool resolutionSmall;
	extern bool resolutionNormal;
	extern bool resolutionBig;

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