#ifndef _GameObjectAnimSkin
#define _GameObjectAnimSkin

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
	class AnimatedObjectHierarchy;

	class GameObjectAnimSkin : public GameObjectAnim
	{
	public:
		// Big four
		GameObjectAnimSkin(GraphicsObject* pGraphicsObject, AnimatedObjectHierarchy* _pAnimatedObjectHierarchy, BoneTransform* pInBoneResult);
		GameObjectAnimSkin() = delete;
		GameObjectAnimSkin(const GameObjectAnimSkin&) = delete;
		GameObjectAnimSkin& operator=(GameObjectAnimSkin&) = delete;
		virtual ~GameObjectAnimSkin();

		virtual void Update(AnimTime currTime);
		virtual void SetIndex(int i) override;

	public:
		BoneTransform* pBoneResult;
		Mat4* poBoneWorld;
		AnimatedObjectHierarchy* pAnimationHierarchy;

	private:
		void UpdateSkinGPU();
	};
}

#endif
