#include "graphics.h"


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
    
    //check if vent is in room or hallway, otherwise display vent_cooldown
    if (determine_vent_id(player.room, 0, 0)) {
        gfx_TransparentSprite(vent_button,255,180);
    } else {
        gfx_TransparentSprite(vent_button_cooldown,255,180);
    }
}
