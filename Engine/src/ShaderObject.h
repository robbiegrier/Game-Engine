#ifndef _ShaderObject
#define _ShaderObject

#include <d3d11.h>
#include "DLink.h"
#include "MathEngine.h"

namespace Azul
{
	class GraphicsObject;
	class Camera;

	// A Shader Object encapsulates a particular type of Gpu shader such that
	// instanced Graphics objects can reference a shared copy of the shader.
	class ShaderObject : public DLink
	{
	public:
		enum class Name
		{
			Default,
			ColorByVertex,
			FlatTexture,
			LightTexture,
			ConstColor,
			EditorVisual,
			Sprite,
			SkinLightTexture,
			Null,
			None
		};

		// Big four
		ShaderObject();
		ShaderObject(const ShaderObject&) = delete;
		ShaderObject& operator = (const ShaderObject&) = delete;
		~ShaderObject();

		// Initialization
		void CreateVertexShader(const void* pCompiledVertexShaderObject, size_t codeSize);
		void CreatePixelShader(const unsigned char* pCompiledPixelShaderObject, size_t codeSize);
		void CreateInputLayout(const unsigned char* compiledVertexShaderObject, size_t codeSize, D3D11_INPUT_ELEMENT_DESC* pDesc, unsigned int SizeInBytes);

		// Open close paradigm for rendering objects using this shader
		virtual void Open(GraphicsObject* pObject);
		virtual void Close();

		virtual Camera* GetCamera() const;

		// As a DLink node
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;
		void SetName(ShaderObject::Name inName);
		ShaderObject::Name GetName() const;
		const char* NameToString();

	protected:
		// Overridable rendering events
		virtual void OnOpen(GraphicsObject* pObject) { static_cast<void>(pObject); }
		virtual void OnClose() {}

		// Initialization helpers
		const char* GetLatestProfile_PixelShader() const;
		const char* GetLatestProfile_VertexShader() const;

		ShaderObject::Name name;
		ID3D11InputLayout* poInputLayout = nullptr;
		ID3D11VertexShader* poVertexShader = nullptr;
		ID3D11PixelShader* poPixelShader = nullptr;

		struct CBObjectUniversal
		{
			Mat4 world;
		};
		ID3D11Buffer* pConstBuffObjectUniversal;

		ID3D11Buffer* pConstBuffProjection;
		ID3D11Buffer* pConstBuffView;
	};
}

#endif
