#pragma once
#include "Game.h"

class DisplayWin32
{
protected:

	LPCWSTR applicationName;
	HINSTANCE hInstance;    
	WNDCLASSEX wc;        
	RECT windowRect;         
	int screenWidth;
	int screenHeight;
	HWND hWnd;			    

public:

	DisplayWin32(LPCWSTR& applicationName, int screenWidth, int screenHeight, WNDPROC wndProc);
	~DisplayWin32();

	int GetScreenWidth();
	int GetScreenHeight();
	HWND& GetHWnd();

};