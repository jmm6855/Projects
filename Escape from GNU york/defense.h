//file: defense.h
//Description: Header file to create a thread the user can control to block missiles
//@author Jacob Manelius

#ifndef DEFENSE_H_
#define DEFENSE_H_

///defender structure to create a defender object
///@param height The height of the platform relative to the screen bottom
///@param name The name of the defense
///@param xCord The current coordinate of the platform on the x axis
///@param maxX The maximum x coordinate, the size of the screen
typedef struct Defender
{
    int height;
    char* name;
    int xCord;
    int maxX;
}Defender;

///constructor for the defender object
///@param height The height of the platform relative to the screen bottom
///@param name The name of the defense
///@param startingX The beginning x coordinate of the platform
///@param maxX The maximum x coordinate, the size of the screen
///@returns Defender* A pointer to the new defender struct
Defender* createDefender(int height, char* name, int startingX, int maxX);

///draws the defender shield at a certain position
///@param height The height of the shield
///@param xCord The x coordinate to draw the left side of the shield
void drawShield(int height, int xCord);

///run the defender thread, getting input to move a platform left and right
///@param defender The defender structure that holds information and represents the user
void* runDefense(void* defender);
#endif
