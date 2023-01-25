#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

/*
    Each cell in grid is represented by a struct obj, containing member vars and 1 member func
    Vars
        coordsStart: Starting coords of cell
        coordsEnd: Ending coords of cell
        coordsImageStart: Starting coords of image (in cell)
        coordsImageEnd: Ending coords of image (in cell)
        imageKind: What type of image to draw (player's or computer's)
        imageDrawn: Used to check whether an image is already drawn, and if not, if it should be
    Funcs
        Draw (): Uses member vars to draw at location
*/

class TicTacToe : public olc::PixelGameEngine {

public:
    TicTacToe() {
        sAppName = "Tic Tac Toe Clone";
    }
    int WINDOW_WIDTH = 900;
    int WINDOW_HEIGHT = 480;
    int GAME_WIDTH = 512;
    int GAME_HEIGHT = 480;

private:

struct Cell {
    olc::vi2d coordsStart;              // coordsStart
    olc::vi2d coordsEnd;
    olc::vi2d coordsImageStart;
    //olc::vi2d coordsImageEnd;
    olc::vi2d imageSize = { 128,128 };
    
    int imageKind = 0;
    bool imageDrawn = false;
    };

    int X_DIFF = 180;
    int Y_DIFF = 170;
    int Y_DIFF_EACHROW = 160;
    int NUM_QUADS = 9;
    struct Cell Quadrant1, Quadrant2, Quadrant3, Quadrant4, Quadrant5, Quadrant6, Quadrant7, Quadrant8, Quadrant9;

    struct Cell myQuadrants[9] = {
        Quadrant1, Quadrant2, Quadrant3,
        Quadrant4, Quadrant5, Quadrant6,
        Quadrant7, Quadrant8, Quadrant9
    };

    float fTargetFrameTime = 1.0f / 100.0f; // Virtual FPS of 100fps
    float fAccumulatedTime = 0.0f;

public:
    bool OnUserCreate() override {
    /*

    Quadrant1: start(0, 1), end(165, 147)
    Quadrant2: start(180, 1), end(333, 147)
    Quadrant3: start(350, 1), end(512, 147)
    Difference in X: 180
    Difference in Y: 170

    Quadrant4: start(1, 163), end(165, 307)
    Quadrant5: start(180, 162), end(333, 307)
    Quadrant6: start(350, 163), end(512, 307)
    Each row, quadrants go down 160 pixels

    Quadrant7: start(1, 336), end(165, 480)
    Quadrant8: start(180, 336), end(333, 480)
    Quadrant9: start(350, 336), end(512, 480)

*/
    int addByThisX = 0;
    int addByThisY = 0;
    int addByThisYEachRowStart = 0;
    int addByThisYEachRowEnd = Y_DIFF_EACHROW;

    for (int i = 1; i < NUM_QUADS+1; i++) {
        // std::cout << "addByThisX: " << addByThisX << ", i: " << i << std::endl;

        myQuadrants[i-1].coordsStart = olc::vi2d(0+addByThisX, 1 + addByThisYEachRowStart);
        myQuadrants[i-1].coordsEnd = olc::vi2d(Y_DIFF + addByThisY, addByThisYEachRowEnd);

        addByThisX += X_DIFF;
        addByThisY += Y_DIFF;

        if (i % 3 == 0 && i != 0) {
            // std::cout << "i % 3 == 0, when i is " << i << std::endl;
            addByThisX = 0;
            addByThisY = 0;
            addByThisYEachRowEnd += Y_DIFF_EACHROW;
            addByThisYEachRowStart += Y_DIFF_EACHROW;
        }
    }

    // for (int i = 0; i < NUM_QUADS; i++) {
    //     std::cout << "Quadrant" << i+1 << " is as follows" << std::endl;
    //     std::cout << "start x: " << myQuadrants[i].coordsStart.x << " " << "start y: " << myQuadrants[i].coordsStart.y << std::endl;
    //     std::cout << "end x: " << myQuadrants[i].coordsEnd.x << " " << "start y: " << myQuadrants[i].coordsEnd.y << std::endl;

    // }

    /*

        Quadrant1.coordsImageStart = olc::vi2d(18, 5);
        Quadrant2.coordsImageStart = olc::vi2d((GAME_WIDTH/2) - 62, 5);
        Quadrant3.coordsImageStart = olc::vi2d(((GAME_WIDTH/2) + 60 + 52), 5);
        Quadrant4.coordsImageStart = olc::vi2d(18, 5 + 128 + 44);
        Quadrant5.coordsImageStart = olc::vi2d((GAME_WIDTH/2) - 62, 5 + 128 + 44);
        Quadrant6.coordsImageStart = olc::vi2d(((GAME_WIDTH/2) + 60 + 52), 5 + 128 + 44);
        Quadrant7.coordsImageStart = olc::vi2d(18, 5 + 128 + 128 + 44 + 44);
        Quadrant8.coordsImageStart = olc::vi2d((GAME_WIDTH/2) - 62, 5 + 128 + 128 + 44 + 44);
        Quadrant9.coordsImageStart = olc::vi2d(((GAME_WIDTH/2) + 60 + 52), 5 + 128 + 128 + 44 + 44);


*/
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

        fAccumulatedTime += fElapsedTime;
        if (fAccumulatedTime >= fTargetFrameTime)
        {
            fAccumulatedTime -= fTargetFrameTime;
            fElapsedTime = fTargetFrameTime;
        }
        else
            return true; // Don't do anything this frame

        // Continue as normal


        // Erase previous frame
        Clear(olc::WHITE);

        SetPixelMode(olc::Pixel::MASK);

        // // Draw first row of blocks
        // FillRect( Quadrant1.coordsImageStart, vBlockSize, olc::RED );
        // FillRect( olc::vi2d( (GAME_WIDTH/2) - 62 , 5 ), vBlockSize, olc::GREEN );
        // FillRect( olc::vi2d( ((GAME_WIDTH/2) + 60 + 52) , 5 ), vBlockSize, olc::BLUE );

        // // Draw second row of blocks
        // FillRect( olc::vi2d( 18, 5 + 128 + 44 ), vBlockSize, olc::RED );
        // FillRect( olc::vi2d( (GAME_WIDTH/2) - 62 , 5 + 128 + 44 ), vBlockSize, olc::GREEN );
        // FillRect( olc::vi2d( ((GAME_WIDTH/2) + 60 + 52) ,5 + 128 + 44 ), vBlockSize, olc::BLUE );

        // // Draw third row of blocks
        // FillRect( olc::vi2d( 18, 5 + 128 + 128 + 44 + 44 ), vBlockSize, olc::RED );
        // FillRect( olc::vi2d( (GAME_WIDTH/2) - 62 , 5 + 128 + 128 + 44 + 44 ), vBlockSize, olc::GREEN );
        // FillRect( olc::vi2d( ((GAME_WIDTH/2) + 60 + 52) , 5 + 128 + 128 + 44 + 44 ), vBlockSize, olc::BLUE );

        // USER INPUT

        /*
        Player clicks space on screen. Iterate through array of cell objs and use the following to check mouse x and y:  

        if ( (GetMouseX, GetMouseY) >= cell[i].coordsStart && (GetMouseX, GetMouseY) <= cell[i].coordsEnd ) && imageDrawn == false)
            cell[i].imageDrawn = true;

        In draw section, iterate through array of cell objs and if imageDrawn == true, use cell obj's draw function
        // */
        // for (int i = 0; i < NUM_QUADS; i++) {
        //     std::cout << "Quadrant" << i << " image drawn: " << myQuadrants[i].imageDrawn << std::endl;
        // }

        // DRAW BORDERS AND IMAGES

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
                //myQuadrants[i].DrawImage();
                // std::cout << "Quadrant" << i << " image should be drawn" << std::endl;
                // std::cout << "Quadrant" << i << " coordsImageStart: " << myQuadrants[i].coordsImageStart.x << ", " << myQuadrants[i].coordsImageStart.y << std::endl;
                FillRect(myQuadrants[i].coordsImageStart, myQuadrants[i].imageSize, olc::BLUE);
            }
        }

        // USER INPUT

        if (GetMouse(0).bPressed) {
            std::cout << "X pressed: " << GetMouseX() << std::endl;
            std::cout << "Y pressed: " << GetMouseY() << std::endl;

            for (int i = 0; i < NUM_QUADS; i++) {
                if ( 
                (GetMouseX() >= myQuadrants[i].coordsStart.x) &&
                (GetMouseY() >= myQuadrants[i].coordsStart.y) &&
                (GetMouseX() <= myQuadrants[i].coordsEnd.x) &&
                (GetMouseY() <= myQuadrants[i].coordsEnd.y) &&
                (myQuadrants[i].imageDrawn == false)
                ) {
                    std::cout << "Image to be drawn at Quadrant" << i << "!" << std::endl;
                    myQuadrants[i].imageKind = 1;
                    myQuadrants[i].imageDrawn = true;
                }
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