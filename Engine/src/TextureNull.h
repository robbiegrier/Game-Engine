//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_NULL_H
#define TEXTURE_NULL_H

#include "TextureObject.h"

class TextureNull : public TextureObject
{
public:
	TextureNull();
	TextureNull(const TextureNull &) = delete;
	TextureNull &operator=(const TextureNull &other) = delete;
	virtual ~TextureNull();


private:

};

#endif

// --- End of File ---
