#include "RL_API.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>


bool RL_API::getPlayerJumpState() {
    bool jumpState;
    ReadProcessMemory(m_gameProcessHandle, (PBYTE*)m_getPlayerJumpStateAddress, &jumpState, sizeof(DWORD), 0);
    return jumpState;
}

DWORD RL_API::initGetPlayerJumpState() { //returns dynamic address of the variable
    int numOfOffsets = 4;
    DWORD offsets[] = {0x1A3A158, 0, 0xCC, 0x48};
    DWORD tempAddr = m_gameModuleAddr;
    DWORD returnTemp = NULL;
    for(unsigned int i = 0; i < numOfOffsets - 1 ; i++) {
        //std::cout << std::hex << tempAddr << " + " << std::hex << offsets[i] << std::endl;
        ReadProcessMemory(m_gameProcessHandle, (PBYTE*)(tempAddr + offsets[i]), &returnTemp, sizeof(DWORD), 0);
        //std::cout << std::hex << returnTemp << std::endl;
        tempAddr = returnTemp;
    }
    return tempAddr + offsets[numOfOffsets-1];
}

RL_API::RL_API() {
    m_game = FindWindowA(NULL, "Rocket League (32-bit, DX9, Cooked)");
    if(m_game == NULL) {
        m_hookedToGame = false;
    }
    else {
        GetWindowThreadProcessId(m_game, &m_processId);
        m_gameModuleAddr = dwGetModuleBaseAddress(m_processId, _T("RocketLeague.exe"));
        m_gameProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, m_processId);
        m_hookedToGame = true;
        /* GETS ADRESSES FOR EVERY METHOD */
        m_getPlayerJumpStateAddress = initGetPlayerJumpState();
    }
}

DWORD_PTR RL_API::dwGetModuleBaseAddress(DWORD dwProcID, TCHAR *szModuleName) {
    DWORD_PTR dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcID);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 ModuleEntry32;
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &ModuleEntry32))
        {
            do
            {
                if (_tcsicmp(ModuleEntry32.szModule, szModuleName) == 0)
                {
                    dwModuleBaseAddress = (DWORD_PTR)ModuleEntry32.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &ModuleEntry32));
        }
        CloseHandle(hSnapshot);
    }
    return dwModuleBaseAddress;
}

RL_API::~RL_API() {
    //dtor
}
