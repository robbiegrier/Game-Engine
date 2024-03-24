#ifndef _ImageManager
#define _ImageManager

#include "ManagerBase.h"
#include "Image.h"

namespace Azul
{
	class ImageManager : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static Image* Add(Image::Name name, Image* pImage);
		static Image* Find(Image::Name name);
		static void Dump();

	protected:
		// Big four
		ImageManager(int initialReserved, int reserveGrow);
		ImageManager(const ImageManager&) = delete;
		ImageManager& operator =(const ImageManager&) = delete;
		~ImageManager();

		// Manager
		virtual DLink* CreateNode() override;

		// Singleton
		static ImageManager& GetInstance();
		static ImageManager* pInstance;
	};
}

#endif