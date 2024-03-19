#ifndef _InspectorWindow
#define _InspectorWindow

#include "MathEngine.h"
#include "GameObject.h"

namespace Azul
{
	// Shows the details of the selected object and allows it to be modified in some ways.
	class InspectorWindow
	{
	public:
		// Big four
		InspectorWindow() = default;
		InspectorWindow(const InspectorWindow&) = delete;
		InspectorWindow& operator=(const InspectorWindow&) = delete;
		~InspectorWindow() = default;

		void Update();

	private:
		void UpdateName();
		void UpdateTranslation();
		void UpdateRotation();
		void UpdateScale();

		bool wasTranslateActive = false;
		bool wasRotateActive = false;
		bool wasScaleActive = false;
		bool wasNameActive = false;

		Vec3 translationStart;
		Vec3 prevTranslation;
		Vec3 cacheTranslation;

		Quat rotationStart;
		Vec3 cacheRot;
		Vec3 prevRotE;
		Quat prevRot;
		Quat totalRot;

		Vec3 scaleStart;
		Vec3 prevScale;
		Vec3 cacheScale;

		char cacheName[GameObject::NAME_SIZE];
	};
}

#endif