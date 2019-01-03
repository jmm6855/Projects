//file: fileReader.c
//Description: implementation of the fileReader.h file
//@author Jacob Manelius

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "fileReader.h"

size_t MAX_NAME_LENGTH = 80;

//variables to store file information
char* defenseName = NULL;
char* offenseName = NULL;
size_t maxMissiles = -1;
int* cityLayout = NULL;
int citySize = 0;

///check that the file exists
bool fileExists(char* fileName)
{
    FILE* fp = fopen(fileName, "r");
    if(fp == NULL)
    {
        fclose(fp);
        fprintf(stderr, "File does not exist.\n");
        return false;
    }
    char ch;
    int lines = 0;
    while(!feof(fp))
    {
        ch = fgetc(fp);
        if(ch == '\n')
            lines++;
    }
    switch(lines)
    {
        case 0:
            fclose(fp);
            fprintf(stderr, "Error: file empty.\n");
            return false;
        case 1:
            fclose(fp);
            fprintf(stderr, "Error: missing defender name.\n");
            return false;
        case 2:
            fclose(fp);
            fprintf(stderr, "Error: missing attacker name.\n");
            return false;
        case 3:
            fclose(fp);
            fprintf(stderr, "Error: missing missile number.\n");
        default:
            break;
    }
    fclose(fp);
    return true;
}

///return the name of the defense
char* getDefenseName()
{
    return defenseName;
}

///return the name of the offense
char* getOffenseName()
{
    return offenseName;
}

///return the max number of missiles
size_t getMaxMissiles()
{
    return maxMissiles;
}

///return the plan for the city
int* getCityLayout()
{
    return cityLayout;
}

///free the memory used by file reader
void freeFileResources()
{
    free(defenseName);
    free(offenseName);
    free(cityLayout);
}

///return the size of the city
int getCitySize()
{
    return citySize;
}

///read the file and store the information
bool readFile(char* fileName)
{
    char line[256];
    size_t lineLength = 0;
    int lineNumber = 1;

    FILE* fp = fopen(fileName, "r");

    int loopcheck = 0;
    //while((int)(lineLength = getline(&line, &lineLength, fp)) > -1)
    while(fgets(line, 256, fp))
    {
        loopcheck++;
        if(line[0] == '#')//commented lines are to be ignored
        {
            loopcheck++;
            if(loopcheck > 500)
            {
                fprintf(stderr, "File unreadable.\n");
                return false;
            }
            continue;
        }
        if(loopcheck > 500)
        {
            fprintf(stderr, "File unreadable.\n");
            return false;
        }

        //get the length of the line
        lineLength = strlen(line);

        char* end = NULL;
        long tempMissiles = -1;
        char* layoutLine = NULL;
        switch(lineNumber++)
        {
            //read the name of the defense
            case 1: if(lineLength > MAX_NAME_LENGTH || lineLength <= 0)
                    {
                        fprintf(stderr, "Improper defense name length.\n");
                        fclose(fp);
                        freeFileResources();
                        return false;
                    }
                    defenseName = malloc(lineLength + 1);
                    strcpy(defenseName, line);
                    break;
                    //read the name of the offense
            case 2: if(lineLength > MAX_NAME_LENGTH || lineLength <= 0)
                    {
                        fprintf(stderr, "Improper offense name length.\n");
                        fclose(fp);
                        freeFileResources();
                        return false;
                    }
                    offenseName = malloc(lineLength + 1);
                    strcpy(offenseName, line);
                    break;
                    //read the maximum number of missiles
            case 3: tempMissiles = strtol(line, &end, 10);
                    for(size_t i = 0; i < lineLength; i++)
                    {
                        if(!isdigit(line[i]))
                        {
                            if(line[i] != '\n')
                                tempMissiles = -1;
                        }
                    }
                    if(tempMissiles < 0)
                    {
                        fprintf(stderr, "Number of missiles must be non-negative integer.\n");
                        fclose(fp);
                        freeFileResources();
                        return false;
                    }
                    maxMissiles = (size_t) tempMissiles;
                    break;
                    //read the city
            default: //add to city layout
                    layoutLine = strtok(line, " ");
                    while(layoutLine != NULL)
                    {
                        cityLayout = realloc(cityLayout, (citySize + 1)*sizeof(int));
                        cityLayout[citySize++] = atoi(layoutLine);
                        layoutLine = strtok(NULL, " ");
                    }
                    break;
        }
    }
    fclose(fp);
    return true; //file was successfully read
}
