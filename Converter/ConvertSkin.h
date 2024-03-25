#ifndef CONVERT_SKIN_H
#define CONVERT_SKIN_H

#include "vboData.h"

namespace Azul
{
	void ConvertSkin(const char* const pName);

	//class Convert
	//{
	//public:
	//	static void SetVBO(
	//		vboData& vbo,                          // which VBO to fill
	//		bool enabled,                          // true
	//		vboData::VBO_TARGET     targetType,    //VBO_TARGET::ARRAY_BUFFER
	//		vboData::VBO_COMPONENT  componentType, //VBO_COMPONENT::FLOAT
	//		vboData::VBO_TYPE       vboType,       // VBO_TYPE::VEC3
	//		unsigned int   attribIndex,            // ATTRIB_LOCATION_VERT
	//		unsigned int   count,                  // Skin0.pos.size()
	//		unsigned int   dataSize,               // Skin0.pos.size() * sizeof(fVect3)
	//		unsigned char* poData                 // Skin0.pos
	//	);
	//};
}
#endif
