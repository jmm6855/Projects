//file: offense.h
//Description: Header file to define functionality of the offensive force
//@author Jacob Manelius

#ifndef OFFENSE_H_
#define OFFENSE_H_

///Offensive structure to create an offender object
///@param name The name of the offense
///@param maxMissiles The max number of missiles it can create (0 for infinite)
///@param screenX The witdth of the screen
///@param screenY The height of the screen
typedef struct Offender
{
    char* name;
    size_t numMissiles;
    int screenX;
    int screenY;

}Offender;

///constructor for the offender object
///@param name The name of the offense
///@param maxMissiles The max number of missiles it can create (0 for infinite)
///@param screenX The witdth of the screen
///@param screenY The height of the screen
///@returns Offender* A pointer to the new structure
Offender* createOffender(char* name, size_t maxMissiles, int screenX, int screenY);

///run the offender thread, using the parameters to mount an attack
///@param offender The offender structure that holds information representing the attack on the city
void* runOffense(void* offense);
#endif
