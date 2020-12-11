#include "vents.h"
#include <stdio.h>

int main(){
    const unsigned vent_id = determine_vent_id(5, 0, 0);
    const unsigned room_id = vents[vent_id].room;
    printf("Vent #%u in %s\n", vent_id, room_names[room_id]);
    return 0;
}
