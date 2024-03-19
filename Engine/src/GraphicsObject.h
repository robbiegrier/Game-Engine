#ifndef _GraphicsObject
#define _GraphicsObject

#include "MathEngine.h"
#include "ShaderObject.h"
#include "Render.h"

namespace Azul
{
	class Mesh;
	class TextureObject;

	// Graphics object is a low level object that combines a mesh, shader, and other state like
	// textures and light. It stores the state for the corresponding shader.
	class GraphicsObject : public Render
	{
	public:
		enum class Name
		{
			GraphicsObjectColorByVertex,
			GraphicsObjectConstColor,
			GraphicsObjectFlatTexture,
			GraphicsObjectLightTexture,
			GraphicsObjectNull,
			None
		};

		// Big four
		GraphicsObject(Mesh* model, ShaderObject* pShaderObj);
		GraphicsObject() = delete;
		GraphicsObject(const GraphicsObject&) = delete;
		GraphicsObject& operator = (const GraphicsObject&) = delete;
		virtual ~GraphicsObject();

		// Core render function called by the draw stage
		void Render();

		// Mutate world state
		void SetWorld(const Mat4& _world);

		// Accessors
		Mesh* GetModel() const;
		ShaderObject* GetShader() const;
		const Mat4& GetWorld() const;

		virtual GraphicsObject* Clone() = 0;

		virtual TextureObject* GetTexture();
		virtual Vec4 GetColor();

		Name GetName() const;

	protected:
		Mesh* pModel;
		Mat4* pWorld;
		ShaderObject* pShaderObject;

		Name name;
	};
}

#endif
