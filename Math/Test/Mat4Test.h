//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef MAT4_TEST_H
#define MAT4_TEST_H

#include "MathEngine.h"
#include "_UnitTestConfiguration.h"

namespace Azul
{
#if SpeedTestA
	class Mat4Test final : public Align16
	{
	public:

		// Big 4
		Mat4Test();
		Mat4Test &operator = (const Mat4Test &A);
		Mat4Test(const Mat4Test &tM);
		~Mat4Test();

		const Mat4Test getInv2(void) const;
		const float det() const;
		const Mat4Test privGetAdj(void) const;
		void operator *= (const float s);

	private:

		friend Mat3;
		friend Vec3;
		friend Vec4;

		union
		{
			/*     | m0  m1  m2   m3  | */
			/* m = | m4  m5  m6   m7  | */
			/*     | m8  m9  m10  m11 | */
			/*     | m12 m13 m14  m15 | */

			struct
			{
				Vec4 _rows[4];
			};


			struct
			{
				Vec4 _v0;
				Vec4 _v1;
				Vec4 _v2;
				Vec4 _v3;
			};

			struct
			{
				float _m0;
				float _m1;
				float _m2;
				float _m3;
				float _m4;
				float _m5;
				float _m6;
				float _m7;
				float _m8;
				float _m9;
				float _m10;
				float _m11;
				float _m12;
				float _m13;
				float _m14;
				float _m15;
			};
		};
	};

#endif
}

#endif

//--- End of File ---
