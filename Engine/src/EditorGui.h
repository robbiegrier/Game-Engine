#ifndef _EditorGui
#define _EditorGui

#include "MathEngine.h"
#include <d3d11.h>

namespace Azul
{
	class Viewport;

	class EditorGui
	{
	public:
		static void Initialize();
		static void NewFrame();
		static void Draw();
		static void Cleanup();

		static void AlignForWidth(float width, float alignment = 0.5f);
		static void AlignForHeight(float height, float alignment = 0.5f);

	private:

		void Update();

		static EditorGui& GetInstance();

		void TreeBuildHelper(class PCSNode* pNode, bool& selection);
		static bool IsGoToRequested();

		Viewport* pWorldViewport;
	};
}

#endif