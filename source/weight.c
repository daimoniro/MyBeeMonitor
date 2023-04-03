/**
 ******************************************************************************
 * @file    weight.c
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------ */
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#include "apiario.h"

/* Private defines ------------------------------------------------------------*/

/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/

/* Public variables -----------------------------------------------------------*/
extern apiario_t apiario;

/* Private function prototypes ------------------------------------------------*/
void *weight_management();
char** str_split(char* a_str, const char a_delim);

/* Private function bodies ----------------------------------------------------*/

//-----------------------------------------------------------------------------
//	StartWeightManagement
//-----------------------------------------------------------------------------
void StartWeightManagement()
{
	// Demone Configurazione Board
	pthread_t ThNTP;

	// create tread
	pthread_create(&ThNTP, NULL, &weight_management, NULL);

} 


//-----------------------------------------------------------------------------
//	weight_management
//-----------------------------------------------------------------------------
void *weight_management()
{
    FILE    *textfile;
    char    *text;
    long    numbytes;
    char** tokens;
    float weight = 0; 

    while(true)
    {
        sleep(10);

        system("python /home/pi/workspace/MyBeeMonitor/python/myhx711py.py -a 1 -d 5 -s 6");
        sleep(1);
        
        textfile = fopen("/home/pi/workspace/MyBeeMonitor/python/weight.txt", "r");
        if(textfile == NULL)
            continue;
        
        fseek(textfile, 0L, SEEK_END);
        numbytes = ftell(textfile);
        fseek(textfile, 0L, SEEK_SET);  
    
        text = (char*)calloc(numbytes, sizeof(char));   
        if(text == NULL)
            continue;
    
        fread(text, sizeof(char), numbytes, textfile);
        fclose(textfile);
    
        //printf("%s\n",text);

        tokens = str_split(text, ';');

        if (tokens)
        {
            int i;
            for (i = 0; *(tokens + i); i++)
            {
                if(i == 2)
                    weight = atof(*(tokens + i));

                free(*(tokens + i));
            }
           // printf("\n");
            free(tokens);
        }

        free(text);

        printf("weight: %f\n",weight);

        apiario.arnie[0].weight = weight;
        
      //  printf("Do stuff ...\n");
    }
}


char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
