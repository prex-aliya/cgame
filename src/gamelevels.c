/** \file gamelevels.h
 ** \brief Stores game spacific data
 **
 ** This file stores code that is spacific to the game, like map data, and
 ** cutscenes and other spa*/

#include "config.h"

//#include "main.c"
//#include "audio.h" // TODO: add multitherading to <this file

void level2() {}

void level1() {
    int map[19][19]={
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,3,3,3,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0},
    {0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0},
    {0,0,0,0,0,4,0,0,0,5,0,0,0,4,0,0,0,0,0},
    {0,0,0,0,0,4,0,0,5,5,5,0,0,4,0,0,0,0,0},
    {0,0,0,0,0,4,0,0,5,3,5,0,0,4,0,0,0,0,0},
    {0,0,0,0,0,4,0,5,5,5,5,5,0,4,0,0,0,0,0},
    {0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0},
    {0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0},
    {0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1}
};

    mapx = 19;
    mapy = 19;

    if (playerx == 9 && playery == 12) {
        BEEPL(2);
        playermove = false;
        playerview = false;
        level += 1;
        level2();
        return;
    }

    if (playermove == true) {
        render(map);
    } else {
        map[4][6] = 1;
        map[4][7] = 1;
        map[4][8] = 1;

        fputs("\033c", stdout);
        level = 1;
        render(map);
        for (register int i=-1; i!=7; i++) {
            map[4][i-1] = 1;
            map[4][i]   = 3;
            map[4][i+1] = 3;
            map[4][i+2] = 3;

            if (i>=5) {
                usleep(700000);
            } else if (i>=4) {
                usleep(500000);
            } else {
                usleep(400000); /* sleep in microseconds */
            }

            fputs("\033c", stdout);
            render(map);
        }

        sleep(1);
        mapx=16;
        mapy=16;
        playerx=7;
        playery=6;
        playermove=true;
        playerview=true;
    }
}

//void level0_walk_cutscene() {} // TODO: cutscene for going down the road
void level0_car_cutscene(int map[10][20]) {
    map[2][7]  = 0;
    map[2][10] = 3;

    fputs("\033c", stdout);
    render(map);

    sleep(1);
    map[2][8]  = 0;
    map[2][9]  = 0;
    map[2][10] = 0;

    for (int i=9; i<17; i++) {
        map[3][i-1] = 1;
        map[3][i]   = 3;
        map[3][i+1] = 3;
        map[3][i+2] = 3;

        usleep(2000); /* sleep in microseconds */
        fputs("\033c", stdout);
        render(map);

        usleep(500000);
    }

    level1();
}
void level0() {
    int map[10][20]={
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    if (playerx == 8 && playery == 3) {
        BEEPL(2);
        playermove = playerview = false;
        level0_car_cutscene(map);
    } else if (playerx >= 1000) {
        /* If follow road long enough, start next level */
        BEEPL(2);
        playermove = playerview = false;
        playerx = 8; playery = 3;
        level1();
    } else {
        render(map);
    }
}

void runlevel() {
    fputs("\033c", stdout);
    /* Run the Level */
    /* TODO must be constant speed solution? */
    switch (level) {
        case 0: level0(); break;
        case 1: level1(); break;
        case 2: level2(); break;
        default: exit_game();
    }
    //if (level == 0) level0();
    //else if (level == 1) level1();
    //else if (level == 2) level2();
    //else exit_game();
}
