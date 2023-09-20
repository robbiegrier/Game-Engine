//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"
// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

using namespace Azul;

void SpeedA_test();
void SpeedB_test();
void SpeedC_test();
void SpeedD_test();
void SpeedE_test();
void SpeedF_test();
void SpeedG_test();
void SpeedH_test();

int main()
{
	{
	//Vec3 A(1, 2, 3);

	//Mat4 R(Mat4::Rot3::XYZ, 0.312f, -0.11f, 0.97f);
	//Mat4 T(Mat4::Trans::XYZ, 3, 4, 5);
	//Mat4 S(Mat4::Scale::XYZ, 7, 8, 9);

	//Mat4 M = S * R * T;

	//A.Print("A");
	//M.Print("M");

	//// Convert Vec3 -> Vec4

	//Vec4  D0(A, 0);  // w=0
	//Vec4  D1(A, 1);  // w=1

	//D0.Print("D0");
	//D1.Print("D1");

	//Vec4 E0 = D0 * M;
	//Vec4 E1 = D1 * M;

	//E0.Print("E0");
	//E1.Print("E1");

	//// In one line
	//Vec4 C0 = Vec4(A, 0) * M;
	//Vec4 C1 = Vec4(A, 1) * M;

	//C0.Print("C0");
	//C1.Print("C1");

	//Vec3 B0(C0);
	//Vec3 B1(C1);

	//B0.Print("B0");
	//B1.Print("B1");

	//Vec4 F0 = B0 * M;
	//F0.Print("F0");
	//Vec4 F1 = B1 * M;
	//F1.Print("F1");

}


	{
		//Vec3 A(1, 2, 3);
		//Vec3 B(-5, 3, 4);

		//A.Print("A");
		//B.Print("B");

		//Vec3 C = A.cross(B);

		//C.Print("C = A x B ");

		//Vec3 D = C.getNorm();

		//D.Print("unit C ");

		//Vec3 E = A.cross(B).getNorm();
	
		//E.Print("E = (A x B).getNorm() ");

		//float  LenC = C.len();

		//Trace::out("Len(C): \n");
		//Trace::out("        %f \n\n", LenC);

		//float  dotAB = A.dot(B);

		//Trace::out("A dot B: \n");
		//Trace::out("         %f \n\n", dotAB);
	}

	Test::RunTests();
}

// --- End of File ---------------
