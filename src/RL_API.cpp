#include "RL_API.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>
#include <vector>


bool RL_API::getPlayerJumpState() { //returns 0 if wheels touch the ground or 1 if they dont
    bool jumpState;
    ReadProcessMemory(m_gameProcessHandle, (PBYTE*)(m_currentDynamicAddresses.getJumpState), &jumpState, sizeof(DWORD), 0);
    return jumpState;
}

float RL_API::getPlayerCameraXPosition() {
    float position;
    ReadProcessMemory(m_gameProcessHandle, (PBYTE*)(m_currentDynamicAddresses.getPlayerCameraXPosition), &position, sizeof(DWORD), 0);
    return position;
}

float RL_API::getPlayerCameraYPosition() {
    float position;
    ReadProcessMemory(m_gameProcessHandle, (PBYTE*)(m_currentDynamicAddresses.getPlayerCameraYPosition), &position, sizeof(DWORD), 0);
    return position;
}

float RL_API::getPlayerCameraZPosition() {
    float position;
    ReadProcessMemory(m_gameProcessHandle, (PBYTE*)(m_currentDynamicAddresses.getPlayerCameraZPosition), &position, sizeof(DWORD), 0);
    return position;
}


DWORD RL_API::functionInitializer(std::vector<DWORD> offsets) { //returns dynamic address of the variable
    int numOfOffsets = offsets.size();
    DWORD tempAddr = m_gameModuleAddr;
    DWORD returnTemp = NULL;
    for(unsigned int i = 0; i < numOfOffsets - 1 ; i++) {
        ReadProcessMemory(m_gameProcessHandle, (PBYTE*)(tempAddr + offsets[i]), &returnTemp, sizeof(DWORD), 0);
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

        /*TODO: Read offsets form a file*/
        m_offsets.getJumpState       = {0x1A3A158, 0, 0xCC, 0x48};
        m_offsets.getPlayerCameraXPosition = {0x1A3A8E8};//, 0x1FC, 0x8, 0x2C}; //index 0 can also be equal to 0x196DB44 nevermind everything broke after a goal :)
        m_offsets.getPlayerCameraZPosition = {0x1A3A8F0};// 0x1FC, 0x8, 0x30};
        m_offsets.getPlayerCameraYPosition = {0x1A3A8EC};// 0x1FC, 0x8, 0x34};

        /* GETS ADRESSES FOR EVERY METHOD */
        m_currentDynamicAddresses.getJumpState = functionInitializer(m_offsets.getJumpState);
        m_currentDynamicAddresses.getPlayerCameraXPosition = functionInitializer(m_offsets.getPlayerCameraXPosition);
        m_currentDynamicAddresses.getPlayerCameraZPosition = functionInitializer(m_offsets.getPlayerCameraZPosition);
        m_currentDynamicAddresses.getPlayerCameraYPosition = functionInitializer(m_offsets.getPlayerCameraYPosition);
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
                if ((ModuleEntry32.szModule == szModuleName) == 0) //Check if it doesnt break anything
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
