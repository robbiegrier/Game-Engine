//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------
#ifndef DIRECT_X_DEVICE_MAN_H
#define DIRECT_X_DEVICE_MAN_H

#include <d3d11.h>

class DirectXDeviceMan
{
public:	
	
	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------

	static void Create(ID3D11Device *pDevice, ID3D11DeviceContext *pContext);
	static void Destroy();

	static ID3D11Device *GetDevice();
	static ID3D11DeviceContext *GetContext();

	~DirectXDeviceMan();

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
private:
	static DirectXDeviceMan *privGetInstance();

	DirectXDeviceMan() = delete;
	DirectXDeviceMan(const DirectXDeviceMan &) = delete;
	DirectXDeviceMan &operator = (const DirectXDeviceMan &) = delete;

	DirectXDeviceMan(ID3D11Device *_pDevice, ID3D11DeviceContext *_pContext);

	
	//----------------------------------------------------------------------
	// Data: unique data for this manager 
	//----------------------------------------------------------------------
private:
	static DirectXDeviceMan *posInstance;
	ID3D11Device *pDevice;
	ID3D11DeviceContext *pContext;

};

#endif

// --- End of File ---
