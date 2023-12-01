#ifndef _GameObject
#define _GameObject

#include <d3d11.h>
#include "MathEngine.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"

namespace Azul
{
	// A Game Object is an object with a transform in the game world
	// and a Graphics Object rendered at that transform.
	class GameObject : public PCSNode
	{
	public:
		// Big four
		GameObject(GraphicsObject* pInGraphicsObject);
		GameObject(const GameObject&) = delete;
		GameObject& operator = (const GameObject&) = delete;
		virtual ~GameObject();

		// Core update and draw
		virtual void Update(AnimTime deltaTime);
		void Draw();

		// Mutate transform
		void SetRelativeLocation(const Vec3& v);
		void SetRelativeScale(const Vec3& v);
		void SetRelativeScale(float s);
		void SetRelativeRotation(const Rot& m);

		// Accessors
		const Mat4& GetWorld() const;
		Vec3 GetLocation() const;
		const Vec3& GetRelativeLocation() const;
		Vec3& RelativeLocation();
		Quat& RelativeRotation();
		Vec3& RelativeScale();

		// Get the parent in the object heirarchy
		GameObject* GetParentGameObject() const;

		// Get the low level graphics object
		GraphicsObject* GetGraphicsObject() const;

		void RenderShell();
		void SetShellColor(const Vec4& inColor);

		void SetRenderShell(bool render);
		static void SetRenderShellGlobal(bool render);

		static float toSeconds(const AnimTime& abstractTime);

	protected:
		// Overridable tick method called once per frame
		virtual void Tick(AnimTime deltaTime) { static_cast<void>(deltaTime); }

		bool renderShell;
		static bool globalRenderShell;

	protected:
		// Itemized transform
		Vec3* pPos;
		Vec3* pScale;
		Quat* pRotation;

		// Combined transform as world matrix
		Mat4* pWorld;

		// Low level graphics object
		GraphicsObject* pGraphicsObject;

		GraphicsObject* pShell;
	};
}

#endif
