#include "vent.h"
#include "rooms.h"

// Room-specific vent information


enum Vents {
    NoVent,
    UpperEngineRoomVent,
    CafeteriaRoomVent,
    WeaponsRoomVent,
    ReactorRoomVentA,
    ReactorRoomVentB,
    SecurityRoomVent,
    MedbayRoomVent,
    NavigationRoomVentA,
    NavigationRoomVentB,
    LowerEngineRoomVent,
    ElectricalRoomVent,
    AdminRoomVent,
    ShieldsRoomVent,
    CommunicationsRoomVent,
    ShieldsHallwayVent,
    n_vents
};

static
const vent_t vents[] = {
    [UpperEngineRoomVent] = {
        .room = UpperEngineRoomID,
        .xpos = -375,
        .ypos = 165
    },
    [CafeteriaRoomVent] = {
        .room = CafeteriaRoomID,
        .xpos = 395,
        .ypos = 260
    },
    [WeaponsRoomVent] = {
        .room = WeaponsRoomID,
        .xpos = 575,
        .ypos = 260
    },
    [ReactorRoomVentA] = {
        .room = ReactorRoomID,
        .xpos = -635,
        .ypos = 380
    },
    [ReactorRoomVentB] = {
        .room = ReactorRoomID,
        .xpos = -580,
        .ypos = 530
    },
    [SecurityRoomVent] = {
        .room = SecurityRoomID,
        .xpos = -265,
        .ypos = 515
    },
    [MedbayRoomVent] = {
        .room = MedbayRoomID,
        .xpos = -190,
        .ypos = 425
    },
    [NavigationRoomVentA] = {
        .room = NavigationRoomID,
        .xpos = 855,
        .ypos = 385
    },
    [NavigationRoomVentB] = {
        .room = NavigationRoomID,
        .xpos = 855,
        .ypos = 500
    },
    [LowerEngineRoomVent] = {
        .room = LowerEngineRoomID,
        .xpos = -370,
        .ypos = 790
    },
    [ElectricalRoomVent] = {
        .room = ElectricalRoomID,
        .xpos = -155,
        .ypos = 570
    },
    [AdminRoomVent] = {
        .room = AdminRoomID,
        .xpos = 325,
        .ypos = 570
    },
    [ShieldsRoomVent] = {
        .room = ShieldsRoomID,
        .xpos = 600,
        .ypos = 800
    },
    [ShieldsHallwayVent] = {
        .room = NoSuchRoom,
        .xpos = 595,
        .ypos = 510
    }
};

uint8_t determine_vent_id(const uint8_t room_id,  const int xpos,  const int ypos){
    switch(room_id){
        case UpperEngineRoomID:
            return UpperEngineRoomVent;
        case CafeteriaRoomID:
            return CafeteriaRoomVent;
        case WeaponsRoomID:
            return WeaponsRoomVent;
        case ReactorRoomID:
            // TODO: There are two vents here in the official game
            return ReactorRoomVentA;
        case SecurityRoomID:
            return SecurityRoomVent;
        case MedbayRoomID:
            return MedbayRoomVent;
        case NavigationRoomID:
            // TODO: There are two vents here in the official game
            return NavigationRoomVentA;
        case LowerEngineRoomID:
            return LowerEngineRoomVent;
        case ElectricalRoomID:
            return ElectricalRoomVent;
        case AdminRoomID:
            return AdminRoomVent;
        case ShieldsRoomID:
            return ShieldsRoomVent;
        case CommunicationsRoomID:
            return CommunicationsRoomVent;
        default:
            if (xpos > 580 && xpos < 730 && ypos > 440 && ypos < 550) {
                //else condition, right side check
                return ShieldsHallwayVent;
            }
            return NoVent;
    }
}
