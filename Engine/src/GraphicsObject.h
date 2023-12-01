#ifndef _GraphicsObject
#define _GraphicsObject

#include "MathEngine.h"
#include "ShaderObject.h"
#include "Render.h"

namespace Azul
{
	class Mesh;

	// Graphics object is a low level object that combines a mesh, shader, and other state like
	// textures and light. It stores the state for the corresponding shader.
	class GraphicsObject : public Render
	{
	public:
		// Big four
		GraphicsObject(Mesh* model, ShaderObject* pShaderObj);
		GraphicsObject() = delete;
		GraphicsObject(const GraphicsObject&) = delete;
		GraphicsObject& operator = (const GraphicsObject&) = delete;
		virtual ~GraphicsObject() override;

		// Core render function called by the draw stage
		void Render();

		// Mutate world state
		void SetWorld(const Mat4& _world);

		// Accessors
		Mesh* GetModel() const;
		ShaderObject* GetShader() const;
		const Mat4& GetWorld() const;

	protected:
		Mesh* pModel;
		Mat4* pWorld;
		ShaderObject* pShaderObject;
	};
}

#endif
