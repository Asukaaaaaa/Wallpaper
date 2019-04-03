#include <iostream>
#include <windows.h>

#include "taskBar.hpp"

HWND WP_HANDLE = 0;

void init()
{
    HWND progman = FindWindow("Progman", NULL);
    HWND defview = FindWindowEx(progman, NULL, "SHELLDLL_Defview", NULL);
    if (defview)
    {
        SendMessageTimeout(progman, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, NULL);
    }
    WP_HANDLE = GetNextWindow(progman, GW_HWNDPREV);
    //
}

char ElectronWdName[] = "Electron Player";
char TaskbarClassName[] = "Shell_TrayWnd";

/**
 * get StartHooks, StopHooks from dll
 *
BOOL WINAPI StartHooks(HWND hwnd);
BOOL WINAPI StopHooks();
*/

int main(int argc, char const *argv[])
{
    if (argc > 1)
    {
        if (!strcmp(argv[1], "-blur"))
        {
            setWdBlur(FindWindow(TaskbarClassName, NULL));
            return 0;
        }
    }
    {
        init();
        setWdBlur(FindWindow(TaskbarClassName, NULL));
        HWND handle = FindWindow(NULL, ElectronWdName);
        SetParent(handle, WP_HANDLE);
        // StartHooks(handle);
        // system("pause");
    }
    return 0;
}