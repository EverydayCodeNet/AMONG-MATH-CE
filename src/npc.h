#include <stdint.h>

// Universal (map-independent) NPC/dummy data


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
