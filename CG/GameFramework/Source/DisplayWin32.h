#pragma once
#include <WinString.h>

class FDisplayWin32
{
public:
    
    FDisplayWin32(const LPCWSTR& ApplicationName, int ScreenWidth, int ScreenHeight, WNDPROC WndProc);
    ~FDisplayWin32();

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
