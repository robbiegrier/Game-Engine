#ifndef _AssetsWindow
#define _AssetsWindow

namespace Azul
{
	// Shows assets that can be spawned
	class AssetsWindow
	{
	public:
		// Big four
		AssetsWindow() = default;
		AssetsWindow(const AssetsWindow&) = delete;
		AssetsWindow& operator=(const AssetsWindow&) = delete;
		~AssetsWindow() = default;

		void Update();
	};
}

#endif