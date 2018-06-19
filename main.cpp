#include <iostream>
#include <iomanip>
#include <windows.h>
#include "RL_API.h"

using namespace std;

char* playerJumped(bool state) {
    char* jump = " JUMP ";
    char* land = "LANDED";
    return (state ? jump : land);
}

float map(float value, float minV, float maxV) {
    return (value - minV) / (maxV - minV); //returns number ranging from 0 to 1
}

int main() {

    RL_API rlApi = RL_API();
    bool lastState = rlApi.getPlayerJumpState();
    /*
    while(true) {
        bool currState = rlApi.getPlayerJumpState();
        if( currState != lastState) {
            playerJumped(currState);
        }
        lastState = currState;
    }
    */
    bool inAir = false;
    while(true) {
        for(int i=0;i<11;i++) cout << "\n";
        for(int i=0;i<22;i++) cout << " ";
        cout << "X: " << fixed << setw(7) << setprecision(2) << rlApi.getPlayerCameraXPosition() << " | " <<  "Y: " << fixed << setw(7) << setprecision(2) << rlApi.getPlayerCameraYPosition() << " | " <<  "Z: " << fixed << setw(7) << setprecision(2) << rlApi.getPlayerCameraZPosition() << endl;
        for(int i=0;i<37;i++) cout << " ";
        cout << playerJumped(rlApi.getPlayerJumpState()) << endl;
        Sleep(100);
        system("cls");
    }
}
