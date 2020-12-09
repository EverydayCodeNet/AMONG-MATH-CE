#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>
#include <math.h>

/* Include the converted graphics file */
#include "gfx/gfx.h"

kb_key_t key;

typedef struct {
    //disable keyboard scan
    bool canPress;
    //used for start menu - if no save, new game.
    bool save;
} game_t;
game_t game;

typedef struct {
    bool impostor;
    bool debug;
    //left or right, 0 left, 1 right
    //should change this to bool name to "bool right"...
    bool dir;
    bool running;
    bool footdown;
    bool down;
    int xOffset, yOffset;
    int xpos, ypos;
    //currently used to keep player in center of screen and handle movement
    uint8_t startX, startY;
    uint8_t speed;
    //which side of map is the player on?
    uint8_t mapGroup;
    //which room is the player in?
    /*rooms - left to right
    upper engine, cafe, weapons
    reactor, cams, medbay, o2, nav
    lower, electrical, storage, admin, shields
    comms*/
    uint8_t room;
    //number of crewmates killed
    uint8_t killed;
    //vented?
    bool vented;
    //nearest dummy distance and index
    //dummydistance is dumb, could be dummies[dummyWho].variable instead
    int dummyDistance;
    uint8_t dummyWho;
    //second choice to reduce use of findDummy?, when main choice dies, switch to dummy?
} player_t;
player_t player;

typedef struct {
    int xpos,ypos;
    //soon...
    uint8_t speed;
    bool footdown;
    bool running;
    //only have the sprite for the suit standing and running outline
    //then gfx_fill the suit with new colors
    //int color;
    bool dead;
    //body fallen or standing?
    //bool fallen;
    int distance;
    uint8_t room;
} dummy_t;
dummy_t dummies[9];

typedef struct {
    int xpos,ypos;
    uint8_t room;
    //connecting vents
    uint8_t connecting;
} vent_t;
vent_t vents[14];

const char *appvarName = "slota";

void SaveData(void) {
    ti_var_t slota;
    ti_CloseAll();
    if (slota = ti_Open("AMGDAT","w+")) {
        ti_Write(&player,sizeof(player),1,slota);
        //ti_Write(&game,sizeof(game),1,slota);
        //ti_Write(&dummies,sizeof(dummies),1,slota);
    }
    ti_SetArchiveStatus(true, slota);
}

void LoadData(void) {
    ti_var_t slota;
    ti_CloseAll();
    if (slota = ti_Open("AMGDAT", "r")) {
        ti_Read(&player, sizeof(player),1,slota);
        //ti_Read(&game, sizeof(game),1,slota);
        //ti_Read(&dummies, sizeof(dummies),1,slota);
    }
}

void CreateSave(void) {
    ti_var_t slota;
    ti_CloseAll();
    if (slota = ti_Open("AMGDAT","w+")) {
        ti_Write(&player, sizeof(player), 1, slota);
        ti_SetArchiveStatus(true, slota);
        //ti_Write(&dummies, sizeof(dummies),1,slota);
        //ti_Write(&game, sizeof(game), 1, slota);
    }
}

void WhiText(void) {
    gfx_SetTextBGColor(0);
    gfx_SetTextFGColor(255);
    gfx_SetTextTransparentColor(0);
}

void YelText(void) {
    gfx_SetTextBGColor(0);
    gfx_SetTextFGColor(229);
    gfx_SetTextTransparentColor(0);
}

void BlaText(void) {
    gfx_SetTextBGColor(255);
    gfx_SetTextFGColor(0);
    gfx_SetTextTransparentColor(255);
}

void drawButtons() {
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(1);
    
    if (player.dummyDistance <= 50 && dummies[player.dummyWho].dead == false) {
        gfx_TransparentSprite(kill_button_new,195,180);
    } else {
        gfx_TransparentSprite(kill_cooldown,195,180);
    }

    //implement report and sabotage if AI is added
    //gfx_TransparentSprite(report, 255, 120);
    //if far from vent - remove sabotage
    //gfx_TransparentSprite(sabotage, 250, 190);
    
    //check if vent is in room, otherwise display vent_cooldown
    if (player.room == 0 || player.room == 3 || player.room == 4 || 
    player.room == 8 || player.room == 9 || player.room == 5 || player.room == 1 ||
    player.room == 11 || player.room == 12 || player.room == 7 || player.room == 2) {
        gfx_TransparentSprite(vent_button,255,180);
    } else if (player.xpos > 580 && player.xpos < 730 && player.ypos > 440 && player.ypos < 550) {
        //special condition for shields hallway
        gfx_TransparentSprite(vent_button,255,180);
    } else {
        gfx_TransparentSprite(vent_button_cooldown,255,180);
    }
    
}

void dispStats() {
    gfx_SetColor(10);
    gfx_FillRectangle(10,10,100,70);

    WhiText();
    gfx_PrintStringXY("Room: ",10,10);
    gfx_PrintInt(player.room,1);
    gfx_PrintStringXY("x: ",10,20);
    gfx_PrintInt(player.xpos,1);
    gfx_PrintStringXY("y: ",10,30);
    gfx_PrintInt(player.ypos,1);
    gfx_PrintStringXY("who: ",10,40);
    gfx_PrintInt(player.dummyWho,1);
    gfx_PrintStringXY("dist.: ",10,50);
    gfx_PrintInt(player.dummyDistance,1);

    //additional debug text
    /*gfx_PrintStringXY("Direction: ",10,10);
    gfx_PrintInt(player.dir,1);
    gfx_PrintStringXY("Running?: ",10,20);
    gfx_PrintInt(player.running,1);
    gfx_PrintStringXY("xOffset: ",10,10);
    gfx_PrintInt(player.xOffset,1);
    gfx_PrintStringXY("yOffset: ",10,20);
    gfx_PrintInt(player.yOffset,1);*/
}

void doCollisions() {
    //base it off of rooms and player direction
    //re-integrate hallways into rooms
}

//find, set, and print current location
void locationTracker() {    
    /*
    rooms - left to right
    upper engine, cafe, weapons
    reactor, cams, medbay, o2, nav
    lower, electrical, storage, admin, shields
    comms
    */
    //divide map into groups to speed up program
    if (player.xpos < 40) {
        //both engines, reactor, sec
       player.mapGroup = 0;
    } else if (player.xpos > 40 && player.xpos < 520 && player.ypos > 440) {
        //storage, admin, comms
        player.mapGroup = 1;
    } else if (player.xpos > 0 && player.xpos < 440 && player.ypos > 0 && player.ypos < 440) {
        //cafeteria
        player.mapGroup = 3;
    } else if (player.xpos > 520) {
        //weapons, o2, nav, shields
        //too hard to calculate exact coords
        player.mapGroup = 2;
    }
    //display current room
    YelText();
    if (player.mapGroup == 0) {
        if (player.xpos > -540 && player.xpos < -340 && player.ypos > 125 && player.ypos < 325) {
            player.room = 0;
            gfx_PrintStringXY("UPPER ENGINE",(160 - gfx_GetStringWidth("UPPER ENGINE") / 2),220);
        } else if (player.xpos > -695 && player.xpos < -510 && player.ypos > 300 && player.ypos < 625) {
            player.room = 3;
            gfx_PrintStringXY("REACTOR",(160 - gfx_GetStringWidth("REACTOR") / 2),220);
        } else if (player.xpos > -340 && player.xpos < -220 && player.ypos > 340 && player.ypos < 560) {
            player.room = 4;
            gfx_PrintStringXY("SECURITY",(160 - gfx_GetStringWidth("SECURITY") / 2),220);
        } else if (player.xpos > -540 && player.xpos < -340 && player.ypos > 605 && player.ypos < 820) {
            player.room = 8;
            gfx_PrintStringXY("LOWER ENGINE",(160 - gfx_GetStringWidth("LOWER ENGINE") / 2),220);
        } else if (player.xpos > -205 && player.xpos < -10 && player.ypos > 275 && player.ypos < 500) {
            player.room = 5;
            gfx_PrintStringXY("MEDBAY",(160 - gfx_GetStringWidth("MEDBAY") / 2),220);
        } else if (player.xpos > -170 && player.xpos < 40 && player.ypos > 530 && player.ypos < 800) {
            player.room = 9;
            gfx_PrintStringXY("ELECTRICAL",(160 - gfx_GetStringWidth("ELECTRICAL") / 2),220);
        }
    } else if (player.mapGroup == 1) {
        if (player.xpos > 40 && player.xpos < 280 && player.ypos > 590 && player.ypos < 945) {
            player.room = 10;
            gfx_PrintStringXY("STORAGE",(160 - gfx_GetStringWidth("STORAGE") / 2),220);
        } else if (player.xpos > 250 && player.xpos < 510 && player.ypos > 495 && player.ypos < 665) {
            player.room = 11;
            gfx_PrintStringXY("ADMIN",(160 - gfx_GetStringWidth("ADMIN") / 2),220);
        } else if (player.xpos > 285 && player.xpos < 495 && player.ypos > 795) {
            player.room = 13;
            gfx_PrintStringXY("COMMUNICATIONS",(160 - gfx_GetStringWidth("COMMUNICATIONS") / 2),220);
        } 
    } else if (player.mapGroup == 2) {
        if (player.xpos > 510 && player.ypos > 670) {
            player.room = 12;
            gfx_PrintStringXY("SHIELDS",(160 - gfx_GetStringWidth("SHIELDS") / 2),220);
        } else if (player.xpos > 850 && player.ypos > 345) {
            player.room = 7;
            gfx_PrintStringXY("NAVIGATION",(160 - gfx_GetStringWidth("NAVIGATION") / 2),220);
        } else if (player.xpos > 400 && player.xpos < 550 && player.ypos > 380 && player.ypos < 470) {
            player.room = 6;
            gfx_PrintStringXY("O2",(160 - gfx_GetStringWidth("O2") / 2),220);
        } else if (player.xpos > 515 && player.xpos < 680 && player.ypos > 125 && player.ypos < 310) {
            player.room = 2;
            gfx_PrintStringXY("WEAPONS",(160 - gfx_GetStringWidth("WEAPONS") / 2),220);
        }
    } else if (player.mapGroup == 3) {
        if (player.xpos > 0 && player.xpos < 430 && player.ypos > 0 && player.ypos < 440) {
            player.room = 1;
            gfx_PrintStringXY("CAFETERIA",(160 - gfx_GetStringWidth("CAFETERIA") / 2),220);
        }
    }
}

//slowest draw functions
void drawCafeteria() {
    //barriers needed
    gfx_SetColor(255);
    
    if (player.ypos > 230) {
        //bottom left
        gfx_FillTriangle(
            0 - player.xOffset,350 - player.yOffset,
            100 - player.xOffset,350 - player.yOffset,
            100 - player.xOffset,440 - player.yOffset
        );
        //bottom right
        gfx_FillTriangle(
            430 - player.xOffset,350 - player.yOffset,
            340 - player.xOffset,350 - player.yOffset,
            340 - player.xOffset,440 - player.yOffset
        );
        //bottom
        gfx_FillRectangle(100 - player.xOffset,350 - player.yOffset,240,90);
    } 
    if (player.ypos < 470) {
        if (player.xpos > 170) {
            //top right cafe
            gfx_FillTriangle(
                330 - player.xOffset,150 - player.yOffset,
                330 - player.xOffset,40 - player.yOffset,
                430 - player.xOffset,150 - player.yOffset
            );
            gfx_SetColor(1);
            //top right triangle wall
            gfx_FillTriangle(
                330 - player.xOffset, 0 - player.yOffset,
                330 - player.xOffset, 40 - player.yOffset,
                430 - player.xOffset, 150 - player.yOffset
            );
            gfx_FillTriangle(
                330 - player.xOffset, 0 - player.yOffset,
                430 - player.xOffset, 150 - player.yOffset,
                430 - player.xOffset, 110 - player.yOffset
            );
        } 
        if (player.xpos < 290) {
            gfx_SetColor(255);
            //top left cafe floor
            gfx_FillTriangle(
                0 - player.xOffset,110 - player.yOffset,
                130 - player.xOffset, 110 - player.yOffset,
                130 - player.xOffset, 40 - player.yOffset
            );
            gfx_SetColor(1);
            //top left triangle wall
            gfx_FillTriangle(
                0 - player.xOffset, 110 - player.yOffset,
                130 - player.xOffset, 0 - player.yOffset,
                0 - player.xOffset, 70 - player.yOffset
            );
            //bottom half
            gfx_FillTriangle(
                130 - player.xOffset,0 - player.yOffset,
                130 - player.xOffset,40 - player.yOffset,
                0 - player.xOffset,110 - player.yOffset
            );
        } 
        if (player.ypos < 160) {
            //top wall
            gfx_SetColor(1);
            gfx_FillRectangle(130 - player.xOffset,0 - player.yOffset,200,40);
        }
        //always displayed
        gfx_SetColor(255);
        //top 
        gfx_FillRectangle(130 - player.xOffset,40 - player.yOffset,200,70);
        gfx_FillRectangle(0 - player.xOffset,110 - player.yOffset,330,40);
        
        //middle
        gfx_FillRectangle(0 - player.xOffset,150 - player.yOffset,430,200);
    }
}

void drawWeapons() {
    gfx_SetColor(1);
    gfx_FillRectangle(515 - player.xOffset,90 - player.yOffset,165,35);
    gfx_FillRectangle(430 - player.xOffset,180 - player.yOffset,85,35);
    gfx_SetColor(255);
    //hallway to cafe
    gfx_FillRectangle(430 - player.xOffset,215 - player.yOffset,85,50);
    gfx_FillRectangle(515 - player.xOffset,125 - player.yOffset,165,185);
}

void drawO2() {
    gfx_SetColor(255);
    gfx_FillRectangle(460 - player.xOffset,335 - player.yOffset,80,105);
    gfx_FillRectangle(400 - player.xOffset,440 - player.yOffset,140,30);
    gfx_FillTriangle(
        460 - player.xOffset,380 - player.yOffset,
        400 - player.xOffset,440 - player.yOffset,
        460 - player.xOffset,440 - player.yOffset
    );
}

void drawNav() {
    gfx_SetColor(1);
    gfx_FillRectangle(850 - player.xOffset,345 - player.yOffset,80,35);
    gfx_SetColor(255);
    gfx_FillRectangle(835 - player.xOffset,435 - player.yOffset,15,55);
    gfx_FillRectangle(870 - player.xOffset,380 - player.yOffset,60,145);
    gfx_FillRectangle(850 - player.xOffset,380 - player.yOffset,60,145);
    gfx_FillTriangle(
        930 - player.xOffset,380 - player.yOffset,
        930 - player.xOffset,425 - player.yOffset,
        990 - player.xOffset,425 - player.yOffset
    );
    gfx_FillRectangle(930 - player.xOffset,425 - player.yOffset,60,55);
    gfx_FillTriangle(
        930 - player.xOffset,480 - player.yOffset,
        990 - player.xOffset,480 - player.yOffset,
        930 - player.xOffset,525 - player.yOffset
    );
}

void drawShields() {
    gfx_SetColor(1);
    gfx_FillRectangle(520 - player.xOffset,605 - player.yOffset,60,35);
    gfx_FillRectangle(640 - player.xOffset,605 - player.yOffset,55,35);
    gfx_SetColor(255);
    gfx_FillRectangle(520 - player.xOffset,640 - player.yOffset,175,180);
}

void drawMedBay() {
    //hallway wall
    gfx_SetColor(1);
    gfx_FillRectangle(-345 - player.xOffset,180 - player.yOffset,345,35);
    //doorway walls
    gfx_FillRectangle(-200 - player.xOffset,280 - player.yOffset,55,30);
    gfx_FillRectangle(-95 - player.xOffset,280 - player.yOffset,60,30);
    gfx_SetColor(255);
    //medbay hallway
    gfx_SetColor(255);
    gfx_FillRectangle(-345 - player.xOffset,215 - player.yOffset,345,50);
    gfx_FillRectangle(-200 - player.xOffset,310 - player.yOffset,165,105);
    //inlet to room
    gfx_FillRectangle(-145 - player.xOffset,250 - player.yOffset,50,60);
    //bottom right
    gfx_FillTriangle(
        -35 - player.xOffset,415 - player.yOffset,
        -35 - player.xOffset,480 - player.yOffset,
        35 - player.xOffset,480 - player.yOffset
    );
    gfx_FillRectangle(-150 - player.xOffset,415 - player.yOffset,115,80);
    gfx_FillRectangle(-35 - player.xOffset,480 - player.yOffset,70,15);
    //MEDBAY VENT
    gfx_FillRectangle(-200 - player.xOffset,415 - player.yOffset,50,45);
    //bottom left
    gfx_FillTriangle(
        -200 - player.xOffset,460 - player.yOffset,
        -165 - player.xOffset,460 - player.yOffset,
        -165 - player.xOffset,495 - player.yOffset
    );
    gfx_FillRectangle(-165 - player.xOffset,460 - player.yOffset,15,35);
}

void drawAdmin() {
    gfx_SetColor(1);
    //admin walls
    gfx_FillRectangle(220 - player.xOffset,490 - player.yOffset,290,35);
    gfx_SetColor(255);
    //gfx_FillRectangle(x,y,width,height)
    //admin hallways
    gfx_FillRectangle(185 - player.xOffset,440 - player.yOffset,50,180);
    gfx_FillRectangle(235 - player.xOffset,525 - player.yOffset,80,50);

    //room
    gfx_FillRectangle(315 - player.xOffset,525 - player.yOffset,160,140);
    gfx_FillRectangle(470 - player.xOffset,525 - player.yOffset,40,105);
    gfx_FillTriangle(
        475 - player.xOffset,630 - player.yOffset,
        510 - player.xOffset,630 - player.yOffset,
        475 - player.xOffset,665 - player.yOffset
    );
}

void drawComms() {
    //hallway walls
    gfx_SetColor(1);
    //hallway walls
    gfx_FillRectangle(275 - player.xOffset,690 - player.yOffset,245,35);
    gfx_FillRectangle(285 - player.xOffset,785 - player.yOffset,130,35);
    gfx_FillRectangle(455 - player.xOffset,785 - player.yOffset,40,35);
    gfx_SetColor(255);
    //map x value - 860
    //hallway
    gfx_FillRectangle(275 - player.xOffset,725 - player.yOffset,245,50);
    //hallway into room
    gfx_FillRectangle(410 - player.xOffset,760 - player.yOffset,60,60);
    //room
    gfx_FillRectangle(285 - player.xOffset,820 - player.yOffset,210,85);
    gfx_FillTriangle(
        285 - player.xOffset,905 - player.yOffset,
        335 - player.xOffset,905 - player.yOffset,
        335 - player.xOffset,940 - player.yOffset
    );
    gfx_FillTriangle(
        455 - player.xOffset,905 - player.yOffset,
        495 - player.xOffset,905 - player.yOffset,
        455 - player.xOffset,940 - player.yOffset
    );
    
    gfx_FillRectangle(335 - player.xOffset,905 - player.yOffset,120,35);
}

void drawStorage() {
    //top walls
    gfx_SetColor(1);
    gfx_FillRectangle(95 - player.xOffset,585 - player.yOffset,90,35);
    gfx_FillRectangle(220 - player.xOffset,585 - player.yOffset,55,35);

    gfx_FillTriangle(
        40 - player.xOffset,685 - player.yOffset,
        40 - player.xOffset,650 - player.yOffset,
        95 - player.xOffset, 620 - player.yOffset
    );
    gfx_FillTriangle(
        95 - player.xOffset, 620 - player.yOffset,
        95 - player.xOffset, 585 - player.yOffset,
        40 - player.xOffset, 650 - player.yOffset
    );
    gfx_SetColor(255);
    //large rightside
    gfx_FillRectangle(120 - player.xOffset, 620 - player.yOffset,155,320);
    //middle
    gfx_FillRectangle(95 - player.xOffset,620 - player.yOffset,25,250);
    gfx_FillRectangle(40 - player.xOffset,685 - player.yOffset,55,185);
    gfx_FillTriangle(
        95 - player.xOffset,620 - player.yOffset,
        95 - player.xOffset,685 - player.yOffset,
        40 - player.xOffset,685 - player.yOffset
    );
    gfx_FillTriangle(
        40 - player.xOffset,870 - player.yOffset,
        120 - player.xOffset,870 - player.yOffset,
        120 - player.xOffset,940 - player.yOffset
    );
    
}

void drawElectrical() {
    //hallway walls
    gfx_SetColor(1);
    gfx_FillRectangle(-345 - player.xOffset,675 - player.yOffset,130,35);
    gfx_FillRectangle(-215 - player.xOffset,785 - player.yOffset,50,35);
    gfx_FillRectangle(-120 - player.xOffset,785 - player.yOffset,160,35);
    gfx_SetColor(255);
    //hallway - bottom
    gfx_FillRectangle(-265 - player.xOffset,820 - player.yOffset,305,50);
    //hallway - left
    gfx_FillRectangle(-265 - player.xOffset,710 - player.yOffset,50,150);
    //hallway top left
    gfx_FillRectangle(-345 - player.xOffset,710 - player.yOffset,80,50);
    
    //bottom of room
    gfx_FillRectangle(-165 - player.xOffset,750 - player.yOffset,60,70);
    gfx_FillRectangle(-165 - player.xOffset,670 - player.yOffset,110,80);
    gfx_FillRectangle(-165 - player.xOffset,565 - player.yOffset,110,65);
    gfx_FillTriangle(
        -55 - player.xOffset,710 - player.yOffset,
        -15 - player.xOffset,710 - player.yOffset,
        -55 - player.xOffset,750 - player.yOffset
    );
    //top room
    gfx_FillRectangle(-55 - player.xOffset,565 - player.yOffset,40,145);
    gfx_FillRectangle(-15 - player.xOffset,565 - player.yOffset,50,20);
    gfx_FillTriangle(
        -15 - player.xOffset,585 - player.yOffset,
        -15 - player.xOffset,630 - player.yOffset,
        35 - player.xOffset,585 - player.yOffset
    );
    
    gfx_SetColor(1);
    //top wall
    gfx_FillRectangle(-165 - player.xOffset,530 - player.yOffset,200,35);
    //middle wall
    gfx_FillRectangle(-165 - player.xOffset, 630 - player.yOffset,110,40);
}

void drawUpperEngine() {
    gfx_SetColor(1);
    gfx_FillRectangle(-530 - player.xOffset,125 - player.yOffset,185,35);
    gfx_SetColor(255);
    //vent sprite top right
    gfx_FillRectangle(-530 - player.xOffset,160 - player.yOffset,185,170);
    //ENGINE SPRITE GOES HERE
}

void drawSecurity() {
    gfx_SetColor(1);
    gfx_FillRectangle(-530 - player.xOffset,430 - player.yOffset,80,35);
    gfx_FillRectangle(-405 - player.xOffset,430 - player.yOffset,75,35);

    //top of cams walls
    gfx_FillRectangle(-295 - player.xOffset,330 - player.yOffset,35,35);
    gfx_FillTriangle(
        -330 - player.xOffset,405 - player.yOffset,
        -330 - player.xOffset,370 - player.yOffset,
        -295 - player.xOffset,330 - player.yOffset
    
    );
    gfx_FillTriangle(
        -295 - player.xOffset,330 - player.yOffset,
        -295 - player.xOffset,365 - player.yOffset,
        -330 - player.xOffset, 405 - player.yOffset
    );

    //right
    gfx_FillTriangle(
        -260 - player.xOffset,330 - player.yOffset,
        -260 - player.xOffset, 365- player.yOffset,
        -230 - player.xOffset,405 - player.yOffset
    );
    gfx_FillTriangle(
        -260 - player.xOffset,330 - player.yOffset,
        -230 - player.xOffset, 370- player.yOffset,
        -230 - player.xOffset,405 - player.yOffset
    );

    gfx_SetColor(255);
    //long hallway from upper engine to
    gfx_FillRectangle(-450 - player.xOffset,330 - player.yOffset,50,310);
    //room
    gfx_FillRectangle(-330 - player.xOffset,405 - player.yOffset,100,130);
    //top of room
    gfx_FillTriangle(
        -330 - player.xOffset,405 - player.yOffset,
        -295 - player.xOffset,405 - player.yOffset,
        -295 - player.xOffset,365 - player.yOffset
    );
    gfx_FillTriangle(
        -260 - player.xOffset,405 - player.yOffset,
        -230 - player.xOffset,405 - player.yOffset,
        -260 - player.xOffset,365 - player.yOffset
    );
    gfx_FillRectangle(-295 - player.xOffset,365 - player.yOffset,35,40);
    //hallway connecting Reactor to Sec
    gfx_FillRectangle(-515 - player.xOffset,460 - player.yOffset,185,50);
}

void drawLowerEngine() {
    gfx_SetColor(1);
    gfx_FillRectangle(-510 - player.xOffset,605 - player.yOffset,60,35);
    gfx_FillRectangle(-400 - player.xOffset,605 - player.yOffset,55,35);
    gfx_SetColor(255);
    gfx_FillRectangle(-510 - player.xOffset,640 - player.yOffset,165,170);
}

void drawReactor() {
    gfx_SetColor(1);
    gfx_FillRectangle(-615 - player.xOffset,365 - player.yOffset,100,35);
    gfx_FillRectangle(-615 - player.xOffset,300 - player.yOffset,35,35);

    //side 
    gfx_FillTriangle(
        -660 - player.xOffset,375 - player.yOffset,
        -660 - player.xOffset,340 - player.yOffset,
        -615 - player.xOffset, 335 - player.yOffset
    );
    gfx_FillTriangle( 
        -615 - player.xOffset,300 - player.yOffset,
        -615 - player.xOffset, 335 - player.yOffset,
        -660 - player.xOffset, 340 - player.yOffset
    );
    gfx_SetColor(255);
    gfx_FillRectangle(-580 - player.xOffset,400-player.yOffset,65,160);
    gfx_FillRectangle(-615 - player.xOffset,335 - player.yOffset,35,285);
    gfx_FillTriangle(
        -660 - player.xOffset,590 - player.yOffset,
        -615 - player.xOffset,590 - player.yOffset,
        -615 - player.xOffset,620 - player.yOffset
    );
    gfx_FillRectangle(-660 - player.xOffset,375 - player.yOffset,45,215);
    gfx_FillTriangle(
        -660 - player.xOffset,375 - player.yOffset,
        -615 - player.xOffset,375 - player.yOffset,
        -615 - player.xOffset,335 - player.yOffset
    );
}

//probably replace with FLOOR sprites
void drawHallways() {
    gfx_SetColor(1);
    gfx_FillRectangle(580 - player.xOffset,465 - player.yOffset,155,35);
    //wall to o2
    gfx_FillRectangle(540 - player.xOffset,345 - player.yOffset,50,35);
    gfx_FillRectangle(590 - player.xOffset,345 - player.yOffset,145,35);
    //into nav
    gfx_FillRectangle(730 - player.xOffset,400 - player.yOffset,130,35);
    gfx_SetColor(255);
    //weapons down to shields
    gfx_FillRectangle(585 - player.xOffset,310 - player.yOffset,50,80);
    //to o2
    gfx_FillRectangle(540 - player.xOffset,380 - player.yOffset,195,45);
    gfx_FillRectangle(680 - player.xOffset,425 - player.yOffset,55,110);
    //to navigation
    gfx_FillRectangle(735 - player.xOffset,435 - player.yOffset,100,55);

    gfx_FillRectangle(580 - player.xOffset,500 - player.yOffset,155,45);
    //down to shields
    gfx_FillRectangle(580 - player.xOffset,535 - player.yOffset,60,115);
}

void createVents() {
    uint8_t idx;
    for (idx = 0; idx < 14; idx++) {
        vent_t* vent = (&vents[idx]);
        
        if (idx == 0) {
            vent->room = 0;
            vent->xpos = -375;
            vent->ypos = 165;
        } else if (idx == 1) {
            vent->room = 1;
            vent->xpos = 395;
            vent->ypos = 260;
        } else if (idx == 2) {
            vent->room = 2;
            vent->xpos = 575;
            vent->ypos = 135;
        } else if (idx == 3) {
            //reactor
            vent->room = 3;
            vent->xpos = -635;
            vent->ypos = 380;
        } else if (idx == 4) {
            //reactor bottom
            vent->room = 3;
            vent->xpos = -580;
            vent->ypos = 530;
        } else if (idx == 5) {
            //sec
            vent->room = 4;
            vent->xpos = -265;
            vent->ypos = 515;
        } else if (idx == 6) {
            //medbay
            vent->room = 5;
            vent->xpos = -190;
            vent->ypos = 425;
        } else if (idx == 7) {
            //nav top
            vent->room = 7;
            vent->xpos = 855;
            vent->ypos = 385;
        } else if (idx == 8) {
            //nav bottom 
            vent->room = 7;
            vent->xpos = 855;
            vent->ypos = 500;
        } else if (idx == 9) {
            //lower engine
            vent->room = 8;
            vent->xpos = -370;
            vent->ypos = 790;
        } else if (idx == 10) {
            //electrical
            vent->room = 9;
            vent->xpos = -155;
            vent->ypos = 570;
        } else if (idx == 11) {
            //admin
            vent->room = 11;
            vent->xpos = 325;
            vent->ypos = 640;
        } else if (idx == 12) {
            //shields
            vent->room = 12;
            vent->xpos = 600;
            vent->ypos = 800;
        } else if (idx == 13) {
            //shields hallway
            //vent->room = ??
            //mapgroup = ?
            vent->xpos = 595;
            vent->ypos = 510;
        }
    }
}

void drawVents() {
    //drawn all at the same time - not big speed problem
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(1);
    //upper engine
    gfx_TransparentSprite(vent_sprite,-375 - player.xOffset, 165 - player.yOffset);
    //sec
    gfx_TransparentSprite(vent_sprite,-265 - player.xOffset, 515 - player.yOffset);
    //reactor
    gfx_TransparentSprite(vent_sprite,-635 - player.xOffset,380  - player.yOffset);
    gfx_TransparentSprite(vent_sprite,-580 - player.xOffset,530 - player.yOffset);
    //lower engine
    gfx_TransparentSprite(vent_sprite,-370 - player.xOffset,790 - player.yOffset);
    //electrical
    gfx_TransparentSprite(vent_sprite,-155 - player.xOffset,570 - player.yOffset);
    //admin
    gfx_TransparentSprite(vent_sprite,325 - player.xOffset,640 - player.yOffset);
    //shields
    gfx_TransparentSprite(vent_sprite,600 - player.xOffset,800 - player.yOffset);
    //shields hallway
    gfx_TransparentSprite(vent_sprite,595 - player.xOffset,510 - player.yOffset);
    //nav
    gfx_TransparentSprite(vent_sprite,855 - player.xOffset,385 - player.yOffset );
    gfx_TransparentSprite(vent_sprite,855 - player.xOffset,500 - player.yOffset);
    //weapons
    gfx_TransparentSprite(vent_sprite,575 - player.xOffset,135 - player.yOffset);
    //cafeteria
    gfx_TransparentSprite(vent_sprite,395 - player.xOffset,260 - player.yOffset);
    //medbay
    gfx_TransparentSprite(vent_sprite,-190 - player.xOffset,425 - player.yOffset);
}

void drawMap() {
    //temp - right side hallways only for now
    //possibly move all hallways to one function
    drawHallways();
    if (player.mapGroup == 0) {
        drawElectrical();
        drawUpperEngine();
        drawSecurity();
        drawLowerEngine();
        drawReactor();
        drawMedBay();
        if (player.xpos > -175 && player.ypos < 560) {
            drawCafeteria();
        } 
        if (player.xpos > -120 && player.ypos > 445) {
            drawStorage();
        }
    } else if (player.mapGroup == 1) {
        drawStorage();
        drawAdmin();
        drawComms();
        if (player.ypos < 560) {
            drawCafeteria();
        } 
        if (player.xpos < 200) {
            drawElectrical();
        } else if (player.xpos > 335) {
            drawShields();
        }
    } else if (player.mapGroup == 2) {
        drawWeapons();
        drawO2();
        drawNav();
        drawShields();
        if (player.xpos < 680 && player.ypos > 510) {
            drawComms();
        } else if (player.xpos < 610) {
            drawCafeteria();
        }
        if (player.xpos < 660 && player.ypos > 405) {
            drawAdmin();
        }
    } else if (player.mapGroup == 3) {
        drawCafeteria();
        if (player.ypos > 320) {
            drawStorage();
            drawAdmin();
        } 
        if (player.xpos > 425) {
            drawWeapons();
            drawO2();
        } else if (player.xpos < 160) {
            drawMedBay();
        } else if (player.xpos > 265) {
            drawWeapons();
        }
    }
    drawVents();
}

void createDummies() {
    uint8_t idx;
    for (idx = 0; idx < 9; idx++) {
        dummy_t* dummy = (&dummies[idx]);
        if (idx == 0) {
            dummy->xpos = -200;
            dummy->ypos = 405;
        } else if (idx == 1) {
            dummy->xpos = -485;
            dummy->ypos = 165;
        } else if (idx == 2) {
            dummy->xpos = 890;
            dummy->ypos = 480;
        } else if (idx == 3) {
            dummy->xpos = 380;
            dummy->ypos = 165;
        } else if (idx == 4) {
            dummy->xpos = -320;
            dummy->ypos = 420;
        } else if (idx == 5) {
            //electrical
            dummy->xpos = -160;
            dummy->ypos = 565;
        } else if (idx == 6) {
            dummy->xpos = -600;
            dummy->ypos = 435;
        } else if (idx == 6) {
            dummy->xpos = -630;
            dummy->ypos = 510;
        } else if (idx == 7) {
            dummy->xpos = 130;
            dummy->ypos = 380;
        } else if (idx == 8) {
            dummy->xpos = 330;
            dummy->ypos = 600;
        }
    }
}

void drawDummies() {
    uint8_t idx;
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(1);
    for (idx = 0; idx < 9; idx++) {
        
        dummy_t dummy = dummies[idx];
        if (dummy.dead == false) {
        gfx_TransparentSprite(red_still_right,dummy.xpos - player.xOffset,dummy.ypos - player.yOffset);
        } else {
            //conditional for standing or fallen dead body - countdown timer?
            gfx_TransparentSprite(dead_body_fallen,dummy.xpos - player.xOffset,dummy.ypos - player.yOffset);
        }
    }
    //future dummy movement
    //gfx_TransparentSprite(red_running_right,0,0);
    //gfx_TransparentSprite(red_running_left,25,0);
    //gfx_TransparentSprite(red_still_left,50,0);
}

void findDummy() {
    uint8_t idx;
    int px,py,dx,dy,distance;
    //maybe store distance from player, then compare in for loop
    //ex. dummy.distance = x; for (i < 9...) if dummy.distance < dummy1.distance ...
    //idx - number of players killed?
        //does not work because it dummies[idx]
    for (idx = 0; idx < 9; idx++) {
        dummy_t dummy = dummies[idx];
        if (dummy.dead == false) {
            px = player.xpos;
            py = player.ypos;
            dx = dummy.xpos;
            dy = dummy.ypos;
            dummy.distance = sqrt(pow((px - dx),2) + pow((py - dy),2));
            if (dummy.distance < player.dummyDistance) {
                player.dummyWho = idx;
                player.dummyDistance = dummy.distance;
            } 
        }
    }   
}

void trackDummy() {
    int px,py,dx,dy;
    uint8_t idx;
    dummy_t* dummy = &(dummies[player.dummyWho]);
    px = player.xpos;
    py = player.ypos;
    dx = dummy->xpos;
    dy = dummy->ypos;
    dummy->distance = sqrt(pow((px - dx),2) + pow((py - dy),2));
    player.dummyDistance = dummy->distance;
    
    //calling finddummy every loop is slow
    findDummy();
}

void doButtons() {
    uint8_t idx;
    uint8_t ventidx;
    
    //impostor kills
    
    if (kb_Data[6] & kb_Sub) {
        if (player.impostor == true) {
            //only check dummies that are alive
            //maybe just find if player is under 50 difference
            
            dummy_t* dummy = &(dummies[player.dummyWho]);
            if (dummy->distance <= 50 && dummy->dead == false) {
                player.xOffset = dummy->xpos - 160;
                player.yOffset = dummy->ypos - 120;
                //overwrite dummywho
                dummy->dead = true;
                player.killed++;
            }
    }    
    } else if (kb_Data[6] & kb_Div) {
        if (player.impostor == true) {
            /*
            rooms - left to right
            upper engine, cafe, weapons
            reactor, cams, medbay, o2, nav
            lower, electrical, storage, admin, shields
            comms
            */
            if (player.xpos > 580 && player.xpos < 730 && player.ypos > 440 && player.ypos < 550) {
                //else condition, right side check
                ventidx = 13;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 0) {
                ventidx = 0;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 1) {
                ventidx = 1;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 2)  {
                ventidx = 2;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 3)  {
                ventidx = 3;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            }else if (player.room == 3)  {
                ventidx = 4;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            }  else if (player.room == 4) {
                ventidx = 5;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 5) {
                ventidx = 6;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 7) {
                ventidx = 7;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 8) {
                ventidx = 8;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 11) {
                ventidx = 9;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 9) {
                ventidx = 10;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            } else if (player.room == 12) {
                ventidx = 12;
                player.xOffset = vents[ventidx].xpos - 160;
                player.yOffset = vents[ventidx].ypos - 120 - 10;
            }
            if (player.vented == false) {
                player.vented = true;
            } else {
                player.vented = false;
            }
            delay(50);
        }
        
    }
}

void drawPlayer() {
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(1);
    //draw white circle underneath player?
    if (player.vented == false) {
        if (player.dir == 0 && player.running == 0) {
        gfx_TransparentSprite(still_left, player.startX, player.startY);
        } else if (player.dir == 1 && player.running == 0) {
            gfx_TransparentSprite(still_right, player.startX, player.startY);
        } else if (player.dir == 0 && player.running == 1) {
            if (player.footdown == 1) {
                gfx_TransparentSprite(still_left, player.startX, player.startY);
                player.footdown = 0;
            } else {
                player.footdown = 1;
                gfx_TransparentSprite(running_left, player.startX, player.startY);
            }
        } else if (player.dir == 1 && player.running == 1) {
            if (player.footdown == 1) {
                gfx_TransparentSprite(still_right, player.startX, player.startY);
                player.footdown = 0;
            } else {
                gfx_TransparentSprite(running_right, player.startX, player.startY);
                player.footdown = 1;
            }
        }
    }
    
}
 
void handleMovement() {
    if (kb_Data[7] & kb_Left) {
        //if not collide with wall
        if (player.vented == false) {
            player.xOffset -= player.speed;
        }
        
        player.dir = 0;
        player.running = 1;
    } else if (kb_Data[7] & kb_Right) {
        if (player.vented == false) {
            player.xOffset += player.speed;
        }
        player.dir = 1;
        player.running = 1;
    } else if (kb_Data[7] & kb_Up) {
        if (player.vented == false) {
            player.yOffset -= player.speed;
        }
        player.running = 1;
    } else if (kb_Data[7]& kb_Down) {
        if (player.vented == false) {
            player.yOffset += player.speed;
        }
        player.running = 1;
    } else {
        player.running = 0;
    }
    player.xpos = player.xOffset + 160;
    player.ypos = player.yOffset + 120;

    if (kb_Data[1] & kb_Mode) {
        if (player.debug == false) {
            player.debug = true;
        } else {
            player.debug = false;
        }
    }
}

void drawNames() {
    uint8_t idx;
    gfx_SetTextFGColor(2);
    if (player.vented == false) {
        gfx_PrintStringXY("YOU",(170 - gfx_GetStringWidth("YOU") / 2),110);
    }
    
    //potential dummy names
    /*for (idx = 0; idx < 9; idx++) {
        dummy_t dummy = dummies[idx];
        YelText();
        //gfx_PrintStringXY("D ",((dummy.xpos - 10) - gfx_GetStringWidth("D  ") / 2) - player.xOffset,(dummy.ypos - 10) - player.yOffset);
        gfx_PrintStringXY("D ",((dummy.xpos - 10) - gfx_GetStringWidth("D  ") / 2) - player.xOffset,(dummy.ypos - 10) - player.yOffset);
        gfx_PrintInt(idx + 1,1);
    }*/
}

void main() {
    LoadData();
    player.startX = 160;
    player.startY = 120;
    
    //speed is fluctuating because map isn't being generated at once && finddummy function is called each loop
    player.speed = 12;
    player.debug = false;
    player.impostor = true;
    player.killed = 0;
    createDummies();
    createVents();
    gfx_Begin();
    //start screen needed?
    do {
        kb_Scan();
        gfx_SetDrawBuffer();
        gfx_ZeroScreen();
        
        drawMap();
        drawDummies();
        if (player.debug == true) {
            dispStats();
        }
        drawButtons();
        doButtons();
        //doTasks();
        drawNames();
        handleMovement();
        //doCollisions();
        locationTracker();
        if (player.impostor == true) {
            trackDummy();
        }
        drawPlayer();
        gfx_SwapDraw();
    } while (kb_Data[6] != kb_Clear);
    SaveData();
    gfx_End();
}
