#ifndef _Iterator
#define _Iterator

#include "DLink.h"

namespace Azul
{
	class Iterator
	{
	public:
		virtual DLink* Next() = 0;
		virtual bool IsDone() = 0;
		virtual DLink* First() = 0;
		virtual DLink* Curr() = 0;
	};
}

#endif
