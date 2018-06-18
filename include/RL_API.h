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
        DWORD     m_getPlayerJumpStateAddress;
        HWND      m_game;
        DWORD     m_processId;
        DWORD     m_gameModuleAddr;
        bool      m_hookedToGame;
        HANDLE    m_gameProcessHandle;
        DWORD_PTR dwGetModuleBaseAddress(DWORD dwProcID, TCHAR *szModuleName);
        DWORD     initGetPlayerJumpState();
};

#endif // RL_API_H
