//@author Jacob Manelius
//mopsolver.c
//10/28/2018
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "Queue.h"

//variables
//booleans for the command line argument flags
static bool prettyPrint = false;
static bool solutionSteps = false;
static bool printPath = false;
static bool inFile = false;
static bool outFile = false;
static char* inFileName; //possible intput file name
static char* outFileName; //possible output file name
static FILE* input; //possible file input
static FILE* output; //possible file output


//neighbor node
static Qnode* neighbor = NULL;
//the maze pointer
static char** maze;

//the number of rows in the maze
static int numberOfRows = 0;
//the number of columns in the maze
static int numberOfCols = 0;
//the number of steps in the solution
static int numberOfSteps = 0;
//boolean for the solvability of the maze
static bool solvable;

//help message for -h flag and execution failures
static char* helpMessage = "Options:\n"
"-h      Print this helpful message to stdout and exit.\n"
"-d      Pretty print (display) the maze after reading.  (Default: off)\n"
"-s      Print shortest solution steps.                  (Default: off)\n"
"-p      Print an optimal path.                          (Default: off)\n"
"-i INFILE       Read maze from INFILE.                  (Default: stdin)\n"
"-o OUTFILE      Write all output to OUTFILE.            (Default: stdout)";


//functions
int fewestSteps(); //counts the shortest solve path
void buildMaze(); //read in and build the maze from input
void printMaze(); //print the maze when the pretty print flag is used
void printSolutionSteps(); //prints the number of steps in the solution
void solveMaze(); //solves the maze and saves the shortest path, # of steps, and whether or not it is solvable
//check if the index is valid
bool isValid(int row, int col);
void cleanup(); //free resources that have been allocated

//returns whether or not a coordinate would be a valid part of the maze
bool isValid(int row, int col)
{
    if(row < numberOfRows && row >= 0 &&
            col < numberOfCols && col >= 0)
        return true;
    else
        return false;
}

//check the neighbors of the current position in the queue
//and adds them to the queue if they have not been in it
void addNeighbors(Queue* queue, Qnode* currentNode)
{
    //get the current row and column
    int currentRow = currentNode->row;
    int currentCol = currentNode->col;

    //check up neighbor is valid
    if(isValid(currentRow-1, currentCol))
    {
        if(maze[currentRow-1][currentCol] == '0')
        {
           neighbor = createNode(currentNode->row-1, currentNode->col, currentNode);
            if(!contains(queue, neighbor))
                enqueue(queue, neighbor);
        }
    }
    //check down neighbor is valid
    if(isValid(currentRow+1, currentCol))
    {
        if(maze[currentRow+1][currentCol] == '0')
        {
            neighbor = createNode(currentNode->row+1, currentNode->col, currentNode);
            if(!contains(queue, neighbor))
                enqueue(queue, neighbor);
        }
    }
    //check left neighbor is valid
    if(isValid(currentRow, currentCol-1))
    {
        if(maze[currentRow][currentCol-1] == '0')
        {
            neighbor = createNode(currentNode->row, currentNode->col-1, currentNode);
            if(!contains(queue, neighbor))
                enqueue(queue, neighbor);
        }
    }
    //check right neighbor is valid
    if(isValid(currentRow, currentCol+1))
    {
        if(maze[currentRow][currentCol+1] == '0')
        {
            neighbor = createNode(currentNode->row, currentNode->col+1, currentNode);
            if(!contains(queue, neighbor))
                enqueue(queue, neighbor);
        }
    }
}

//perform a breadth first search on the maze
void solveMaze()
{
    Queue* queue = createQueue();
    Qnode* start = createNode(0, 0, NULL);
    Qnode* currentNode;

    enqueue(queue, start);
    bool done = false;
    while(!isEmpty(queue) && !done)
    {
        currentNode = dequeue(queue);
        //check if the destination has been reached
        if(currentNode->row == numberOfRows-1 && currentNode->col == numberOfCols-1)
        {
            solvable = true;
            done = true;
        }
        else //continue searching
        {
            addNeighbors(queue, currentNode);
        }
    }

    if(done)
    {
        while(currentNode->parent != NULL)
        {
            numberOfSteps++;
            maze[currentNode->row][currentNode->col] = '+';
            currentNode = currentNode->parent;
        }
        maze[currentNode->row][currentNode->col] = '+';
        solvable = true;
    }
    else
    {
        solvable = false;
    }
    //cleanup the queue
    freeQueue(queue);
}

//output the number of steps in the solution
void printSolutionSteps()
{
    if(solvable)
    {
        //account for the start space
        numberOfSteps = numberOfSteps + 1;
        fprintf(output, "Solution in %d steps.\n", numberOfSteps);
    }
    else
    {
        fprintf(output, "No solution.\n");
    }
}

//free the resources allocated to the maze
void cleanup()
{
    for(int i = 0; i < numberOfRows; i++)
    free(maze[i]);
}

//print the maze with the pretty formatting
void printMaze()
{
    //print the top wall of the pretty print maze
    fprintf(output, "|");
    for(int i = 0; i < numberOfCols * 2 + 1; i++)
        fprintf(output, "-");
    fprintf(output, "|\n");

    //print the rows of the maze
    for(int row = 0; row < numberOfRows; row++)
    {
        if(row == 0)
            fprintf(output, " ");
        else
            fprintf(output, "|");
        for(int col = 0; col < numberOfCols; col++)
        {
            char toPrint;

            switch(maze[row][col])
            {
                //print open space
                case '0':
                    toPrint = '.';
                    break;
                    //print walls
                case '1':
                    toPrint = '#';
                    break;
                case '+':
                    toPrint = '+';
                    break;
                    //print unexpected character
                default:
                    toPrint = '!';
            }
            fprintf(output, " %c", toPrint);
        }
        if(row == numberOfRows - 1)
            fprintf(output, "  \n");
        else
            fprintf(output, " |\n");
    }

    //print the bottom wall of the pretty print maze
    fprintf(output, "|");
    for(int i = 0; i < numberOfCols * 2 + 1; i++)
        fprintf(output, "-");
    fprintf(output, "|\n");
}

void buildMaze()
{
    maze = (char**)calloc(1, sizeof(char*));
    char* line = NULL;
    size_t lineLength = 0;

    //get input and store the maze characters into the char**
    while((int)(lineLength = getline(&line, &lineLength, input))  > -1)
    {
        //get the length of the line
        lineLength = strlen(line);

        //length of line with no end line character or spaces
        size_t lengthNoSpaces = strlen(line) / 2;
        //save the length for later
        if(numberOfCols == 0 )
            numberOfCols = lengthNoSpaces;

        //resize the maze for the new line
        maze = realloc(maze, sizeof(char*) * lengthNoSpaces);
        char* lineNoSpaces = (char*)calloc(lengthNoSpaces+1, sizeof(char));

        int noSpaceIndex = 0;
        for(size_t i = 0; i < lineLength; i++)
        {
            if(line[i] == ' ' || line[i] == '\n')
                continue;
            else
                lineNoSpaces[noSpaceIndex++] = line[i];
        }

        //store the line without spaces into the current maze row
        maze[numberOfRows] = strdup(lineNoSpaces);
        numberOfRows++;

        free(lineNoSpaces);
    }
    free(line);

}

//function implementation
int main(int argc, char* argv[])
{
    int opt;
    if(argc > 1)
    {
        //read command line arguments
        while((opt = getopt(argc, argv, "hdspo:i:")) != -1)
        {
            switch(opt)
            {
                case 'h':
                    fprintf(output, "%s\n", helpMessage);
                    return EXIT_SUCCESS;
                    break;
                case 'd':
                    prettyPrint = true;
                    break;
                case 's':
                    solutionSteps = true;
                    break;
                case 'p':
                    printPath = true;
                    break;
                case 'i':
                    inFile = true;
                    inFileName = optarg;
                    break;
                case 'o':
                    outFile = true;
                    outFileName = optarg;
                    break;
                default:
                    fprintf(stderr, "Invalid arguments.\n");
                    fprintf(output, "%s\n", helpMessage);
                    return EXIT_FAILURE;
            }
        }
        //set the input stream to a file name if present
        if(inFile)
            input = fopen(inFileName, "r");
        else
            input = stdin;

        //build the maze
        buildMaze();

        //send all the output to an outfile
        if(outFile)
            output = fopen(outFileName, "w"); //some output file name;
        else //send output to stdout
            output = stdout;


        //perform flag actions
        if(prettyPrint)
            printMaze();

        //need to print the maze first because solveMaze alters the characters
        solveMaze();

        if(solutionSteps)
            printSolutionSteps();
        if(printPath)
            printMaze();

        //close any open files
        if(inFile)
            fclose(input);
        if(outFile)
            fclose(output);

       
        //free the maze
        cleanup();
    }
    else
    {
        fprintf(output, "%s\n", helpMessage);
        return EXIT_FAILURE; //there were no arguments given
    }
}
