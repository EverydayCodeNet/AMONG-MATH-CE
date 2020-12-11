#include "graphics.h"
#include "button_graphics.h"
#include "draw_rooms.h"
#include "draw_players.h"
#include "draw_debug.h"
#include "key_handler.h"
#include "filesystem.h"

#include "rooms.h"
#include "vents.h"
#include "npcs.h"

#include <math.h>


typedef struct {
    //disable keyboard scan
    bool canPress;
    //used for start menu - if no save, new game.
    bool save;
    bool is_debug;
} game_t;
game_t game;

typedef struct {
    bool impostor;
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

const char *appvarName = "slota";

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
    player.room = get_room_given_position(player.mapGroup, player.xpos, player.ypos);
    const char* const room_name = room_names[player.room];
    print_text_at(room_name,  160 - get_str_width(room_name),  220);
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
    if (is_kill_key_combo_pressed()) {
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
    } else if (is_vent_key_combo_pressed()) {
        if (player.impostor == true) {
            /*
            rooms - left to right
            upper engine, cafe, weapons
            reactor, cams, medbay, o2, nav
            lower, electrical, storage, admin, shields
            comms
            */
            const uint8_t ventidx = determine_vent_id(player.room, player.xpos, player.ypos);
            player.xOffset = vents[ventidx].xpos - 160;
            player.yOffset = vents[ventidx].ypos - 120 - 10;
            player.vented = !player.vented;
            delay(50);
        }
    }
}
 
void handleMovement() {
    switch(interpret_key_press()){
        case RunLeft:
            //if not collide with wall
            if (player.vented == false) {
                player.xOffset -= player.speed;
            }
            player.dir = 0;
            player.running = 1;
            break;
        case RunRight:
            if (player.vented == false) {
                player.xOffset += player.speed;
            }
            player.dir = 1;
            player.running = 1;
            break;
        case RunUp:
            if (player.vented == false) {
                player.yOffset -= player.speed;
            }
            player.running = 1;
            break;
        case RunDown:
            if (player.vented == false) {
                player.yOffset += player.speed;
            }
            player.running = 1;
            break;
        case ToggleDebugMode:
            game.is_debug = !game.is_debug;
        default:
            player.running = 0;
    }
    player.xpos = player.xOffset + 160;
    player.ypos = player.yOffset + 120;
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
    init_screen();
    do {
        key_scan();
        init_screen_frame();
        
        drawMap();
        drawDummies();
        if (game.is_debug) {
            dispStats();
        }
        drawButtons();
        doButtons();
        //doTasks();
        drawPlayerNames();
        handleMovement();
        //doCollisions();
        locationTracker();
        if (player.impostor == true) {
            trackDummy();
        }
        drawPlayer();
        end_screen_frame();
    } while (is_key_down());
    SaveData();
    end_screen();
}
