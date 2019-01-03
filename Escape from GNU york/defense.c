//file: defense.c
//Description: implementation of the defense.h header file
//@author Jacob Manelius

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <curses.h>
#include "defense.h"

//create the defender object
Defender* createDefender(int height, char* name, int startingX, int maxX)
{
    Defender* output = (Defender*)malloc(sizeof(Defender));
    output->height = height;
    output->name = name;
    output->xCord = startingX;
    output->maxX = maxX;
    return output;
}

//draw the defender shield, starting from the left
void drawShield(int height, int xCord)
{
    mvprintw(height, xCord - 1, " ");
    refresh();
    mvprintw(height, xCord + 6, " ");
    refresh();
    for(int i = 0; i < 5; i++) //5 is the length of the shield
    {
        mvprintw(height, xCord + i, "#");
        refresh();
    }
}

//run the defender thread
void* runDefense(void* defender)
{
    //draw the shield at its starting point
    drawShield(((Defender*)defender)->height, ((Defender*)defender)->xCord);

    //continue to get left and right inputs to move the shield
    int input;
    while(1)
    {
        input = getch();
        switch(input)
        {
            //move the shield left
            case KEY_LEFT:
                if(((Defender*)defender)->xCord - 1 >= 0)
                    drawShield(((Defender*)defender)->height, --((Defender*)defender)->xCord);
                break;
            //move the shield right
            case KEY_RIGHT:
                if(((Defender*)defender)->xCord + 1 <=  ((Defender*)defender)->maxX - 5)
                    drawShield(((Defender*)defender)->height, ++((Defender*)defender)->xCord);
                break;
            //terminate the program
            case 'q':
                pthread_exit(0);
                break;
            //other input is ignored
            default:
                continue;
        }
    }
}
