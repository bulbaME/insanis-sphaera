#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>
#include "congl.hpp"
#include "sprites.cpp"
using namespace ConGL;
using namespace _2D;
using namespace std;

Screen screen(true);

//====== LAYOUTS ======= 

int currL = 0;
int prevL = 0;
bool switchedL = true;
bool loop = true;

// menu
Layout mainMenu(&screen);
Layout creditsMenu(&screen);
Layout controlsMenu(&screen);

// state, counter, y
int creditsMenuSt[3] = {0, 0, 0};
// selected button
int mainMenuSt = 1;

int gameSt = 0;

// game 
Layout pauseScene(&screen);
Layout mainUI(&screen);
Layout mainScene(&screen);
Layout mainPause(&screen);

int pb = 0;

void switchL(int l) {
    if (currL == l) return;
    prevL = currL;
    currL = l;
    switchedL = true;
}

//======================

#include "classes.cpp"

int main() {
    srand(time(nullptr));

    screen.autosizeToggle(true);
    screen.setFpsLimit(80);
    spriteInit();
    ui.init();

    shapes::Sprite mainMenuSPR(&menu0TXR);
    shapes::Sprite creditsMenuSPR(&menu1TXR);
    shapes::Sprite controlsMenuSPR(&menu2TXR);
    mainMenu.addFigure(&mainMenuSPR);
    creditsMenu.addFigure(&creditsMenuSPR);
    controlsMenu.addFigure(&controlsMenuSPR);
    mainMenuSPR.setPos({0, 15});
    controlsMenuSPR.setPos({0, 15});

    shapes::Sprite pause1SPR(&pause1TXR);
    shapes::Sprite pause2SPR(&pause2TXR);
    shapes::Sprite pause3SPR(&pause3TXR);
    pause1SPR.setPos({0, 15});
    pause2SPR.setPos({0, 15});
    pause3SPR.setPos({0, 15});

    shapes::Sprite easterEgg1SPR(&easterEgg1TXR);
    game.easterSPR = &easterEgg1SPR;
    easterEgg1SPR.setPos({0, -20});
    mainScene.addFigure(&easterEgg1SPR);
    
    int pauseIds[3];
    pauseIds[0] = mainPause.addFigure(&pause1SPR);
    pauseIds[1] = mainPause.addFigure(&pause2SPR);
    pauseIds[2] = mainPause.addFigure(&pause3SPR);

    // game loop
    while (loop) {
        // when layout is switched
        if (switchedL) {
            switchedL = false;
            mainMenuSt = 0;
            if (currL < 4 || gameSt > 1) {
                screen.setFont({10, 25});
            } else screen.setFont({8, 8});

            switch (currL) {
            case 0:
                screen.setSurfaceSize({menu0TXR.size.X, 200});
                break;
            case 1:
                screen.setSurfaceSize({menu1TXR.size.X, 200});
                creditsMenuSt[0] = 1;
                creditsMenuSt[1] = 0;
                creditsMenuSt[2] = screen.getSize().Y;
                creditsMenuSPR.setPos({0, screen.getSize().Y});
                break;
            case 2:
                screen.setSurfaceSize({menu2TXR.size.X, 200});
                break;
            case 3:
                screen.setSurfaceSize({22, 200});
                break;
            case 4:
                mainPause.disableFigure(pauseIds[0]);
                mainPause.disableFigure(pauseIds[1]);
                mainPause.disableFigure(pauseIds[2]);

                if (gameSt == 1) 
                    screen.setSurfaceSize(Game::surfaceSize);
                else if (gameSt == 2) {
                    screen.setSurfaceSize({pause1TXR.size.X, 200});
                    mainPause.enableFigure(pauseIds[0]);
                } else {
                    screen.setSurfaceSize({pause2TXR.size.X, 200});

                    if (gameSt == 3) mainPause.enableFigure(pauseIds[1]);
                    else mainPause.enableFigure(pauseIds[2]);
                }
            }
        }

        // processing
        switch(currL) {
        // main menu
        case 0:
            if (keys::up(VK_UP)) mainMenuSt = mainMenuSt > 1 ? mainMenuSt - 1 : 4;
            else if (keys::up(VK_DOWN)) mainMenuSt = mainMenuSt < 4 ? mainMenuSt + 1 : 1;

            if (pb) {
                screen.setPX_s({6, 16}, L'S');
                screen.setPX_s({7, 16}, L'T');
                screen.setPX_s({8, 16}, L'A');
                screen.setPX_s({9, 16}, L'G');
                screen.setPX_s({10, 16}, L'E');
                string stageStr = to_string(pb);
                for (int x = 0; x < stageStr.length(); ++x) 
                    screen.setPX_s({short (12 + x), 16}, stageStr[x]);
            }

            if (keys::down(VK_RETURN)) {
                switch (mainMenuSt) {
                case 1:
                    switchL(4);
                    gameSt = 1;
                    break;
                case 2:
                    switchL(2);
                    break;
                case 3:
                    switchL(1);
                    break;
                case 4:
                    loop = false;
                }
            }

            switch (mainMenuSt) {
            case 1:
                screen.setPX_s({5, 18}, L'>');
                break;
            case 2:
                screen.setPX_s({5, 20}, L'>');
                break;
            case 3:
                screen.setPX_s({5, 21}, L'>');
                break;
            case 4:
                screen.setPX_s({5, 22}, L'>');
            }

            mainMenu.draw();
            break;

        // credits
        case 1:
            if (creditsMenuSt[0] == 1) {
                if (keys::up(VK_SPACE)) creditsMenuSt[0] = 0;

                if (creditsMenuSt[1] > 50) {
                    creditsMenuSt[1] = 0;

                    creditsMenuSPR.setPos({0, (short) creditsMenuSt[2]--});
                    if (creditsMenuSt[2] < -20) switchL(0);
                }

                ++creditsMenuSt[1];
            } else if (keys::up(VK_SPACE)) creditsMenuSt[0] = 1;

            if (keys::up(VK_ESCAPE)) switchL(0);

            creditsMenu.draw();
            break;

        // controls
        case 2:
            if (keys::up(VK_ESCAPE)) switchL(prevL);
            controlsMenu.draw();
        break;

        // paused
        case 3:
            pauseScene.draw();
            break;
        
        // game
        case 4:
            if (gameSt == 1) game.update();
            else if (gameSt == 2) {
                if (keys::up(VK_UP)) mainMenuSt = mainMenuSt > 1 ? mainMenuSt - 1 : 4;
                else if (keys::up(VK_DOWN)) mainMenuSt = mainMenuSt < 4 ? mainMenuSt + 1 : 1;
                
                string stageStr = to_string(sphere.stage);
                for (int x = 0; x < stageStr.length(); ++x) 
                    screen.setPX_s({short (12 + x), 16}, stageStr[x]);

                if (keys::up(VK_ESCAPE)) {
                    gameSt = 1;
                    switchedL = true;
                }

                if (keys::up(VK_RETURN)) switch(mainMenuSt) {
                case 2:
                    game.restart();
                case 1: 
                    gameSt = 1;
                    switchedL = true;
                    break;
                case 3: 
                    gameSt = 0;
                    pb = sphere.stage > pb ? sphere.stage : pb;
                    switchL(0);
                    break;
                case 4: loop = false;
                }
                
                switch (mainMenuSt) {
                case 1:
                    screen.setPX_s({5, 18}, L'>');
                    break;
                case 2:
                    screen.setPX_s({5, 19}, L'>');
                    break;
                case 3:
                    screen.setPX_s({5, 20}, L'>');
                    break;
                case 4:
                    screen.setPX_s({5, 21}, L'>');
                }

                mainPause.draw();
            } else {
                if (keys::up(VK_UP)) mainMenuSt = mainMenuSt > 1 ? mainMenuSt - 1 : 3;
                else if (keys::up(VK_DOWN)) mainMenuSt = mainMenuSt < 3 ? mainMenuSt + 1 : 1;
                
                if (keys::up(VK_ESCAPE)) switchL(0);
                if (keys::up(VK_RETURN)) switch(mainMenuSt) {
                case 1:
                    game.restart();
                    gameSt = 1;
                    switchedL = true;
                    break;
                case 2: 
                    game.restart();
                    gameSt = 1;
                    pb = sphere.stage < pb ? pb : sphere.stage;
                    switchL(0);
                    break;
                case 3: loop = false;
                }

                string stageStr = to_string(sphere.stage);
                for (int x = 0; x < stageStr.length(); ++x) 
                    screen.setPX_s({short (12 + x), 16}, stageStr[x]);
                
                switch (mainMenuSt) {
                case 1:
                    screen.setPX_s({5, 18}, L'>');
                    break;
                case 2:
                    screen.setPX_s({5, 19}, L'>');
                    break;
                case 3:
                    screen.setPX_s({5, 20}, L'>');
                }

                mainPause.draw();
            }
            
            break;
        default:
            break;
        }
    }

    return 0;
}