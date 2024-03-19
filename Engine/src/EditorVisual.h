#ifndef _EditorVisual
#define _EditorVisual

#include "MathEngine.h"
#include "TextureObject.h"
#include "DLink.h"
#include "EditorInputObserver.h"

namespace Azul
{
	class GraphicsObject;
	class GameObject;
	class Selection;

	// Represents a visual 3D handle for different types of interactive transforms to game objects.
	class EditorVisual : public DLink, public Align16
	{
	public:
		enum class MutationAxis
		{
			X, Y, Z
		};

		// Big four
		EditorVisual();
		EditorVisual(const EditorVisual&) = delete;
		EditorVisual& operator=(const EditorVisual&) = delete;
		virtual ~EditorVisual();

		// Main loop methods when active
		void Update();
		void Draw();

		// Contract for different types of visuals.
		virtual void OnDragEnter() = 0;
		virtual void OnDragExit() = 0;
		virtual void OnDragUpdate() = 0;
		virtual float GetMouseOverDistance() const = 0;
		virtual bool IsMouseOver(float tolerance) const = 0;

		// Transform Accessors
		void SetLocation(const Vec3& v);
		void SetScale(const Vec3& v);
		void SetRotation(const Rot& m);
		const Vec3& GetLocation() const;
		const Mat4& GetWorld() const;

		// Bounding sphere accessors for physical object size
		Vec3 GetShellCenter() const;
		float GetShellRadius() const;
		GraphicsObject* GetGraphicsObject() const;

		// Setters
		void SetParentSelection(Selection* pInSelection);
		void SetTexture(TextureObject::Name inTexture);
		void SetMutationAxis(MutationAxis inAxis);
		void SetLocationFromObject(GameObject* pObject);

	protected:
		void Wash() override;
		bool Compare(DLink* pTargetNode) override;
		Mat4 GetShellWorld() const;

		Vec3 GetMutationAxis() const;
		Vec3 GetLocalAxis() const;
		Vec3 GetMutationUp() const;

		void SetGraphicsObject(GraphicsObject* pinGraphicsObject);

		void SetOriginVarsFromAxisOffset();
		Vec3 GetTotalDeltaDragOnMutationAxis() const;
		Vec3 GetClosestApproachFromMouseRay() const;
		float GetAxisDistanceToMouseRay() const;
		bool IsMouseOverAxisHandle(float tolerance) const;
		void RevertTexture();
		void AddTempTexture(TextureObject::Name textureName);

		MutationAxis mutationAxisType;

		Vec3 translation;
		Vec3 scale;
		Quat rotation;
		Vec3 dragStartPos;
		Vec3 originalDragOffset;
		Vec3 scaleMod;
		Vec3 translationMod;

		Mat4 worldCache;

		GraphicsObject* pShell;
		TextureObject* pOriginalTexture;

		GraphicsObject* pGraphicsObject;
		Selection* pParentSelection;
	};
}

#endif
