//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#ifndef ENGINE_MATH_QUAT_H
#define ENGINE_MATH_QUAT_H

#include "Mat4.h"
#include "Debug.h"

//------------------------------------------------------------------------
//
//  This class contains functions and data structure for a 4-dimensional
//  Quaternions.  The quaternions are internally stored interms of the
//  imaginary components (x,y,z) and the real components (real).
//
//  In the Math library, quaternions can be thought of as "rotational"
//  matrices, everything that you can represent with rotational matrices
//  you can use with quaternions.  Such as quaternion concatenation,
//  inverses, transpose, normalize, multiplying with matrices, and with
//  vectors.
//
//------------------------------------------------------------------------
namespace Azul
{
	class Quat final : public Align16
	{
	public:
		Quat(void);
		Quat(const Quat& qIn);
		Quat(const Vec3& vect, const float real);
		Quat(const float vx, const float vy, const float vz, const float real);

		explicit Quat(const Rot& mIn);
		explicit Quat(const Special  value);

		Quat(const Rot1 type, const float angle);
		Quat(const Rot3 type, const float angle_x, const float angle_y, const float angle_z);
		Quat(const Axis type, const Vec3& vAxis, const float angle_radians);
		Quat(const Orient, const Vec3& dof, const Vec3& up);
		~Quat(void);

		// Set
		void set(const Special type);
		void set(const Rot1 type, const float angle);
		void set(const Rot3 type, const float angle_x, const float angle_y, const float angle_z);

		void set(const Axis type, const Vec3& vAxis, const float angle_radians);
		void set(const Orient, const Vec3& dof, const Vec3& up);
		void set(const float vx, const float vy, const float vz, const float real);

		float& operator[](const x_enum);
		float& operator[](const y_enum);
		float& operator[](const z_enum);
		float& operator[](const w_enum);

		void qx(float val);
		void qy(float val);
		void qz(float val);
		void real(float val);

		void setVec3(const Vec3& vect);
		void set(const Vec3& vect, const float real);
		void set(const Mat4& m);

		// Get

		float operator[](const x_enum) const;
		float operator[](const y_enum) const;
		float operator[](const z_enum) const;
		float operator[](const w_enum) const;

		float qx()const;
		float qy()const;
		float qz()const;
		float real()const;

		float getAngle(void) const;
		void getVec3(Vec3& vOut) const;
		void getAxis(Vec3& vOut) const;

		// General member functions

		// dot product of two quaternions (4D)
		float dot(const Quat& qin) const;

		// Magnitude

		float mag(void) const;

		// magnitude Squared (Remember for Unit Quats, magSquared == 1.0f );
		float magSquared(void) const;
		float invMag(void) const;

		// Conjugate
		// Conjugate Quat in place
		Quat& conj(void);

		// Return conjuate Quat
		Quat getConj(void) const;

		// Inverse
		// Inverse Quat in place
		Quat& inv(void);

		// Return inverse Quat
		Quat getInv(void) const;

		// Normalize

			// Normalize Quat in place
		Quat& norm(void);

		// Return Normalize Quat
		Quat getNorm(void) const;

		// Linear Operator

			// Linear Operator vout = Quat * Vector * Quat(conjugate)
			// vOut = Quat * vIn * Quat.conj()
		void Lqvqc(const Vec3& vIn, Vec3& vOut) const;

		// Linear Operator  vOut = Quat(conjugate) * Vector * Quat
		// vOut = Quat.conj() * vIn * Quat
		void Lqcvq(const Vec3& vIn, Vec3& vOut) const;

		// Test functions

		bool isEqual(const Quat& qin, const float epsilon = MATH_TOLERANCE) const;
		bool isEquivalent(const Quat& qin, const float epsilon = MATH_TOLERANCE) const;
		bool isNegEqual(const Quat& qin, const float epsilon = MATH_TOLERANCE) const;
		bool isConjugateEqual(const Quat& qin, const float epsilon = MATH_TOLERANCE) const;

		bool isIdentity(const float epsilon = MATH_TOLERANCE) const;
		bool isNormalized(const float epsilon = MATH_TOLERANCE) const;
		bool isZero(const float epsilon = MATH_TOLERANCE) const;

		// Overload the = operator

		Quat& operator=(const Quat& q);
		Quat& operator=(const Rot& m);

		// Overload the + operator

		Quat operator+(void) const;
		Quat operator+(const Quat& q) const;
		Quat& operator+=(const Quat& q);
		Quat operator+(const float a) const;
		Quat& operator+=(const float a);
		friend Quat operator+(const float a, const Quat& q);

		// Overload the - operator

		Quat operator-(void) const;
		Quat operator-(const Quat& q) const;
		Quat& operator-=(const Quat& q);
		Quat operator-(const float a) const;
		Quat& operator-=(const float a);
		friend Quat operator-(const float a, const Quat& q);

		// Overload the * operator

		Quat operator*(const float a) const;
		Quat& operator*=(const float a);
		friend Quat operator*(const float a, const Quat& q);

		Quat& operator*=(const Mat4& m) = delete;
		Mat4 operator*(const Mat4& m) const;

		Quat operator*(const Quat& q) const;
		Quat& operator*=(const Quat& q);

		Mat4& operator*=(const Scale& m) = delete;
		Mat4 operator*(const Scale& m) const;

		Quat& operator*=(const Rot& m);
		Rot operator*(const Rot& m) const;

		Mat4& operator*=(const Trans& m) = delete;
		Mat4 operator*(const Trans& m) const;

		// Overload the / operator

		Quat operator/(const Quat& q) const;
		Quat& operator/=(const Quat& q);
		Quat operator/(const float a) const;
		Quat& operator/=(const float a);
		friend Quat operator/(const float a, const Quat& q);

		// For printing
		void Print(const char* pName) const;

	private:
		friend Vec4;
		friend Mat4;
		friend Rot;
		friend Scale;
		friend Trans;
		friend Mat4;

		// anonymous union
		union
		{
			__m128	_mq;

			// anonymous struct
			struct
			{
				float _qx;
				float _qy;
				float _qz;
				float _qw;
			};

			struct
			{
				Vec3  _qV3;
			};

			struct
			{
				Vec4 _qV4;
			};
		};
	};
}

#endif

//--- End of File ---
