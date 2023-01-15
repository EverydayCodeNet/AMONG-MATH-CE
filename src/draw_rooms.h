#include "graphics.h"


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
