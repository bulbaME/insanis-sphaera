#include "congl.hpp"
using namespace ConGL::_2D;

wchar_t menu0DT [8][22] = {
    L"   Insanis Sphaera   ",
    L"                     ",
    L"                     ",
    L"       START         ",
    L"                     ",
    L"       ABOUT         ",
    L"       CREDITS       ",
    L"       EXIT          "
};

wchar_t menu1DT [28][46] = {
    L"               Insanis Sphaera               ",
    L"                                             ",
    L"                                             ",
    L"                                             ",
    L"                 CODE   BULBA                ",
    L"                                             ",
    L"             GRAPHICS   BULBA                ",
    L"                                             ",
    L"                                             ",
    L"                                             ",
    L"                                             ",
    L"   This game was written using ConGL API:    ",
    L"      https://github.com/bulbaME/ConGL       ",
    L"                                             ",
    L"                                             ",
    L"                                             ",
    L"          Big thanks to everyone who         ",
    L"          helped creating this game!         ",
    L"                                             ",
    L"                                             ",
    L"        Watermelon - for funny pistacio      ",
    L"           Адвокат - for lovely gifs         ",
    L"        Flamingo - greatest beta tester      ",
    L"         Master - for awesome feedback       ",
    L"         Coffevarca - beta tester 2          ",
    L"          Dasha - for amazing alien          ",
    L"                                             ",
    L"                                             "
};

wchar_t menu2DT [17][82] = {
    L"    ACTION           KEY                    Main goal is to keep shpere stable.  ",
    L"Close              [ESCAPE]                 You can achieve that dragging and    ",
    L"Pause / Resume     [ESCAPE]                 collapsing particles in the sphere.  ",
    L"Select             [RETURN]                 Avoid enemy particles.               ",
    L"Pause credits      [SPACE]                                                       ",
    L"                                            No stability makes sphere collapse.  ",
    L"Pick particle      [ARROWS]                                                      ", 
    L"Select particle    [RETURN]                 Sphere is growing when particles are ",
    L"Move particle      [WASD]                   collapsing inside of it.             ",
    L"Speed up particle  [SPACE]                                                       ",
    L"Collapse particle [E]                       Also you have to keep all particle   ",
    L"                                            levels balanced. If there is too much",
    L"  ====== PARTICLES ======                   or too little of any particle shpere ",
    L"[@] - particle A                            collapses.                           ",
    L"[8] - particle B                                                                 ",
    L"[¢] - particle C                                                                 ",
    L"[Z] - enemy particle                                                             "
};

wchar_t uiDT [3][19] = {
    L"STB [            ]",
    L"BLC [            ]",
    L"EXP [            ]"
};

wchar_t pause1DT [7][22] = {
    L"       PAUSED        ",
    L"      STAGE          ",
    L"                     ",
    L"       RESUME        ",
    L"       RESTART       ",
    L"       MAIN MENU     ",
    L"       EXIT          "
};

wchar_t pause2DT [6][22] = {
    L"   Sphaera Concidit  ",
    L"     STAGE           ",
    L"                     ",
    L"       RESTART       ",
    L"       MAIN MENU     ",
    L"       EXIT          "
};

wchar_t pause3DT [6][22] = {
    L" Sphaera Confirmatae ",
    L"     STAGE           ",
    L"                     ",
    L"       RESTART       ",
    L"       MAIN MENU     ",
    L"       EXIT          "
};

wchar_t easterEgg1DT [7][10] = {
 L"  X   X  ",
 L"   X X   ",
 L"  XXXXX  ",
 L" XX X XX ",
 L" XXXXXXX ",
 L"X X   X X",
 L"   X X   "
};

wchar_t easterEgg2DT [7][10] = {
 L"  X   X  ",
 L"   X X   ",
 L"X XXXXX X",
 L" XX X XX ",
 L" XXXXXXX ",
 L"  X   X  ",
 L" X     X "
};

wchar_t uiTxtDT [1][6] = { {L"STAGE"} };

txr::Texture menu0TXR;
txr::Texture menu1TXR;
txr::Texture menu2TXR;
txr::Texture uiTXR;
txr::Texture uiTxtTXR;
txr::Texture pause1TXR;
txr::Texture pause2TXR;
txr::Texture pause3TXR;
txr::Texture easterEgg1TXR;
txr::Texture easterEgg2TXR;

void spriteInit() {
    menu0TXR.setProper<22, 8>(menu0DT);
    menu1TXR.setProper<46, 28>(menu1DT);
    menu2TXR.setProper<82, 17>(menu2DT);
    uiTXR.setProper<19, 3>(uiDT);
    uiTxtTXR.setProper<6, 1>(uiTxtDT);
    pause1TXR.setProper<22, 7>(pause1DT);
    pause2TXR.setProper<22, 6>(pause2DT);
    pause3TXR.setProper<22, 6>(pause3DT);
    easterEgg1TXR.setProper<10, 7>(easterEgg1DT);
    easterEgg2TXR.setProper<10, 7>(easterEgg2DT);
}