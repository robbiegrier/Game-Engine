#ifndef _CSAnimationWorld
#define _CSAnimationWorld

#include "CSAnimationMixer.h"

namespace Azul
{
	struct BoneTransform;

	class CSAnimationWorld
	{
	public:
		CSAnimationWorld();
		CSAnimationWorld(const CSAnimationWorld&) = delete;
		CSAnimationWorld& operator=(const CSAnimationWorld&) = delete;
		~CSAnimationWorld();

		void Open(int* pTable, int hierarchyDepth, const Mat4& pivotWorld, Mat4* invBindArray);
		void Run();
		void AwaitResults(Mat4* pResultOut);
		void Close();

		void AttachTo(const CSAnimationMixer& computeShaderMixer);

		ID3D11ComputeShader* pComputeShader;
		ID3D11Buffer* pComputeOutBuffer = nullptr;

		struct CBAnimWorld
		{
			Mat4 pivotWorld;
			int hierarchyDepth;
			int pad0 = 0u;
			int pad1 = 0u;
			int pad2 = 0u;
		};
		ID3D11Buffer* pConstBuffAnimWorld;

		ID3D11Buffer* pConstBuffInvBindArray;

		ComputeUAVBuffer* pBoneLocal;
		ComputeRSVBuffer table;
		ComputeUAVBuffer worldResult;
	};
}

#endif