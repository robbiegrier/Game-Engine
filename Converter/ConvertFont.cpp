#include "ConvertFont.h"
#include <rapidxml.hpp>
#include "File.h"
#include "glyphData.h"
#include "fontData.h"
#include "ProtoAzul.h"
#include "ConverterUtils.h"

using namespace Azul;

void ConvertFont(const char* const pFileName)
{
	std::string pFileString = std::string(pFileName);
	const char* textureName = pFileString.replace(pFileString.end() - 4, pFileString.end(), ".png").c_str();

	Trace::out("Converting font:\n\txml: %s\n\tpng: %s\n\n", pFileName, textureName);

	std::string str = ConverterUtils::FileToString(pFileName);

	char* pBuff = new char[str.size() + 1];
	memset(pBuff, 0, str.size() + 1);
	memcpy(pBuff, str.c_str(), str.size());

	using namespace rapidxml;
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(pBuff);    // 0 means default parse flags

	fontData font;

	xml_node<>* node = doc.first_node("fontMetrics");
	while (node)
	{
		Trace::out("node %s\n", node->name());
		for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
		{
			Trace::out("attribute %s ", attr->name());
			Trace::out("=> %s\n", attr->value());
		}

		for (xml_node<>* child = node->first_node(); child; child = child->next_sibling())
		{
			glyphData* glyph = new glyphData();

			//Trace::out("\tchild %s\n", child->name());
			for (xml_attribute<>* attr = child->first_attribute(); attr; attr = attr->next_attribute())
			{
				//Trace::out("\tattribute %s ", attr->name());
				//Trace::out("=> %s\n", attr->value());

				if (strcmp(attr->name(), "key") == 0)
				{
					glyph->key = (unsigned int)std::atoi(attr->value());
				}
			}

			for (xml_node<>* child2 = child->first_node(); child2; child2 = child2->next_sibling())
			{
				//Trace::out("\t\tchild2 %s ", child2->name());
				//Trace::out("=> %s\n", child2->value());
				if (strcmp(child2->name(), "x") == 0)
				{
					glyph->x = (unsigned int)std::atoi(child2->value());
				}
				else if (strcmp(child2->name(), "y") == 0)
				{
					glyph->y = (unsigned int)std::atoi(child2->value());
				}
				else if (strcmp(child2->name(), "width") == 0)
				{
					glyph->width = (unsigned int)std::atoi(child2->value());
				}
				else if (strcmp(child2->name(), "height") == 0)
				{
					glyph->height = (unsigned int)std::atoi(child2->value());
				}

				for (xml_attribute<>* attr = child2->first_attribute(); attr; attr = attr->next_attribute())
				{
					//Trace::out("\t\tattribute %s ", attr->name());
					//Trace::out("=> %s\n", attr->value());
				}
			}

			font.glyphs[glyph->key] = glyph;
		}
		node = node->next_sibling();
	}

	delete[] pBuff;

	font.Print("Century Font");

	fontData_proto proto;
	font.Serialize(proto);
	const char* outFile = pFileString.replace(pFileString.end() - 4, pFileString.end(), ".proto.azul").c_str();
	ProtoAzul::WriteProtoFile(proto, outFile);
}