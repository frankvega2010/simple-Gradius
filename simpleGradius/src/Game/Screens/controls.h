#ifndef CONTROLS_H
#define CONTROLS_H

namespace Juego
{
	const int buttonGoMenu = 12;

	namespace Controls_Section
	{
		void InitControlsScreen();
		void UpdateControlsScreen();
		void DrawControls();
		bool FinishControlsScreen();
		void DeInitControlsResources();
	}
}
#endif // CONTROLS_H
