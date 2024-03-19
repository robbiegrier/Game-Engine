#ifndef _EditActionRotate
#define _EditActionRotate

#include "EditAction.h"
#include "MathEngine.h"

namespace Azul
{
	// Action to rotate an object with a quaternion
	class EditActionRotate : public EditAction, public Align16
	{
	public:
		// Big four
		EditActionRotate() = default;
		EditActionRotate(const EditActionRotate&) = delete;
		EditActionRotate& operator=(const EditActionRotate&) = delete;
		virtual ~EditActionRotate() = default;

		// Action contract
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Dump() override;

		// Access Rotation
		void SetRotation(const Quat& inRot);
		const Quat& GetRotation() const;

	private:
		Quat rotation;
	};
}

#endif