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
#include "DLinkedList.h"
#include "Component.h"

namespace Azul
{
	class Component;

	// A Game Object is an object with a transform in the game world
	// and a Graphics Object rendered at that transform.
	class GameObject : public PCSNode
	{
	public:
		enum class Name
		{
			GameObject,
			Player,
			GameObjectSprite,
			GameObjectText,
			None
		};

		// Big four
		GameObject(GraphicsObject* pInGraphicsObject);
		GameObject(const GameObject&) = delete;
		GameObject& operator = (const GameObject&) = delete;
		virtual ~GameObject();

		// Core update and draw
		virtual void Update(float deltaTime);
		virtual void Draw();

		virtual void Start() {}
		virtual void Tick(float deltaTime) { static_cast<void>(deltaTime); }

		// Mutate transform
		void SetRelativeLocation(const Vec3& v);
		void SetRelativeScale(const Vec3& v);
		void SetRelativeScale(float s);
		void SetRelativeRotation(const Rot& m);
		void SetRelativeRotation(const Quat& q);

		void AddRelativeLocationOffset(const Vec3& v);
		void AddRelativeScaleOffset(const Vec3& v);
		void AddRelativeRotationOffset(const Quat& q);

		void AddWorldLocationOffset(const Vec3& v);
		void AddWorldRotationOffset(const Quat& q);

		void SetWorldLocation(const Vec3& v);
		void SetWorldRotation(const Quat& q);

		// Accessors
		const Mat4& GetWorld() const;

		Vec3 GetWorldLocation() const;
		Vec3 GetWorldScale() const;
		Quat GetWorldRotation() const;

		const Vec3& GetRelativeLocation() const;
		const Vec3& GetRelativeScale() const;
		const Quat& GetRelativeRotation() const;

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
		bool GetRenderShell() const;
		static void SetRenderShellGlobal(bool render);

		static float toSeconds(const AnimTime& abstractTime);

		Vec3 GetShellCenter() const;
		float GetShellRadius() const;

		void SetIsSelectable(bool inIsSelectable);
		bool IsSelectable() const;

		virtual GameObject* Clone();

		Name GetTypeName() const;

		bool IsParentOf(GameObject* pOther);

		void AttachComponent(Component* pInComponent);

	protected:
		bool alwaysRenderShell = false;
		bool renderShell;
		static bool globalRenderShell;

		bool isSelectable = true;

		Mat4 GetShellWorld() const;

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

		Name typeName;

		DLinkedList components;
	};
}

#endif
