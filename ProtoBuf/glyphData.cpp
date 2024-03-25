#include "glyphData.h"

glyphData::glyphData()
	: key(0u), x(0u), y(0u), width(0u), height(0u)
{
}

glyphData::~glyphData()
{
}

void glyphData::Serialize(glyphData_proto& out) const
{
	out.set_key(key);
	out.set_x(x);
	out.set_y(y);
	out.set_height(height);
	out.set_width(width);
}

void glyphData::Deserialize(const glyphData_proto& in)
{
	key = in.key();
	x = in.x();
	y = in.y();
	width = in.width();
	height = in.height();
}

void glyphData::Print(const char* const pName) const
{
	Trace::out("%s: ", pName);
	Trace::out("\tkey:%u ", key);
	Trace::out("\tx:%u ", x);
	Trace::out("\ty:%u ", y);
	Trace::out("\tw:%u ", width);
	Trace::out("\th:%u\n", height);
}