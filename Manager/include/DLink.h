#ifndef _DLink
#define _DLink

namespace Azul
{
	class DLink
	{
	public:
		DLink();
		virtual ~DLink() = default;
		virtual void Wash() = 0;
		virtual char* GetName()
		{
			return nullptr;
		}

		virtual bool Compare(DLink* pTargetNode) = 0;
		void Clear();
		virtual void Dump();

		DLink* pNext;
		DLink* pPrev;
	};
}

#endif
