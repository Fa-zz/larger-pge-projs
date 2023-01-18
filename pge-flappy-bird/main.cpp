#include <iostream>
#include <string>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class OneLoneCoder_FlappyBird : public olc::PixelGameEngine {
public:
    OneLoneCoder_FlappyBird() {
        sAppName = "Flappy Bird";
    }

private:

};

int main() {
    OneLoneCoder_FlappyBird game;
    if (game.Construct(640, 300, 2, 2))
        game.Start();
    return 0;
}