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
		enum class Name
		{
			Default,
			High,
			Low,
			Aux,
			None
		};

		// Big four
		Camera();
		Camera(const Camera&) = delete;
		Camera& operator = (const Camera&) = delete;
		~Camera();

		// Initialize camera params
		void SetPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
		void SetOrientAndPosition(const Vec3& Up_vect, const Vec3& inLookAt_pt, const Vec3& pos_pt);

		// Mutate camera state
		void AddLocationOffset(const Vec3& offset);
		void AddHorizontalLocationOffset(const Vec3& offset);
		void AddLookAtOffset(const Vec3& offset);

		// Update called by core loop
		void UpdateCamera();

		// Accessors
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

	private:
		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

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
	};
}

#endif
