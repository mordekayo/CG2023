#include "FDisplayWin32.h"

#include <windows.h>

FDisplayWin32::FDisplayWin32(const LPCWSTR& ApplicationName, int ScreenWidth, int ScreenHeight, WNDPROC WndProc)
{
    this->ApplicationName = ApplicationName;
    this->ScreenWidth = ScreenWidth;
    this->ScreenHeight = ScreenHeight;
    
    hInstance = GetModuleHandle(nullptr);

#pragma region Window init
    WNDCLASSEX wc;
    
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = ApplicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class.
    RegisterClassEx(&wc);
    
    RECT windowRect = { 0, 0, static_cast<LONG>(ScreenWidth), static_cast<LONG>(ScreenHeight) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

    auto posX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth) / 2;
    auto posY = (GetSystemMetrics(SM_CYSCREEN) - ScreenHeight) / 2;

    HWnd = CreateWindowEx(WS_EX_APPWINDOW,
        ApplicationName,
        ApplicationName,
        dwStyle,
        posX, posY,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(HWnd, SW_SHOW);
    SetForegroundWindow(HWnd);
    SetFocus(HWnd);

    ShowCursor(true);

#pragma endregion Window init
}

FDisplayWin32::~FDisplayWin32()
{
    UnregisterClass(ApplicationName, hInstance);
    DestroyWindow(HWnd);
}

HWND& FDisplayWin32::GetHWnd()
{
    return HWnd;
}

int FDisplayWin32::GetScreenWidth() const
{
    return ScreenWidth;
}

int FDisplayWin32::GetScreenHeight() const
{
    return ScreenHeight;
}
