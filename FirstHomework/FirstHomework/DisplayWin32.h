#pragma once
#include <WinString.h>

class DisplayWin32
{
public:
    
    DisplayWin32(const LPCWSTR& ApplicationName, int ScreenWidth, int ScreenHeight, WNDPROC WndProc);
    ~DisplayWin32();

    HWND& GetHWnd();

    int GetScreenWidth() const;
    int GetScreenHeight() const;
    
private:

    LPCWSTR ApplicationName;
    HWND HWnd;
    HINSTANCE hInstance;

    int ScreenWidth;
    int ScreenHeight;
};
