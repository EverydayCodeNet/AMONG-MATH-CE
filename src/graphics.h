#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

/* Include the converted graphics file */
#include "gfx/gfx.h"

// This file is for general (map-independent) graphics manipulation


static
kb_key_t key;


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

void get_str_width(const char* const str){
    return gfx_GetStringWidth(str);
}

void print_text_at(const char* const str,  const int x,  const int y){
    gfx_PrintStringXY(str, x, y);
}

void init_screen(){
    gfx_Begin();
    //start screen needed?
}

void end_screen(){
    gfx_End();
}

void init_screen_frame(){
    gfx_SetDrawBuffer();
    gfx_ZeroScreen();
}

void end_screen_frame(){
    gfx_SwapDraw();
}
