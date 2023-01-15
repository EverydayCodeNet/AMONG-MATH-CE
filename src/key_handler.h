#include <keypadc.h>


static
kb_key_t key;


enum KeyPressInterpretation {
    NoKeyPressInterpretation,
    RunLeft,
    RunRight,
    RunUp,
    RunDown,
    ToggleDebugMode
};

void key_scan(){
    kb_Scan();
}

bool is_key_down(){
    return (kb_Data[6] != kb_Clear);
}

bool is_kill_key_combo_pressed(){
    return (kb_Data[6] & kb_Sub);
}

bool is_vent_key_combo_pressed(){
    return (kb_Data[6] & kb_Div);
}

KeyPressInterpretation interpret_key_press(){
    if (kb_Data[7] & kb_Left) {
        return RunLeft;
    } else if (kb_Data[7] & kb_Right) {
        return RunRight;
    } else if (kb_Data[7] & kb_Up) {
        return RunUp;
    } else if (kb_Data[7]& kb_Down) {
        return RunDown;
    } else if (kb_Data[1] & kb_Mode) {
        return ToggleDebugMode;
    }
    return NoKeyPressInterpretation;
}
