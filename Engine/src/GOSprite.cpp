#include "GOSprite.h"
#include "Engine.h"
#include "Mesh.h"
#include "ShaderObjectManager.h"
#include "MeshManager.h"

namespace Azul
{
	GOSprite::GOSprite(Mesh* pMesh, ShaderObject* pShader, Image* pInImage, Rect rect)
		: GraphicsObject(pMesh, pShader), pImage(pInImage)
	{
		name = Name::GraphicsObjectSprite;

		assert(pImage);

		this->origWidth = rect.width;
		this->origHeight = rect.height;
		this->origPosX = rect.x;
		this->origPosY = rect.y;

		// Create the UV matrix
		this->poMatrix_uv = new Mat4();

		float w = pImage->imageRect.width / (float)pImage->pText->GetWidth();
		float h = pImage->imageRect.height / (float)pImage->pText->GetHeight();
		float u = pImage->imageRect.x / (float)pImage->pText->GetWidth();
		float v = pImage->imageRect.y / (float)pImage->pText->GetHeight();
		Scale ScaleUV(w, h, 1.0f);
		Trans TransUV(u, v, 0.0f);
		*this->poMatrix_uv = ScaleUV * TransUV;
	}

	GOSprite::GOSprite(Image* pInImage, Rect rect)
		: GOSprite(MeshManager::Find(Mesh::Name::Sprite), ShaderObjectManager::Find(ShaderObject::Name::Sprite), pInImage, rect)
	{
	}

	GOSprite::~GOSprite()
	{
		delete poMatrix_uv;
	}

	void GOSprite::ActivateGraphics()
	{
		pImage->pText->ActivateTexture();
		pModel->TransferUVCorrection(this->poMatrix_uv);
	}

	GraphicsObject* GOSprite::Clone()
	{
		Rect cloneRect;
		cloneRect.width = (*poMatrix_uv)[m0];
		cloneRect.height = (*poMatrix_uv)[m5];
		cloneRect.x = (*poMatrix_uv)[m12];
		cloneRect.y = (*poMatrix_uv)[m13];

		return new GOSprite(pModel, pShaderObject, pImage, cloneRect);
	}

	void GOSprite::Draw()
	{
		pModel->RenderIndexBuffer();
	}

	void GOSprite::OverwriteImageProperties(TextureObject* pInTexture, Rect inRect)
	{
		pImage->SetTexture(pInTexture);
		pImage->SetRect(inRect);

		float w = pImage->imageRect.width / (float)pImage->pText->GetWidth();
		float h = pImage->imageRect.height / (float)pImage->pText->GetHeight();
		float u = pImage->imageRect.x / (float)pImage->pText->GetWidth();
		float v = pImage->imageRect.y / (float)pImage->pText->GetHeight();
		Scale ScaleUV(w, h, 1.0f);
		Trans TransUV(u, v, 0.0f);
		*this->poMatrix_uv = ScaleUV * TransUV;
	}
}