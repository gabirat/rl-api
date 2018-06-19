#ifndef RL_API_H
#define RL_API_H
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>

class RL_API
{
    public:
        RL_API();
        virtual ~RL_API();
        bool getPlayerJumpState();
        float getPlayerCameraXPosition();
        float getPlayerCameraYPosition();
        float getPlayerCameraZPosition();

    private:
        DWORD         m_playerJumpStateAddress;
        DWORD         m_playerXPositionAddress; //next positions are just offested 4 bytes from each other in order ( X , Z , Y )
        HWND          m_game;
        DWORD         m_processId;
        DWORD         m_gameModuleAddr;
        bool          m_hookedToGame;
        HANDLE        m_gameProcessHandle;
        struct OffsetsStruct {
            std::vector<DWORD> getJumpState;
            std::vector<DWORD> getPlayerCameraXPosition;
            std::vector<DWORD> getPlayerCameraYPosition;
            std::vector<DWORD> getPlayerCameraZPosition;
        };
        struct CurrentDynamicAddressesStruct {
            DWORD getJumpState;
            DWORD getPlayerCameraXPosition;
            DWORD getPlayerCameraYPosition;
            DWORD getPlayerCameraZPosition;
        };
        OffsetsStruct m_offsets;
        CurrentDynamicAddressesStruct m_currentDynamicAddresses;
        DWORD_PTR dwGetModuleBaseAddress(DWORD dwProcID, TCHAR *szModuleName);
        DWORD functionInitializer(std::vector<DWORD> offsets);
};

#endif // RL_API_H
