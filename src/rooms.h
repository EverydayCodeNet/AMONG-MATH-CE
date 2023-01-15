#include <stdint.h> // for uint8_t


enum Rooms {
    NoSuchRoom,
    UpperEngineRoomID,
    CafeteriaRoomID,
    WeaponsRoomID,
    ReactorRoomID,
    SecurityRoomID,
    MedbayRoomID,
    O2RoomID,
    NavigationRoomID,
    LowerEngineRoomID,
    ElectricalRoomID,
    StorageRoomID,
    AdminRoomID,
    ShieldsRoomID,
    CommunicationsRoomID,
    n_rooms
};

const char* room_names[] = {
    "NO ROOM",
    "UPPER ENGINE",
    "CAFETERIA",
    "WEAPONS",
    "REACTOR",
    "SECURITY",
    "MEDBAY",
    "O2",
    "NAVIGATION",
    "LOWER ENGINE",
    "ELECTRICAL",
    "STORAGE",
    "ADMIN",
    "SHIELDS",
    "COMMUNICATIONS"
};

uint8_t get_room_given_position(const uint8_t map_group_id,  const int xpos,  const int ypos){
    if (map_group_id == 0) {
        if (xpos > -540 && xpos < -340 && ypos > 125 && ypos < 325) {
            return UpperEngineRoomID;
        } else if (xpos > -695 && xpos < -510 && ypos > 300 && ypos < 625) {
            return ReactorRoomID;
        } else if (xpos > -340 && xpos < -220 && ypos > 340 && ypos < 560) {
            return SecurityRoomID;
        } else if (xpos > -540 && xpos < -340 && ypos > 605 && ypos < 820) {
            return LowerEngineRoomID;
        } else if (xpos > -205 && xpos < -10 && ypos > 275 && ypos < 500) {
            return MedbayRoomID;
        } else if (xpos > -170 && xpos < 40 && ypos > 530 && ypos < 800) {
            return ElectricalRoomID;
        }
    } else if (map_group_id == 1) {
        if (xpos > 40 && xpos < 280 && ypos > 590 && ypos < 945) {
            return StorageRoomID;
        } else if (xpos > 250 && xpos < 510 && ypos > 495 && ypos < 665) {
            return AdminRoomID;
        } else if (xpos > 285 && xpos < 495 && ypos > 795) {
            return CommunicationsRoomID;
        } 
    } else if (map_group_id == 2) {
        if (xpos > 510 && ypos > 670) {
            return ShieldsRoomID;
        } else if (xpos > 850 && ypos > 345) {
            return NavigationRoomID;
        } else if (xpos > 400 && xpos < 550 && ypos > 380 && ypos < 470) {
            return O2RoomID;
        } else if (xpos > 515 && xpos < 680 && ypos > 125 && ypos < 310) {
            return WeaponsRoomID;
        }
    } else if (map_group_id == 3) {
        if (xpos > 0 && xpos < 430 && ypos > 0 && ypos < 440) {
            return CafeteriaRoomID;
        }
    }
}
