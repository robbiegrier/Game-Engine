#ifndef _ConsoleWindow
#define _ConsoleWindow

namespace Azul
{
	// Shows a console, I should implement a real console
	class ConsoleWindow
	{
	public:
		// Big four
		ConsoleWindow() = default;
		ConsoleWindow(const ConsoleWindow&) = delete;
		ConsoleWindow& operator=(const ConsoleWindow&) = delete;
		~ConsoleWindow() = default;

		void Update();

		void Log(const char* msg, ...);

	private:
		std::string text;
	};
}

#endif
