#include <tice.h>


void SaveData(void) {
    ti_var_t slota;
    ti_CloseAll();
    if (slota = ti_Open("AMGDAT","w+")) {
        ti_Write(&player,sizeof(player),1,slota);
        //ti_Write(&game,sizeof(game),1,slota);
        //ti_Write(&dummies,sizeof(dummies),1,slota);
    }
    ti_SetArchiveStatus(true, slota);
}

void LoadData(void) {
    ti_var_t slota;
    ti_CloseAll();
    if (slota = ti_Open("AMGDAT", "r")) {
        ti_Read(&player, sizeof(player),1,slota);
        //ti_Read(&game, sizeof(game),1,slota);
        //ti_Read(&dummies, sizeof(dummies),1,slota);
    }
}

void CreateSave(void) {
    ti_var_t slota;
    ti_CloseAll();
    if (slota = ti_Open("AMGDAT","w+")) {
        ti_Write(&player, sizeof(player), 1, slota);
        ti_SetArchiveStatus(true, slota);
        //ti_Write(&dummies, sizeof(dummies),1,slota);
        //ti_Write(&game, sizeof(game), 1, slota);
    }
}
