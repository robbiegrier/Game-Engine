//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef VERIFY_H
#define VERIFY_H

class Verify
{
public:
	struct Model_stats
	{
		unsigned int numVerts;
		unsigned int numTris;
		unsigned int TextureWidth;
		unsigned int TextureHeight;
	};

public:
	// Create a new buffer with a copy of the data
	// Update the pointer and numBytes
	// Data will be released in the Unit Test
	static void GetVertsCopy(unsigned char*& p, unsigned int& numBytes);
	static void GetUVsCopy(unsigned char*& p, unsigned int& numBytes);
	static void GetNormsCopy(unsigned char*& p, unsigned int& numBytes);
	static void GetIndexCopy(unsigned char*& p, unsigned int& numBytes);
	static void GetTextureBufferCopy(unsigned char*& p, unsigned int& numBytes);

	// just return the structure
	static Model_stats GetModelStats();
};

#endif

// --- End of File ---
