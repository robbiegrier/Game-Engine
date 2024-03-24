#ifndef _Camera
#define _Camera

#include "DLink.h"
#include "MathEngine.h"

struct ID3D11Buffer;

namespace Azul
{
	// Mathematical representation of a camera that can be injected into shaders
	// when rendering to render objects from this camera's perspective.
	class Camera final : public Align16, public DLink
	{
	public:
		enum class Type
		{
			Perspective3D,
			Orthographic2D
		};

		enum class Name
		{
			Default,
			High,
			Low,
			Aux,
			Player,
			Sprite,
			None
		};

		// Big four
		Camera();
		Camera(const Camera&) = delete;
		Camera& operator = (const Camera&) = delete;
		~Camera();

		// Initialize camera params
		void SetPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
		void SetOrthographic(const float xMin, const float xMax, const float yMin, const float yMax, const float zMin, const float zMax);
		void SetOrientAndPosition(const Vec3& Up_vect, const Vec3& inLookAt_pt, const Vec3& pos_pt);
		void SetViewport(const int inX, const int inY, const int width, const int height);
		void SetAspectRatio(float inAspectRatio);

		// Mutate camera state
		void AddLocationOffset(const Vec3& offset);
		void AddHorizontalLocationOffset(const Vec3& offset);
		void AddLookAtOffset(const Vec3& offset);

		// Update called by core loop
		void UpdateCamera();

		// Accessors
		Type GetType() const;
		Mat4& GetViewMatrix();
		Mat4& GetProjMatrix();
		void GetLocation(Vec3& outPos) const;
		Vec3 GetLocation() const;
		void GetDirection(Vec3& outDir) const;
		const Vec3& GetDirection() const;
		void GetUp(Vec3& outUp) const;
		void GetLookAt(Vec3& outLookAt) const;
		const Vec3& GetLookAt() const;
		void GetRight(Vec3& outRight) const;

		// Name
		void SetName(Name inName);
		Name GetName() const;
		const char* NameToString();

		// As a DLink node
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;

		Vec3 GetRay(float x, float y);

		float GetAspectRatio() const;
		float GetFovY() const;
		int GetScreenWidth() const;
		int GetScreenHeight() const;

	private:
		void UpdateProjectionMatrix();
		void UpdateViewMatrix();
		void privSetViewState(void);
		void privCalcPlaneHeightWidth(void);
		void privCalcFrustumVerts(void);
		void privCalcFrustumCollisionNormals(void);

	private:
		Mat4 projMatrix;
		Mat4 viewMatrix;

		Vec3 vUp;
		Vec3 vDir;
		Vec3 vRight;
		Vec3 vPos;
		Vec3 vLookAt;

		float nearDist;
		float farDist;
		float fovy;
		float aspectRatio;

		Name name;

		// world space coords for viewing frustum
		Vec3	nearTopLeft;
		Vec3	nearTopRight;
		Vec3	nearBottomLeft;

		Vec3	nearBottomRight;
		Vec3	farTopLeft;
		Vec3	farTopRight;
		Vec3	farBottomLeft;

		Vec3	farBottomRight;

		// Normals of the frustum
		Vec3	frontNorm;
		Vec3	backNorm;
		Vec3	rightNorm;

		Vec3	leftNorm;
		Vec3	topNorm;
		Vec3	bottomNorm;

		// w1(7)
		Camera::Type camType;

		// Screen size in world space
		float	near_height;
		float	near_width;
		float	far_height;
		float	far_width;

		float	right;
		float	left;
		float	n;
		float	f;
		float	top;
		float	bot;

		// viewports
		int		viewport_x;
		int		viewport_y;
		int		viewport_width;
		int		viewport_height;
	};
}

#endif
