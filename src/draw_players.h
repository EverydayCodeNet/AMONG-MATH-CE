#include "graphics.h"


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

void drawPlayerNames() {
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
