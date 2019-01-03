//file: offense.c
//Description: implementation of the offense.h header file
//@author Jacob Manelius

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "offense.h"
#include "missile.h"

//create the offender object
Offender* createOffender(char* name, size_t maxMissiles, int screenX, int screenY)
{
    Offender* output = (Offender*)malloc(sizeof(Offender));
    output->name = name;
    output->numMissiles = maxMissiles;
    output->screenX = screenX;
    output->screenY = screenY;
    return output;
}

//run the offense thread
void* runOffense(void* offense)
{
    size_t batchSize = 8;
    //get the number of missiles
    size_t numMissiles = ((Offender*)offense)->numMissiles;

    if(numMissiles != 0)
    {
        size_t createIndex = 0;
        size_t joinIndex = 0;

        //create all the missiles
        Missile* missiles[numMissiles];
        for(size_t i = 0; i < numMissiles; i++)
        {
            int randX = (rand() % (((Offender*)offense)->screenX + 1));
            missiles[i] = createMissile(randX, 5, ((Offender*)offense)->screenY);
        }

        size_t missilesRemaining = numMissiles;
        //setup ids for each missile
        pthread_t tids[numMissiles];
        while(missilesRemaining > 0)
        {
            //launch a batch of missiles
            for(size_t i = 0; i < batchSize; i++)
            {
                if(createIndex < numMissiles)
                {
                    pthread_create(&tids[createIndex], NULL, runMissile, (void*)missiles[createIndex]);
                    createIndex++;
                    missilesRemaining--;
                }
            }

            //wait for the batch to join
            for(size_t i = 0; i < batchSize; i++)
            {
                if(joinIndex < numMissiles)
                {
                    pthread_join(tids[joinIndex], NULL);
                    joinIndex++;
                }
            }
        }

        //end the offense
        pthread_exit(0);
    }
    //infinite missiles
    else
    {
        while(1)
        {
            Missile* missiles[batchSize];
            for(size_t i = 0; i < batchSize; i++)
            {
                int randX = (rand() % (((Offender*)offense)->screenX + 1));
                missiles[i] = createMissile(randX, 5, ((Offender*)offense)->screenY);
            }
            pthread_t tids[batchSize];

            //launch a batch of missiles
            for(size_t i = 0; i < batchSize; i++)
            {
                pthread_create(&tids[i], NULL, runMissile, (void*)missiles[i]);
            }

            //join the batch
            for(size_t i = 0; i < batchSize; i++)
            {
                pthread_join(tids[i], NULL);
            }
        }
    }
}
