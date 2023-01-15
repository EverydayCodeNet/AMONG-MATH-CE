#include "graphics.h"


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
