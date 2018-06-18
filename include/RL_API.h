#ifndef RL_API_H
#define RL_API_H
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

class RL_API
{
    public:
        RL_API();
        virtual ~RL_API();
        bool getPlayerJumpState();

    private:
        DWORD getPlayerJumpStateAddress;
        HWND game;
        DWORD processId;
        DWORD gameModuleAddr;
        bool hookedToGame;
        HANDLE gameProcessHandle;
        DWORD_PTR dwGetModuleBaseAddress(DWORD dwProcID, TCHAR *szModuleName);
        DWORD initGetPlayerJumpState();
};

#endif // RL_API_H
