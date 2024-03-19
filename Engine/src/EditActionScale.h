#ifndef _EditActionScale
#define _EditActionScale

#include "EditAction.h"
#include "MathEngine.h"

namespace Azul
{
	// Action to scale an object by some 3D vector
	class EditActionScale : public EditAction, public Align16
	{
	public:
		// Big four
		EditActionScale() = default;
		EditActionScale(const EditActionScale&) = delete;
		EditActionScale& operator=(const EditActionScale&) = delete;
		virtual ~EditActionScale() = default;

		// Action contract
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Dump() override;

		// Access Scale
		void SetScale(const Vec3& inScale);
		const Vec3& GetScale() const;

	private:
		Vec3 scale;
	};
}

#endif