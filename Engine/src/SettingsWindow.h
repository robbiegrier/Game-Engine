#ifndef _SettingsWindow
#define _SettingsWindow

namespace Azul
{
	// Shows engine settings.
	class SettingsWindow
	{
	public:
		// Big four
		SettingsWindow() = default;
		SettingsWindow(const SettingsWindow&) = delete;
		SettingsWindow& operator=(const SettingsWindow&) = delete;
		~SettingsWindow() = default;

		void Update();
	};
}

#endif