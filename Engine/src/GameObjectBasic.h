#ifndef _GameObjectBasic
#define _GameObjectBasic

#include "GameObjectRigid.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"

namespace Azul
{
	class GameObjectBasic : public GameObjectRigid
	{
	public:
		// Big four
		GameObjectBasic(GraphicsObject* graphicsObject);
		GameObjectBasic() = delete;
		GameObjectBasic(const GameObjectBasic&) = delete;
		GameObjectBasic& operator=(GameObjectBasic&) = delete;
		virtual ~GameObjectBasic();

		virtual void Update(AnimTime currTime);

		void SetScale(float sx, float sy, float sz);
		void SetQuat(float qx, float qy, float qz, float qw);
		void SetTrans(float x, float y, float z);

		void SetScale(const Vec3& r);
		void SetQuat(const Quat& r);
		void SetTrans(const Vec3& r);

	private:
		void privUpdate(AnimTime currTime);

	public:
		Vec3* poScale;
		Quat* poQuat;
		Vec3* poTrans;

		float delta_x;
		float delta_y;
		float delta_z;

		float cur_rot_x;
		float cur_rot_y;
		float cur_rot_z;
	};
}

#endif

// --- End of File ---
