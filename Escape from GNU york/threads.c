//@author Jacob Manelius
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <curses.h>
#include <unistd.h>
#include "fileReader.h"
#include "cityGenerator.h"
#include "defense.h"
#include "offense.h"

int main(int argc, char* argv[])
{
    //if two args then print error and exit
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./threads config-file\n");
        return EXIT_FAILURE;
    }
    //if the file doesn't exist, exit
    if(!fileExists(argv[1]))
        return EXIT_FAILURE;

    //if the file fails to be read, exit
    bool fileSuccess = readFile(argv[1]);
    if(!fileSuccess)
        return EXIT_FAILURE;

    //retrieve the information from the file reader and clean up memory
    char* defenseName = (char*)malloc(sizeof(char) * strlen(getDefenseName()) + 1);
    strcpy(defenseName, getDefenseName());

    char* offenseName = (char*)malloc(sizeof(char) * strlen(getOffenseName()) + 1);
    strcpy(offenseName, getOffenseName());

    size_t maxMissiles = getMaxMissiles();

    int citySize = getCitySize() - 1;
    int* cityPlan = (int*)malloc(sizeof(int) * citySize);
    //copy the city plan
    int* blueprint = getCityLayout();
    for(int i = 0; i < citySize; i++)
        cityPlan[i] = blueprint[i];

    freeFileResources();

    initscr(); //initializes stdscr, a curses global variable.
    cbreak();
    noecho();
    curs_set(false);
    keypad(stdscr, true);
    nodelay(stdscr, true);

    int x, y;
    getmaxyx(stdscr, y, x);

    //get the height if the highest building
    int highestBuilding = -1;
    for(int i = 0; i < citySize; i++)
    {
        if(cityPlan[i] > highestBuilding)
            highestBuilding = cityPlan[i];
    }

    //print a usage message to curses
    mvprintw(0, 5, "Enter 'q' to quit at end of attack, or control-C");
    refresh();

    //generate the city
    generateCity(cityPlan, x, y, citySize);

    //thread IDs
    pthread_t defense_tid;
    pthread_t offense_tid;

    //setup the defense
    Defender* player = createDefender(y - (highestBuilding + 2), defenseName, x / 2, x);

    //setup the offense
    Offender* offense = createOffender(offenseName, maxMissiles, x, y);

    //start the defensive and offenseive threads
    pthread_create(&defense_tid, NULL, runDefense, (void*) player);
    pthread_create(&offense_tid, NULL, runOffense, (void*) offense);

    //wait until threads are done
    pthread_join(offense_tid, NULL);
    pthread_join(defense_tid, NULL);

    //hold window open until a key press
    getch();

    //close the window
    endwin();

    //free remaining resources
    free(defenseName);
    free(offenseName);
    free(cityPlan);
    //TODO: additional frees for the offense and defense.

    //end program
    return EXIT_SUCCESS;
}
