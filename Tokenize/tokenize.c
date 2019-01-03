/* file: tokenize.c
 * Description: accepts and processes strings as they conform to a custom transitino matrix.
 * @author Jacob Manelius
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "classes.h"


#define MAX_STATES 75
#define MAX_CHARACTER_CLASSES 12 
#define ERROR_STATE 99

//struct to manage and build the transition matrix
struct transition_matrix
{
    int loaded;
    int next_state[MAX_CHARACTER_CLASSES];
    char action[MAX_CHARACTER_CLASSES];
};

//function definition
int get_character_class(int inputChar);

char linebuf[128];    // Buffer used to read lines from file
char tag[81];         // Used for reading first three lines
char buf1[10];
char buf2[10];
char save_token[81];
char action;
char *token;
char *cc_token;
int i;                // Counters
int k;
int ch;
int rejecting = 0;
int cur_state;
int token_offset = 0;
int character_class;
int next_state;
int value;            // Used to hold a value for first three lines
int total_states;     // Total number of states in TM
int start_state;      // State that is the first state in the TM
int accepting_state;  // Final state in the TM

//accepts and processes input from a file to create a transiton matrix.
int main(argc, argv)
    int argc;
    char* argv[];
{  
    // Make sure the program was run with the right number of aruguments
    if(argc != 2)
    {
        fprintf(stderr, "usage: ./tokenize tmfile\n");
        return EXIT_FAILURE;
    }

    FILE* fp;
    // Try and open the file
    if((fp = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "%s: No such file or directory\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Read in the first three lines and save the values
    for(i=0;i<3;i++)
    {
        fgets(linebuf,128,fp);
        sscanf(linebuf,"%s %d",tag,&value);
        if(!strcmp(tag,"states"))
            total_states = value;
        else if(!strcmp(tag,"start"))
            start_state = value;
        else if(!strcmp(tag,"accept"))
            accepting_state = value;
    }

    //make the matrix
    struct transition_matrix tm[75];

    //First zero out the entire transition matrix
    for(i=0;i<MAX_STATES;i++)
    {
        tm[i].loaded = 0;
        for(k=0;k<MAX_CHARACTER_CLASSES;k++) 
        {
            tm[i].next_state[k] = 99;
            tm[i].action[k] = 'd';
        }
    }

    //Turn on the Accepting State
    tm[accepting_state].loaded = 1;

    while(fgets(linebuf,128,fp))
    {
        linebuf[strlen(linebuf) - 1] = '\0';
        //get the first token 
        token = strtok(linebuf, " ");
        cur_state = atoi(token);
        token = strtok(NULL, " ");

        //walk through other tokens
        while( token != NULL ) 
        {
            sscanf(token,"%[^/]/%[^/]", buf1, buf2);
            character_class = atoi(buf1);
            action = buf2[strlen(buf2) - 1];
            buf2[strlen(buf2) - 1] = '\0';
            next_state = atoi(buf2);
            tm[cur_state].next_state[character_class] = next_state;
            tm[cur_state].action[character_class] = action;
            tm[cur_state].loaded = 1;

            token = strtok(NULL, " ");
        }
    }

    fclose(fp);
    //print the matrix
    printf("Scanning using the following matrix:\n");
    printf("  ");
    for(k=0;k<MAX_CHARACTER_CLASSES;k++)
    {
        if(k == MAX_CHARACTER_CLASSES - 1)
            printf("%4d", k);
        else
            printf("%4d ", k);
    }
    printf("\n");

    for(k=0;k<MAX_STATES;k++)
    {
        if(tm[k].loaded)
        {
            printf("%2d  ",k);
            for(i=0;i<MAX_CHARACTER_CLASSES;i++)
                if(i == MAX_CHARACTER_CLASSES - 1)
                    printf("%2d%c", tm[k].next_state[i], tm[k].action[i]);
                else
                    printf("%2d%c  ",tm[k].next_state[i],tm[k].action[i]);
            printf("\n");
        }
    }

    //get input
    cur_state = start_state;
    strcpy(save_token,"");
    token_offset = 0;

    while((ch = getchar()) != EOF)
    {
        character_class = get_character_class(ch);

        if((rejecting) && (character_class == CC_WS))
        {
            rejecting = 0;
            continue;
        }

        if(rejecting)
            continue;

        printf("%d ",cur_state);

        if(tm[cur_state].action[character_class] == 's') //add character to buffer
            save_token[token_offset++] = ch;

        //advance to next state
        cur_state = tm[cur_state].next_state[character_class];

        //check for accepting state
        if(cur_state == accepting_state)
        {
            printf("%d ",accepting_state);
            save_token[token_offset] = '\0';
            printf("recognized '%s'\n",save_token);
            strcpy(save_token,"");
            token_offset = 0;
            cur_state = start_state;
        }

        //check for error state
        if(cur_state == 99)
        {
            printf("%d ", cur_state);
            printf("rejected\n");
            strcpy(save_token,"");
            token_offset = 0;
            cur_state = start_state;
            rejecting = 1;
        }
        //check for end of input
        if(ch == '\n')
        {
            printf("%d %d EOF\n", start_state, accepting_state);
            break;
        }
    }
}

//gets the character class of a given character.
//@return character class for given character.
int get_character_class(int inputChar)
{
    int retval = CC_ERROR;

    switch(inputChar)
    {
        case 32:  //space
        case 9:  //tab
            retval = CC_WS;
            break;
        case 10: //newline
            retval = CC_NEWLINE;
            break;
        case 48: //0
            retval = CC_DIG_0;
            break;
        case 49: //1-7
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
            retval = CC_DIG_1_7;
            break;
        case 56: //8-9
        case 57:
            retval = CC_DIG_8_9;
            break;
        case 47: //slash
            retval = CC_CHAR_SLASH;
            break;
        case 42: //star
            retval = CC_CHAR_STAR;
            break;
        case 43: //+
        case 45: //-
        case 37: //%
            retval = CC_ARITH_OP;
            break;
        default:
            if(isalpha(inputChar))
                retval = CC_ALPHA;
            else if(__isascii(inputChar))
                retval = CC_OTHER;
            else if(inputChar == EOF)
                retval = CC_EOF;
            else
                retval = CC_ERROR;
            break;
    }

    return(retval);
}
