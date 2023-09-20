#ifndef _GameObject
#define _GameObject

#include <d3d11.h>

#include "MathEngine.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "GraphicsObject.h"
#include "PCSNode.h"

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
		void Update(float deltaTime);
		void Draw();

		// Mutate transform
		void SetRelativeLocation(const Vec3& v);
		void SetRelativeScale(const Vec3& v);
		void SetRelativeRotation(const Mat4& m);

		// Accessors
		const Mat4& GetWorld() const;
		Vec3 GetLocation() const;
		const Vec3& GetRelativeLocation() const;

		// Get the parent in the object heirarchy
		GameObject* GetParentGameObject() const;

		// Get the low level graphics object
		GraphicsObject* GetGraphicsObject() const;

	protected:
		// Overridable tick method called once per frame
		virtual void Tick(float deltaTime) { static_cast<void>(deltaTime); }

	private:
		// Itemized transform
		Vec3* pPos;
		Vec3* pScale;
		Mat4* pRotation;

		// Combined transform as world matrix
		Mat4* pWorld;

		// Low level graphics object
		GraphicsObject* pGraphicsObject;
	};
}

#endif
