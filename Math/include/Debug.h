//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

#ifndef ENGINE_MATH_DEBUG_H
#define ENGINE_MATH_DEBUG_H

// This file assumes Framework.h is included in executable for Trace::Out

namespace Azul
{
	// Forward declarations:
	class Vec4; 
	class Vec3;
	class Mat4;
	class Mat3;
	class Quat;

	// ----------------------------------------------------------------------
	// Output Print for Vec4 and Mat4
	// ----------------------------------------------------------------------
	class Debug
	{
	public:
		static void Print(const char* pName, const Vec4 &r);
		static void Print(const char *pName, const Vec3 &r);
		static void Print(const char* pName, const Mat4 &r);
		static void Print(const char *pName, const Mat3 &r);
		static void Print(const char *pName, const Quat &r);
	};

}

#endif

//--- End of File ---
