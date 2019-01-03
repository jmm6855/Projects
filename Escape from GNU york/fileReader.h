//file: fileRader.h
//Description: Header file to define functionality of a file reader
//@author Jacob Manelius

#ifndef FILEREADER_H_
#define FILEREADER_H_

///Checks that the file exists and is openable.
///@param fileName String representation of a file name.
///@returns bool True if the file can be opened.
bool fileExists(char* fileName);

///Get the name for the defense force. (Max 80 characters)
///@returns Character pointer to a string naming the defense.
char* getDefenseName();

///Get the name for the offensive force. (Max 80 characters)
///@returns Character pointer to a string naming the offence.
char* getOffenseName();

///Get the maximum number of missiles that will be launched. 0 for infinite.
///@returns A non-negative number
size_t getMaxMissiles();

///Get a sequence of integers that represents the witdth and heights of the buildings in the cityscape
///@returns Integer pointer representing the city read from the file
int* getCityLayout();

///Free any memory that was allocated to the file read process.
void freeFileResources();

///get the length of the int* that represents the city
///@returns an integer that is the length of the city
int getCitySize();

///Read the file and save the information in it
///@param fileName String representation of a config-file name
///@returns boolean true if file was successfully read
bool readFile(char* fileName);
#endif
