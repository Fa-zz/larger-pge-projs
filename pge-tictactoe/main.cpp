#include <time.h>
#include <thread>
#include <chrono>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace std::chrono_literals;

class TicTacToe : public olc::PixelGameEngine {

public:
    TicTacToe() {
        sAppName = "Tic Tac Toe Clone";
    }
    int WINDOW_WIDTH = 900;             // WINDOW_WIDTH: Width of window. Larger than game width to be able to have text on the side
    int WINDOW_HEIGHT = 480;            // WINDOW_HEIGHT: Height of window
    int GAME_WIDTH = 512;               // GAME_WIDTH: Width of the game. Used in drawing the TTT borders and images
    int GAME_HEIGHT = 480;              // GAME_HEIGHT: Height of the game

private:

struct Cell {                           // Cell objs are used to construct each "quadrant" of the TTT board, including where images are drawn within each quadrant
    olc::vi2d coordsStart;              // coordsStart: The start of the coords of each quadrants' space
    olc::vi2d coordsEnd;                // coordsEnd: The end of the coords of the each quadrants' space
    olc::vi2d coordsImageStart;         // coordsImageStart: The start of the coords of each image within each quadrant
    olc::vi2d imageSize = { 128,128 };  // imageSize: Size of each image
    
    int imageKind = 0;                  // imageKind: 1 represents an image drawn by a player, 0 represents image drawn by the computer
    bool imageDrawn = false;            // imageDrawn: Whether or not an image should be drawn at this quadrant
    };

    int X_DIFF = 180;                   // X_DIFF: Each quadrant occupies a width of 180 px. Used in determining coordsStart for each quadrant.
    int Y_DIFF = 170;                   // Y_DIFF: Each quadrant occupies a height of 170 px.
    int Y_DIFF_EACHROW = 160;           // Y_DIFF_EACHROW: There is a space of 160 px between rows.
    int NUM_QUADS = 9;                  // NUM_QUADS: Number of quadrants

    // Creation of quadrants using Cell struct, and an array which holds each
    struct Cell Quadrant1, Quadrant2, Quadrant3, Quadrant4, Quadrant5, Quadrant6, Quadrant7, Quadrant8, Quadrant9;

    struct Cell myQuadrants[9] = {
        Quadrant1, Quadrant2, Quadrant3,
        Quadrant4, Quadrant5, Quadrant6,
        Quadrant7, Quadrant8, Quadrant9
    };

    // Info for FPS and time
    float fTargetFrameTime = 1.0f / 100.0f; // Virtual FPS of 100fps
    float fAccumulatedTime = 0.0f;

public:
    bool OnUserCreate() override {

        // Random seed
        srand(time(0));

        // The following 4 variables and for loop are used to initialize each Quadrants' coordsStart and coordsEnd, in other words, the space they occupy

        int addByThisX = 0;                         // addByThisX: X val of each coordsStart is added with this val, is reset every row (every 3 quadrants)
        int addByThisY = 0;                         // addByThisY: Y val of each coordsEnd is added with this val and Y_DIFF, is reset every row
        int addByThisYEachRowStart = 0;             // addBythisYEachRowStart: Y val of coordsStart is added with this val, is added with Y_DIFF_EACHROW every row
        int addByThisYEachRowEnd = Y_DIFF_EACHROW;  // addByThisYEachRowEnd: Y val of coordsEnd is added with this val, is added with Y_DIFF_EACHROW every row

        for (int i = 1; i < NUM_QUADS+1; i++) { // We have to set i = 1 bc we use mod to determine when to reset vals, which doesn't work properly if i = 1

            // coordsStart and coordsEnd are defined with some values
            myQuadrants[i-1].coordsStart = olc::vi2d(0 + addByThisX, 1 + addByThisYEachRowStart);
            myQuadrants[i-1].coordsEnd = olc::vi2d(Y_DIFF + addByThisY, addByThisYEachRowEnd);

            addByThisX += X_DIFF;
            addByThisY += Y_DIFF;

            // Resetting of values each row, or every 3 quadrants
            if (i % 3 == 0 && i != 0) {
                addByThisX = 0;
                addByThisY = 0;
                addByThisYEachRowEnd += Y_DIFF_EACHROW;
                addByThisYEachRowStart += Y_DIFF_EACHROW;
            }
        }

        // Hard coded values for coordsImageStart are as follows
        myQuadrants[0].coordsImageStart = olc::vi2d(20, 10);
        myQuadrants[1].coordsImageStart = olc::vi2d((GAME_WIDTH/2) - 62, 10);
        myQuadrants[2].coordsImageStart = olc::vi2d(((GAME_WIDTH/2) + 60 + 52), 10);

        myQuadrants[3].coordsImageStart = olc::vi2d(18, 5 + 128 + 44);
        myQuadrants[4].coordsImageStart = olc::vi2d((GAME_WIDTH/2) - 62, 5 + 128 + 44);
        myQuadrants[5].coordsImageStart = olc::vi2d(((GAME_WIDTH/2) + 60 + 52), 5 + 128 + 44);

        myQuadrants[6].coordsImageStart = olc::vi2d(18, 5 + 128 + 128 + 44 + 40);
        myQuadrants[7].coordsImageStart = olc::vi2d((GAME_WIDTH/2) - 62, 5 + 128 + 128 + 44 + 40);
        myQuadrants[8].coordsImageStart = olc::vi2d(((GAME_WIDTH/2) + 60 + 52), 5 + 128 + 128 + 44 + 40);

        return true;

    }

    bool OnUserUpdate(float fElapsedTime) override {
        bool playerDone = false;    // playerDone: Flag that determines if user has played their turn. Set to false every update

        // Info for FPS/game time
        fAccumulatedTime += fElapsedTime;
        if (fAccumulatedTime >= fTargetFrameTime)
        {
            fAccumulatedTime -= fTargetFrameTime;
            fElapsedTime = fTargetFrameTime;
        }
        else
            return true; // Don't do anything this frame

        // Continue as normal
        std::cout << "TIME ELASPED: " << fAccumulatedTime << std::endl;

        // Erase previous frame
        Clear(olc::WHITE);

        SetPixelMode(olc::Pixel::MASK);

        // PLAYER TURN
        if (GetMouse(0).bPressed) {
            std::cout << "X pressed: " << GetMouseX() << std::endl;
            std::cout << "Y pressed: " << GetMouseY() << std::endl;

            // When user presses on screen, determine if it occurs within the boundaries of a quadrant space
            // And if there isn't already an image drawn there
            for (int i = 0; i < NUM_QUADS; i++) {
                if ( 
                (GetMouseX() >= myQuadrants[i].coordsStart.x) &&
                (GetMouseY() >= myQuadrants[i].coordsStart.y) &&
                (GetMouseX() <= myQuadrants[i].coordsEnd.x) &&
                (GetMouseY() <= myQuadrants[i].coordsEnd.y) &&
                (myQuadrants[i].imageDrawn == false)
                ) {
                    // If within bounds and imageDrawn == false, set the Quadrant imageKind to 1, imageDrawn to true, and plyaerDone to true
                    std::cout << "Image to be drawn at Quadrant" << i << "!" << std::endl;
                    myQuadrants[i].imageKind = 1;
                    myQuadrants[i].imageDrawn = true;
                    playerDone = true;
                }
            }
        }

        // DRAW STUFF

        // Draw borders

        // Left hand vertical
        FillRect( olc::vi2d(170-5, 0), olc::vi2d((GAME_WIDTH/32), GAME_HEIGHT), olc::DARK_CYAN );
        // Right hand verital
        FillRect( olc::vi2d(170+170-5, 0), olc::vi2d((GAME_WIDTH/32), GAME_HEIGHT), olc::DARK_CYAN );
        // Top horizontal
        FillRect( olc::vi2d(0, (GAME_HEIGHT/32) + 32 + 100), olc::vi2d(GAME_WIDTH, (GAME_HEIGHT/32)), olc::DARK_CYAN );
        // Bottom horizontal
        FillRect( olc::vi2d(0, (GAME_HEIGHT/32) + (GAME_HEIGHT/2) + 65 ), olc::vi2d(GAME_WIDTH, (GAME_HEIGHT/32)), olc::DARK_CYAN );

        // Draw images
        for (int i = 0; i < NUM_QUADS; i++) {
            if (myQuadrants[i].imageDrawn == true) {

                // If imageKind is 1, draw player obj (Blue color), if imageKind is 0, draw comp obj (Red color)
                if (myQuadrants[i].imageKind == 1)
                    FillRect(myQuadrants[i].coordsImageStart, myQuadrants[i].imageSize, olc::BLUE);
                if (myQuadrants[i].imageKind == 0)
                    FillRect(myQuadrants[i].coordsImageStart, myQuadrants[i].imageSize, olc::RED);
            }
        }

        // Draw text
        DrawString(600, 100, "Welcome to Tic Tac Toe!", olc::BLACK);

        // COMPUTER TURN
        if (playerDone) {
            float timeStamp = fAccumulatedTime;
            bool timeCheck = false;

            float delay = 10.0f;

            bool computerDone = false;
            int computersChoice;
            while (!(computerDone)) {
                computersChoice = rand() % 8 + 0;

                if (myQuadrants[computersChoice].imageDrawn == false)
                    computerDone = true;
            }
            myQuadrants[computersChoice].imageKind = 0;
            myQuadrants[computersChoice].imageDrawn = true;
            
            // while(!(timeCheck)) {
            //     std::cout << "Timestamp + 0.25: " << timeStamp + 0.05 << std::endl;
            //     if (fAccumulatedTime == (timeStamp + 0.25)) {
            //         myQuadrants[computersChoice].imageDrawn = true;
            //         std::cout << "Time check out" << std::endl;

            //         timeCheck = false;
            //     }
            // }
        }

        SetPixelMode(olc::Pixel::NORMAL);

        return true;
    }
};

int main() {
    TicTacToe game;
    if (game.Construct(game.WINDOW_WIDTH, game.WINDOW_HEIGHT, 2, 2))
        game.Start();
    return 0;
}