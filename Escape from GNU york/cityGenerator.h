//file: cityGenerator.h
//Description: Header file to define functions that build the city in the curses environment
//@author Jacob Manelius

#ifndef CITYGENERATOR_H_
#define CITYGENERATOR_H_

///Builds the wall of a building
///@param height How tall the wall is
///@param xCord Where on the X axis of the screen to print the wall
void generateWall(int height, int screenY, int xCord);

///Builds the city in the curses environment
///@param blueprint Representation of the city in number form
///@param screenX The horizontal size of the screen
///@param screenY The vertical size of the screen
void generateCity(int* blueprint, int screenX, int screenY, int citySize);

#endif
