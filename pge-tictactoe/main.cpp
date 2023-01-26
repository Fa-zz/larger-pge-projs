#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <list>
#include <iterator>

#include "stringButton.cpp"

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
        bool imageChecked = false;
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

    // Buttons
    int NUM_BUTTONS = 2;
    stringButton playAgainButton{600, 300, "Play Again", 590, 290, 180, 35};
    stringButton quitButton{700, 400, "Quit", 690, 390, 180, 35};
    std::list<stringButton> myButtons {playAgainButton, quitButton};

    // Info for FPS and time
    float fTargetFrameTime = 1.0f / 100.0f; // Virtual FPS of 100fps
    float fAccumulatedTime = 0.0f;
    float timer = 0;

    bool playerDone = false;    // playerDone: Flag that determines if user has played their turn. Set to false every update
    int numQuadrantsFilled = 0;

    void resetProcedure() {
        for (int i = 0; i < NUM_QUADS; i++) {
            std::cout << "Reset procedure reached" << std::endl;
            myQuadrants[i].imageDrawn = false;
            myQuadrants[i].imageKind = 0;
            myQuadrants[i].imageChecked = false;
            timer = 0;
            playerDone = false;
            numQuadrantsFilled = 0;
        }
    }


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

        // int pAStartX = 600;
        // int pAStartY = 300;
        // std::string pAText = "Play Again";
        // int pARectX = 590;
        // int pARectY = 290;
        // int pARectScaleX = 180;
        // int pARectScaleY = 35;


        /*
        Create buttons, create array of buttons. Set visibility appropriately.
        In main game loop, use for loop to iterate through array. If visible, call makeButton 
        If mouse clicks within bounds, call buttonFunctionality
        */
       
        // stringButton playAgainButton(600, 300, "Play Again", 590, 290, 180, 35);
        // stringButton quitButton(700, 400, "Quit", 690, 390, 180, 35);
        playAgainButton.setVisible(false);
        quitButton.setVisible(false);

        // stringButton myStrButtons[2] = {playAgainButton, quitButton};

        // Creates the button when visible=true
        // void makeButton() {
        //     olc::PixelGameEngine::DrawStringDecal(olc::vi2d(textStartX, textStartY), buttonText, olc::BLACK, olc::vi2d(2,2));
        //     olc::PixelGameEngine::DrawRect(olc::vi2d(buttonStartX, buttonStartY), olc::vi2d(scaleX, scaleY), olc::MAGENTA);
        // }

        return true;

    }

    bool OnUserUpdate(float fElapsedTime) override {
        auto start = std::chrono::system_clock::now();

        // Info for FPS/game time
        fAccumulatedTime += fElapsedTime;
        timer += fElapsedTime;
        if (fAccumulatedTime >= fTargetFrameTime)
        {
            fAccumulatedTime -= fTargetFrameTime;
            fElapsedTime = fTargetFrameTime;
        }
        else
            return true; // Don't do anything this frame


        // Erase previous frame
        Clear(olc::WHITE);

        SetPixelMode(olc::Pixel::MASK);

        //textStartX, int textStartY, std::string buttonText, int buttonStartX, int buttonStartY, int scaleX, int scaleY

        // CHECKING BUTTONS
        for (auto it = myButtons.begin(); it != myButtons.end(); ++it) {
            if (it->getVisible() == true) {
                // draw button if it's supposed to be visible
                DrawRect(olc::vi2d(it->getButtonStartX(), it->getButtonStartY()), olc::vi2d(it->getScaleX(), it->getScaleY()), olc::MAGENTA);
                DrawStringDecal(olc::vi2d(it->getTextStartX(), it->getTextStartY()), it->getButtonText(), olc::BLACK, olc::vi2d(2,2));
            }
        }

        // HANDLE END GAME
        // Check to see if all quadrants are filled. If so, reset
        for (int i = 0; i < NUM_QUADS; i++) {
            if (myQuadrants[i].imageDrawn && myQuadrants[i].imageChecked == false) {
                numQuadrantsFilled += 1;
                myQuadrants[i].imageChecked = true;
            }
        }

        if (numQuadrantsFilled == 9) {
            resetProcedure();
        }

        // PLAYER TURN
        if (GetMouse(0).bPressed) {
            std::cout << "X pressed: " << GetMouseX() << std::endl;
            std::cout << "Y pressed: " << GetMouseY() << std::endl;

            // When user presses on screen, determine if it occurs within the boundaries of a quadrant space
            // And whether an image is drawn there
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
                    //timer = 0;
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
        std::string currentTurnString1 = "It's the";
        std::string currentTurnString2;
        if (playerDone) {
            currentTurnString2 = "computer's turn";
        } else if (playerDone == false) {
            currentTurnString2 = "player's turn";
        }
        DrawStringDecal(olc::vi2d(515, 25), "Welcome to Tic Tac Toe!", olc::BLACK, olc::vi2d(2,2));
        DrawStringDecal(olc::vi2d(590, 75), currentTurnString1, olc::BLACK, olc::vi2d(3,2));
        DrawStringDecal(olc::vi2d(525, 100), currentTurnString2, olc::BLACK, olc::vi2d(3,2));
    
        //DrawString(olc::vi2d(600, 100), currentTurnString, olc::BLACK);

        // COMPUTER TURN
        float delayTime = 3.00f;    // To standarize time, timer should be set to 0 every time playerDone = true. As it is right now
                                    // time is basically random.
        while (timer >= delayTime) {    // This creates a "delay" before the computer's turn
            timer -= delayTime;
            // Computer will take turn if player is done theirs
            if (playerDone) {

                bool computerDone = false;
                int computersChoice;
                while (!(computerDone)) {
                    computersChoice = rand() % 8 + 0;

                    if (myQuadrants[computersChoice].imageDrawn == false)
                        computerDone = true;
                }
                myQuadrants[computersChoice].imageKind = 0;
                myQuadrants[computersChoice].imageDrawn = true;
                playerDone = false;
            }

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