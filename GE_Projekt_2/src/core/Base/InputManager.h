#ifndef _RAWINPUT
#define _RAWINPUT

#include "RenderInterface.h"

extern RenderInterface* renderInderface;

//register keyboard mouse as input devices!
bool RegisterInputDevices( HWND &hWnd )
{
	RAWINPUTDEVICE inputDevices[2];
        
	//adds mouse and allow legacy messages
	inputDevices[0].usUsagePage = 0x01; 
	inputDevices[0].usUsage = 0x02; 
	inputDevices[0].dwFlags = 0;   
	inputDevices[0].hwndTarget = 0;

	//adds keyboard and allow legacy messages
	inputDevices[1].usUsagePage = 0x01; 
	inputDevices[1].usUsage = 0x06; 
	inputDevices[1].dwFlags = 0;   
	inputDevices[1].hwndTarget = 0;

	if ( RegisterRawInputDevices(inputDevices, 2, sizeof(inputDevices[0]) ) == FALSE ) 
	{
		return false;
	}

	return true;
}

void inline HandleRawInput( HWND &hWnd, HRAWINPUT &lParam )
{
	//get raw input data buffer size
	UINT dbSize;
	GetRawInputData( lParam, RID_INPUT, NULL, &dbSize,sizeof(RAWINPUTHEADER) );
    

	//allocate memory for raw input data and get data
	BYTE* buffer = new BYTE[dbSize];    
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &dbSize, sizeof(RAWINPUTHEADER) );
	RAWINPUT* raw = (RAWINPUT*)buffer;
	
	// Handle Keyboard Input
	//---------------------------------------------------------------------------
	if (raw->header.dwType == RIM_TYPEKEYBOARD) 
	{
		switch( raw->data.keyboard.Message )
		{
			//key up
			case WM_KEYUP : 
				switch ( raw->data.keyboard.VKey )
				{
					case 'W' : renderInderface->getCamera()->setMovementToggle( 0, 0 );
					break;

					case 'S': renderInderface->getCamera()->setMovementToggle(1, 0);
					break;

					case 'A': renderInderface->getCamera()->setMovementToggle(2, 0);
					break;

					case 'D': renderInderface->getCamera()->setMovementToggle(3, 0);
					break;

					case 'M': renderInderface->getButtonInput()->SetMPressed(false);
					break;
					
					case 'N': renderInderface->getButtonInput()->SetNPressed(false);
					break;
					
					case 'B': renderInderface->getButtonInput()->SetBPressed(false);
					break;

					case 'V': renderInderface->getButtonInput()->SetVPressed(false);
					break;

					case 'C': renderInderface->getButtonInput()->SetCPressed(false);
					break;
				}
			break;

			//key down
			case WM_KEYDOWN : 
				switch ( raw->data.keyboard.VKey )
				{
					case VK_ESCAPE : PostQuitMessage(0);
					break;

					case 'W': renderInderface->getCamera()->setMovementToggle(0, 1);
					break;

					case 'S': renderInderface->getCamera()->setMovementToggle(1, -1);
					break;

					case 'A': renderInderface->getCamera()->setMovementToggle(2, -1);
					break;

					case 'D': renderInderface->getCamera()->setMovementToggle(3, 1);
					break;

					case 'M': renderInderface->getButtonInput()->SetMPressed(true);
					break;

					case 'N': renderInderface->getButtonInput()->SetNPressed(true);
					break;
					
					case 'B': renderInderface->getButtonInput()->SetBPressed(true);
					break;

					case 'V': renderInderface->getButtonInput()->SetVPressed(true);
					break;

					case 'C': renderInderface->getButtonInput()->SetCPressed(true);
					break;
				}
			break;
		}
	}
	
	// Handle Mouse Input
	//---------------------------------------------------------------------------
	else if (raw->header.dwType == RIM_TYPEMOUSE) 
	{
		float x = raw->data.mouse.lLastX;
		float y = raw->data.mouse.lLastY;
		float mul = 0.0025f;
		//mouse camera control
		renderInderface->getCamera()->adjustHeadingPitch(mul * (float)raw->data.mouse.lLastX, mul * (float)raw->data.mouse.lLastY);
		
	}

	//free allocated memory
	delete[] buffer;

}

#endif