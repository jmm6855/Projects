//file: missile.h
//Description: Header file to describe functionality of the missle object
//@author Jacob Manelius

#ifndef MISSILE_H_
#define MISSILE_H_

///missile structure to store information about each missile
///@param xCord The x coordinate of the specific missile
///@param height The current height of the missile
///@param screenY The height of the screen
///@param speed The rate at which the missile falls in microseconds
typedef struct Missile
{
    int xCord;
    int height;
    int screenY;
    int speed;
}Missile;

///draws an impact mark at the given position
///@param xCord The x coordinate for the impact
///@param yCord The y coordinate for the impact
void drawImpact(int xCord, int yCord);

///advance the missile to its next position
///@param missile the missile to be advanced
void advanceMissile(void* missile);

///constructor for a missile
///@param xCord The x coordinate of the specific missile
///@param height The current height of the missile
///@returns Missile* a new missile pointer
Missile* createMissile(int xCord, int height, int screenY);

///run the missile thread, defines how a missile behaves
///@param missile The missile structure that hold information about the missile
void* runMissile(void* missile);
#endif
