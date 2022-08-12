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
    while(true)
    {
        sleep(1);

        apiario.arnie[0].weight = 40.4;
        apiario.arnie[1].weight = 0.4;
        apiario.arnie[2].weight = 80.4;
      //  printf("Do stuff ...\n");
    }
}