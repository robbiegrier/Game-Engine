//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#ifndef ENGINE_MATH_ROT_4x4_H
#define ENGINE_MATH_ROT_4x4_H

#include "Mat4.h"

namespace Azul
{
	class Rot final : public Mat4
	{
	public:

		// Do your magic here

		// Big 4
		Rot();

		Rot(const Rot&) = default;
		~Rot() = default;

		// Big 6
		Rot(Rot&&) = default;
		Rot& operator = (Rot&&) = default;

		Rot& operator = (const Rot& A) = default;
		Rot& operator = (const Quat&);

		explicit Rot(const Mat4& m);
		explicit Rot(const Quat& q);

		void set(const Quat& q);

		Mat4 operator * (const Mat4& A) const;
		Rot& operator *= (const Mat4& A) = delete;

		Rot operator * (const Quat& A) const;
		Rot& operator *= (const Quat& A);

		Mat4 operator * (const Scale& A) const;
		Rot& operator *= (const Scale& A) = delete;

		Rot operator * (const Rot& A) const;
		Rot& operator *= (const Rot& A);

		Mat4 operator * (const Trans& A) const;
		Rot& operator *= (const Trans& A) = delete;

		Rot(const Rot1 type, const float angle);
		Rot(const Rot3 mode, const float angle_x, const float angle_y, const float angle_z);
		Rot(const Axis mode, const Vec3& vAxis, const float angle_radians);
		Rot(const Orient type, const Vec3& dof, const Vec3& up);

		void set(const Rot1 type, const float angle);
		void set(const Rot3 mode, const float angle_x, const float angle_y, const float angle_z);
		void set(const Axis mode, const Vec3& vAxis, const float angle_radians);
		void set(const Orient, const Vec3& dof, const Vec3& up);

	private:

		friend Vec4;
		friend Mat4;
		friend Quat;
		friend Scale;
		friend Trans;
		friend Mat4;
	};
}

#endif

//--- End of File ---
