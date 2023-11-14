//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "DirectXDeviceMan.h"

DirectXDeviceMan *DirectXDeviceMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void DirectXDeviceMan::Create(ID3D11Device *pDevice, ID3D11DeviceContext *pContext)
{
	// make sure values are ressonable 
	assert(pDevice);
	assert(pContext);

	// intialize the singleton here
	assert(posInstance == nullptr);

	// Do the initialization
	if(posInstance == nullptr)
	{
		posInstance = new DirectXDeviceMan(pDevice, pContext);
	}
}

ID3D11Device *DirectXDeviceMan::GetDevice()
{
	DirectXDeviceMan *pMan = DirectXDeviceMan::privGetInstance();
	assert(pMan);

	return pMan->pDevice;
}

ID3D11DeviceContext *DirectXDeviceMan::GetContext()
{
	DirectXDeviceMan *pMan = DirectXDeviceMan::privGetInstance();
	assert(pMan);

	return pMan->pContext;
}

void DirectXDeviceMan::Destroy()
{
	delete posInstance;
}

DirectXDeviceMan::DirectXDeviceMan(ID3D11Device *_pDevice, ID3D11DeviceContext *_pContext)
{
	this->pDevice = _pDevice;
	this->pContext = _pContext;
}

DirectXDeviceMan::~DirectXDeviceMan()
{
	
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
DirectXDeviceMan *DirectXDeviceMan::privGetInstance()
{
	// Safety - this forces users to call Create() first before using class
	assert(posInstance != nullptr);

	return posInstance;
}

// --- End of File ---
