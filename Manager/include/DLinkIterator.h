#ifndef	_DLinkIterator
#define	_DLinkIterator

#include "Iterator.h"

namespace Azul
{
	class DLinkIterator : public Iterator
	{
	public:
		DLinkIterator();

		virtual DLink* Next() override;
		virtual bool IsDone() override;
		virtual DLink* First() override;
		virtual DLink* Curr() override;

		void Reset(DLink* pHead);

	private:
		void privClear();

	public:
		DLink* pHead;
		DLink* pCurr;
		bool bDone;
	};
}

#endif
