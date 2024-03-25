#ifndef _GameObjectAnimSkeleton
#define _GameObjectAnimSkeleton

#include "GameObjectAnim.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"
#include "BoneTransform.h"

namespace Azul
{
	class GameObjectAnimSkeleton : public GameObjectAnim
	{
	public:
		// Big four
		GameObjectAnimSkeleton(GraphicsObject* graphicsObject, BoneTransform* pBoneResult);
		GameObjectAnimSkeleton() = delete;
		GameObjectAnimSkeleton(const GameObjectAnimSkeleton&) = delete;
		GameObjectAnimSkeleton& operator=(GameObjectAnimSkeleton&) = delete;
		virtual ~GameObjectAnimSkeleton();

		virtual void Update(AnimTime currTime);

		virtual void SetIndex(int i) override;

		Mat4 GetBoneOrientation(void) const;
		void SetBoneOrientation(const Mat4&);

	public:
		BoneTransform* pBoneResult;
		Mat4* poBoneOrientation;
	};
}

#endif
