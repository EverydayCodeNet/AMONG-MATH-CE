#include <stdint.h> // for uint8_t

// Universal (map-indifferent) vent data


typedef struct {
    int xpos,ypos;
    uint8_t room;
    //connecting vents
    uint8_t connecting;
} vent_t;
