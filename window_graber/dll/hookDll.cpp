#include <windows.h>
#include <stdio.h>

//
// 共享代码段，所有线程共享
//
#pragma data_seg("SHARED")
static HHOOK hhkMouse = NULL;  // 鼠标钩子句柄
static HHOOK hhkKey = NULL;    // 键盘钩子
static HINSTANCE hInst = NULL; // 本DLL的实例句柄
static HWND handle = NULL;     // 调用DLL的主窗口句柄,这样就可以SendMessage给主窗口鼠标消息及其参数
#pragma data_seg()
#pragma comment(linker, "/section:SHARED, rws")

BOOL WINAPI DllMain(HINSTANCE hInstDll,
                    DWORD lpwReason,
                    LPVOID lpvReserved)
{
    hInst = hInstDll;
    return TRUE;
}

//
// 定义低级钩子函数
//
struct MsgInfo
{
    int nCode;
    WPARAM wParam;
    LPARAM lParam;
};

DWORD WINAPI MouseProcThread(LPVOID lpParam)
{
    MsgInfo *info = (MsgInfo *)lpParam;
    // 有消息时，将其发给主程序
    if (handle && info->nCode == HC_ACTION)
    {
        switch (info->wParam)
        {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        case WM_MOUSEWHEEL:
            printf("mouse_event\n");
            PostMessage(handle, info->wParam, 0, info->lParam);
            break;
        }
    }
    return 0;
}

LRESULT CALLBACK LowLevelMouseProc(int nCode,     // hook code
                                   WPARAM wParam, // message identifier
                                   LPARAM lParam  // mouse coordinates
)
{
    MsgInfo info = {nCode, wParam, lParam};
    HANDLE hThread = CreateThread(NULL, 0, MouseProcThread, &info, 0, NULL);
    CloseHandle(hThread);
    return CallNextHookEx(hhkMouse, nCode, wParam, lParam);
}

DWORD WINAPI KeyProcThread(LPVOID lpParam)
{
    MsgInfo *info = (MsgInfo *)lpParam;
    if (handle && info->nCode == HC_ACTION)
    {
        switch (info->wParam)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            printf("key_event\n");
            KBDLLHOOKSTRUCT *pKey = (KBDLLHOOKSTRUCT *)(info->lParam);
            PostMessage(handle, info->wParam, pKey->vkCode, pKey->scanCode);
            break;
        }
    }
    return 0;
}

LRESULT CALLBACK LowLevelKeyProc(int nCode,     // hook code
                                 WPARAM wParam, // message identifier
                                 LPARAM lParam  // mouse coordinates
)
{
    MsgInfo info = {nCode, wParam, lParam};
    HANDLE hThread = CreateThread(NULL, 0, KeyProcThread, &info, 0, NULL);
    CloseHandle(hThread);
    return CallNextHookEx(hhkKey, nCode, wParam, lParam);
}

//
// 安装低级钩子函数，从而截获系统所有的鼠标、键盘消息
//
BOOL WINAPI StartHooks(HWND hwnd)
{
    handle = hwnd;
    hhkMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0);
    hhkKey = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyProc, hInst, 0);
    return hhkMouse && hhkKey ? TRUE : FALSE;
}

//
// 卸载低级鼠标钩子
//
VOID WINAPI StopHooks()
{
    if (hhkMouse)
    {
        UnhookWindowsHookEx(hhkMouse);
    }
    if (hhkKey)
    {
        UnhookWindowsHookEx(hhkKey);
    }
}