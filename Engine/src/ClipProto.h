#ifndef _ClipProto
#define _ClipProto

#include "Clip.h"
#include "animClipData.h"

namespace Azul
{
	class ClipProto : public Clip
	{
	public:
		ClipProto(const char* pAnimFilename);
		ClipProto() = delete;
		ClipProto(const ClipProto&) = delete;
		ClipProto& operator = (const ClipProto&) = delete;
		virtual ~ClipProto() = default;

	private:
		void SetFromClipData(const animClipData& mB);
	};
}

#endif
