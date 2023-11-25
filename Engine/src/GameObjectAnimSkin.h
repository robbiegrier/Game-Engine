#ifndef _GameObjectAnimSkin
#define _GameObjectAnimSkin

#include "GameObjectAnim.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"
#include "Bone.h"

namespace Azul
{
	class GameObjectAnimSkin : public GameObjectAnim
	{
	public:
		// Big four
		GameObjectAnimSkin(GraphicsObject* graphicsObject, Bone* pBoneResult);
		GameObjectAnimSkin() = delete;
		GameObjectAnimSkin(const GameObjectAnimSkin&) = delete;
		GameObjectAnimSkin& operator=(GameObjectAnimSkin&) = delete;
		virtual ~GameObjectAnimSkin();

		virtual void Update(AnimTime currTime);

		void SetScale(float sx, float sy, float sz);
		void SetQuat(float qx, float qy, float qz, float qw);
		void SetTrans(float x, float y, float z);

		void SetScale(const Vec3& r);
		void SetQuat(const Quat& r);
		void SetTrans(const Vec3& r);

		virtual void SetIndex(int i) override;

		Mat4 GetBoneOrientation(void) const;
		void SetBoneOrientation(const Mat4&);

	private:
		void privUpdate(AnimTime currTime);

	public:
		Vec3* poScale;
		Quat* poQuat;
		Vec3* poTrans;
		Mat4* poLocal;
		Bone* pBoneResult;
		Mat4* poBoneOrientation;
	};
}

#endif
