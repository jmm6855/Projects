//file: missile.c
//Description: implementation of the missile.h header file
//@author Jacob Manelius

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <curses.h>
#include <unistd.h>
#include "missile.h"

//make a new missile
Missile* createMissile(int xCord, int height, int screenY)
{
    Missile* output = (Missile*)malloc(sizeof(Missile));
    output->xCord = xCord;
    output->height = height;
    output->screenY = screenY;
    output->speed = (rand() % (1000000 - 200000 + 1)) + 200000;
    return output;
}

//draw an impact at the given position
void drawImpact(int xCord, int yCord)
{
    mvprintw(yCord, xCord, "*");
    refresh();
    mvprintw(yCord - 1, xCord, "?");
    refresh();
}

//advance the missile
void advanceMissile(void* missile)
{
    //update the new position
    ((Missile*)missile)->height++;

    //clear the old position of the missile
    mvprintw(((Missile*)missile)->height - 1, ((Missile*)missile)->xCord, " ");
    refresh();
    mvprintw(((Missile*)missile)->height - 2, ((Missile*)missile)->xCord, " ");
    refresh();
    //draw the new position
    mvprintw(((Missile*)missile)->height, ((Missile*)missile)->xCord, "|");
    refresh();

    //wait until the next update time
    usleep(((Missile*)missile)->speed);
}

//run the missile thread
void* runMissile(void* missile)
{
    advanceMissile(missile);
    while(((Missile*)missile)->height < ((Missile*)missile)->screenY - 3)
    {
        switch(mvinch(((Missile*)missile)->height + 1, ((Missile*)missile)->xCord))
        {
            //ground, roof, wall, or shield
            case '_':
            case '|':
            case '#':
                drawImpact(((Missile*)missile)->xCord, ((Missile*)missile)->height + 1);
                free(missile);
                pthread_exit(0);
                break;
            //other (smoke or crater)
            default:
                advanceMissile(missile);
                break;
        }
    }
    drawImpact(((Missile*)missile)->xCord, ((Missile*)missile)->height + 2);
    free(missile);
    pthread_exit(0);
}
