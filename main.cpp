#include <iostream>
#include "RL_API.h"

using namespace std;

void playerJumped(bool state) {
    cout << (state ? "JUMP\n" : "LANDED\n");
}

int main() {

    RL_API rlApi = RL_API();
    bool lastState = rlApi.getPlayerJumpState();
    while(true) {
        bool currState = rlApi.getPlayerJumpState();
        if( currState != lastState) {
            playerJumped(currState);
        }
        lastState = currState;
    }
}
