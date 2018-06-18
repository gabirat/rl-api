#include "RL_API.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>


bool RL_API::getPlayerJumpState() {
    bool jumpState;
    ReadProcessMemory(gameProcessHandle, (PBYTE*)getPlayerJumpStateAddress, &jumpState, sizeof(DWORD), 0);
    return jumpState;
}

DWORD RL_API::initGetPlayerJumpState() { //returns dynamic address of the variable
    int numOfOffsets = 4;
    DWORD offsets[] = {0x1A3A158, 0, 0xCC, 0x48};//{0x1953F64, 0x80, 0x4, 0x138};//{0x194894C, 0x218, 0x64};
    DWORD tempAddr = gameModuleAddr;
    DWORD returnTemp = NULL;
    for(unsigned int i = 0; i < numOfOffsets - 1 ; i++) {
        //std::cout << std::hex << tempAddr << " + " << std::hex << offsets[i] << std::endl;
        ReadProcessMemory(gameProcessHandle, (PBYTE*)(tempAddr + offsets[i]), &returnTemp, sizeof(DWORD), 0);
        //std::cout << std::hex << returnTemp << std::endl;
        tempAddr = returnTemp;
    }
    return tempAddr + offsets[numOfOffsets-1];
}

RL_API::RL_API() {
    game = FindWindowA(NULL, "Rocket League (32-bit, DX9, Cooked)");
    if(game == NULL) {
        hookedToGame = false;
    }
    else {
        GetWindowThreadProcessId(game, &processId);
        gameModuleAddr = dwGetModuleBaseAddress(processId, _T("RocketLeague.exe"));
        gameProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
        hookedToGame = true;
        /* GETS ADRESSES FOR EVERY METHOD */
        getPlayerJumpStateAddress = initGetPlayerJumpState();
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
