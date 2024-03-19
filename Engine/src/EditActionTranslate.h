#ifndef _EditActionTranslate
#define _EditActionTranslate

#include "EditAction.h"
#include "MathEngine.h"

namespace Azul
{
	// Action to translate an object by some 3D vector
	class EditActionTranslate : public EditAction, public Align16
	{
	public:
		// Big four
		EditActionTranslate() = default;
		EditActionTranslate(const EditActionTranslate&) = delete;
		EditActionTranslate& operator=(const EditActionTranslate&) = delete;
		virtual ~EditActionTranslate() = default;

		// Action contract
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Dump() override;

		// Access Translation
		void SetTranslation(const Vec3& inTranslation);
		const Vec3& GetTranslation() const;

	private:
		Vec3 translation;
	};
}

#endif