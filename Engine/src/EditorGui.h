#ifndef _EditorGui
#define _EditorGui

namespace Azul
{
	class EditorGui
	{
	public:

		static void Initialize();
		static void NewFrame();
		static void Draw();
		static void Cleanup();

	private:

		void Update();

		static EditorGui& GetInstance();

		void TreeBuildHelper(class PCSNode* pNode, bool& selection);
		static bool IsGoToRequested();
	};
}

#endif