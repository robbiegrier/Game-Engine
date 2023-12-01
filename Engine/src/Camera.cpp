#include "Camera.h"
#include "Engine.h"

namespace Azul
{
	Camera::Camera()
		: aspectRatio(0), farDist(0), fovy(0), nearDist(0)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::SetPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
	{
		aspectRatio = Aspect;
		fovy = MATH_PI_180 * Fovy;
		nearDist = NearDist;
		farDist = FarDist;
	};

	void Camera::SetOrientAndPosition(const Vec3& inUp, const Vec3& inLookAt, const Vec3& inPos)
	{
		// Remember the up, dir and right are unit length, and are perpendicular.
		// Treat lookAt as king, find Right vect, then correct Up to insure perpendiculare.
		// Make sure that all vectors are unit vectors.

		vLookAt = inLookAt;
		vDir = -(inLookAt - inPos); // Right-Hand camera: vDir is flipped
		vDir.norm();

		// Clean up the vectors (Right hand rule)
		vRight = inUp.cross(vDir);
		vRight.norm();
		vUp = vDir.cross(vRight);
		vUp.norm();
		vPos = inPos;
	}

	void Camera::SetAspectRatio(float inAspectRatio)
	{
		aspectRatio = inAspectRatio;
	}

	void Camera::AddLocationOffset(const Vec3& offset)
	{
		Vec3 tmpPos = vPos;

		Vec3 forward = vLookAt - vPos;
		Vec3 right = vRight;
		Vec3 up = Vec3(0.f, 1.f, 0.f);

		tmpPos += forward.getNorm() * offset[z];
		tmpPos += right.getNorm() * offset[x];
		tmpPos += up.getNorm() * offset[y];

		Vec3 newLookAt = tmpPos + (vLookAt - vPos).getNorm();

		SetOrientAndPosition(vUp, newLookAt, tmpPos);
	}

	void Camera::AddHorizontalLocationOffset(const Vec3& offset)
	{
		Vec3 tmpPos = vPos;

		Vec3 right = vRight;
		Vec3 up = Vec3(0.f, 1.f, 0.f);
		Vec3 forward = up.cross(right);

		tmpPos += forward.getNorm() * offset[z];
		tmpPos += right.getNorm() * offset[x];

		Vec3 newLookAt = tmpPos + (vLookAt - vPos).getNorm();

		SetOrientAndPosition(vUp, newLookAt, tmpPos);
	}

	void Camera::AddLookAtOffset(const Vec3& offset)
	{
		Vec3 toLookAt = (vLookAt - vPos).getNorm();

		toLookAt = Vec3(Vec4(toLookAt.getNorm(), 0.f) * Rot(Rot1::Y, -offset[x])).getNorm();
		float len = toLookAt.len();

		toLookAt[y] += offset[y];
		toLookAt.norm();
		toLookAt *= len;

		SetOrientAndPosition(Vec3(0, 1, 0), vPos + toLookAt, vPos);
	}

	void Camera::Wash()
	{
		name = Name::None;
		aspectRatio = 0;
		farDist = 0;
		fovy = 0;
		nearDist = 0;
	}

	bool Camera::Compare(DLink* pTargetNode)
	{
		Camera* pOther = (Camera*)pTargetNode;
		return name == pOther->GetName();
	}

	void Camera::Dump()
	{
		Trace::out("%s\n", NameToString());
	}

	// The projection matrix
	void Camera::UpdateProjectionMatrix()
	{
		float d = 1 / tanf(fovy / 2);

		projMatrix[m0] = d / aspectRatio;
		projMatrix[m1] = 0.0f;
		projMatrix[m2] = 0.0f;
		projMatrix[m3] = 0.0f;

		projMatrix[m4] = 0.0f;
		projMatrix[m5] = d;
		projMatrix[m6] = 0.0f;
		projMatrix[m7] = 0.0f;

		projMatrix[m8] = 0.0f;
		projMatrix[m9] = 0.0f;
		projMatrix[m10] = -(farDist + nearDist) / (farDist - nearDist);
		projMatrix[m11] = -1.0f;

		projMatrix[m12] = 0.0f;
		projMatrix[m13] = 0.0f;
		projMatrix[m14] = (-2.0f * farDist * nearDist) / (farDist - nearDist);
		projMatrix[m15] = 0.0f;

		// Converting from OpenGL-style NDC of [-1,1] to DX's [0,1].  See note: https://anteru.net/blog/2011/12/27/1830/
		// (Note: NDCConvert should be precomputed once and stored for reuse)
		Trans B(0.0f, 0.0f, 1.0f);
		Scale S(1.0f, 1.0f, 0.5f);

		projMatrix = projMatrix * B * S;
	};

	void Camera::UpdateViewMatrix()
	{
		// This functions assumes the your vUp, vRight, vDir are still unit
		// And perpendicular to each other

		// Set for DX Right-handed space
		viewMatrix[m0] = vRight[x];
		viewMatrix[m1] = vUp[x];
		viewMatrix[m2] = vDir[x];
		viewMatrix[m3] = 0.0f;

		viewMatrix[m4] = vRight[y];
		viewMatrix[m5] = vUp[y];
		viewMatrix[m6] = vDir[y];
		viewMatrix[m7] = 0.0f;

		viewMatrix[m8] = vRight[z];
		viewMatrix[m9] = vUp[z];
		viewMatrix[m10] = vDir[z];
		viewMatrix[m11] = 0.0f;

		// Apply R^t( -Pos ). Use dot-product with the basis vectors
		viewMatrix[m12] = -vPos.dot(vRight);
		viewMatrix[m13] = -vPos.dot(vUp);
		viewMatrix[m14] = -vPos.dot(vDir);
		viewMatrix[m15] = 1.0f;
	};

	// Update everything (make sure it's consistent)
	void Camera::UpdateCamera()
	{
		UpdateProjectionMatrix();
		UpdateViewMatrix();
	}

	Mat4& Camera::GetViewMatrix()
	{
		return viewMatrix;
	}

	Mat4& Camera::GetProjMatrix()
	{
		return projMatrix;
	}

	void Camera::GetLocation(Vec3& outPos) const
	{
		outPos = vPos;
	}

	Vec3 Camera::GetLocation() const
	{
		return vPos;
	}

	void  Camera::GetDirection(Vec3& outDir) const
	{
		outDir = vDir;
	}

	const Vec3& Camera::GetDirection() const
	{
		return vDir;
	}

	void  Camera::GetUp(Vec3& outUp) const
	{
		outUp = vUp;
	}

	void Camera::GetLookAt(Vec3& outLookAt) const
	{
		outLookAt = vLookAt;
	}

	const Vec3& Camera::GetLookAt() const
	{
		return vLookAt;
	}

	void Camera::GetRight(Vec3& outRight) const
	{
		outRight = vRight;
	}

	void Camera::SetName(Name inName)
	{
		name = inName;
	}

	Camera::Name Camera::GetName() const
	{
		return name;
	}

	const char* Camera::NameToString()
	{
		switch (name)
		{
		case Name::Default:
			return "Default";
		case Name::High:
			return "High";
		case Name::Low:
			return "Low";
		case Name::None:
			return "None";
		default:
			return "NO STRING FOR NAME";
		}
	}
}