#include "ImageManager.h"
#include "DLinkedList.h"
#include "ListNode.h"

namespace Azul
{
	ImageManager* ImageManager::pInstance = nullptr;

	ImageManager::ImageManager(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
	}

	ImageManager::~ImageManager()
	{
	}

	void ImageManager::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new ImageManager(initialReserved, reserveGrow);
	}

	void ImageManager::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	Image* ImageManager::Add(Image::Name name, Image* pImage)
	{
		ImageManager& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pImage);
		pImage->SetName(name);
		//Trace::out("\tAdded Image: %s\n", pImage->NameToString());
		return pImage;
	}

	Image* ImageManager::Find(Image::Name name)
	{
		ImageManager& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			Image* pAsset = (Image*)pNode->Get();

			if (pAsset->GetName() == name)
			{
				return pAsset;
			}
		}

		return nullptr;
	}

	DLink* ImageManager::CreateNode()
	{
		return new OwningListNode();
	}

	ImageManager& ImageManager::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void ImageManager::Dump()
	{
		ImageManager& self = GetInstance();
		Trace::out("%s\n", STRING_ME(ShaderObjectManager));
		self.ManagerBase::Dump();
	}
}