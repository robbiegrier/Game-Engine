//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "MathEngine.h"
#include "MathApp.h"

//-----------------------------------------------------------------------------
//  LERP - linear interpolation
//
//  This function returns a point on a line segment given in parametric form.
//  Where the inputs are point a (starting point), point b (ending point) of
//  the line segment and the parametric variable t.  If t is in the range of
//  ranging from t=0.0 to 1.0, this function returns a point on the line
//  segment. If t=0.0 this function returns point a, if t=1.0 this returns
//  point b.  Values of t<0.0 return points on the infinite line defined
//  before point a. Values of t>1.0 returns points on the infinite line
//  defined after point b.
//
//  inputs:
// 		a - point A
// 		b - point B
// 		t - parametric t
//
//  @return Vector (point) linearly interpolated based on parametric t
//
//  Example:
//
//      Vect   v1(1.0f, 2.0f, 3.0f);   // create v1=[1,2,3,1]
//      Vect   v2(-5.0f, 2.0f, 7.0f);  // create v2=[-5,2,7,1]
//      Vect   out;                    // create out vector
//
//      out =	lineParametric (v1, v2, 0.4f );  // t = 0.4f
//
//-----------------------------------------------------------------------------

namespace Azul
{
	void Vec3App::Lerp(Vec3& out, const Vec3& a, const Vec3& b, const float t)
	{
		out = a + ((b - a) * t);
	}

	void Vec3App::LerpArray(Vec3* out, const Vec3* a, const Vec3* b, const float t, const int numVects)
	{
		for (auto i = 0; i < numVects; i++)
		{
			out[i] = a[i] + ((b[i] - a[i]) * t);
		}
	}
}

// ---  End of File ---------------