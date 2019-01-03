//file: cityGenerator.c
//Description: implementation of the cityGenerator.h file
//@author Jacob Manelius

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "cityGenerator.h"

///draws the wall of a building on the curses standard screen
void generateWall(int height, int screenY, int xCord)
{
    for(int i = 2; i < height; i++)
    {
        mvprintw(screenY - i, xCord, "|");
        refresh();
    }
}

///draws the city on the curses standard screen
void generateCity(int* blueprint, int screenX, int screenY, int citySize)
{
    for(int i = 0; i < citySize; i++)
    {
        //exit if the city was larger than the screen
        if(i > screenX)
            break;

        int currVal = blueprint[i];
        if(currVal <= 2) //print ground terrain
        {
            mvprintw(screenY - 2, i, "_");
            refresh();
        }
        else //print buildings
        {
            //handle a corner case where buildings are not complete
            if(i + 1 > citySize)
            {
                generateWall(currVal, screenY, i + 1);
            }
            //build a normal wall
            else if(blueprint[i - 1] < currVal || blueprint[i + 1] < currVal)
            {
                generateWall(currVal, screenY, i);
            }
            //build a roof
            else
            {
                mvprintw(screenY - currVal, i, "_");
                refresh();
            }
        }
    }
    //pad out the rest of the screen if the city didn't fill it
    if(citySize < screenX)
    {
        for(int i = citySize; i < screenX; i++)
        {
            mvprintw(screenY - 2, i, "_");
            refresh();
        }
    }
}
