/**
 ******************************************************************************
 * @file    rain.c
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
void *rain_management();

/* Private function bodies ----------------------------------------------------*/

//-----------------------------------------------------------------------------
//	StartRainManagement
//-----------------------------------------------------------------------------
void StartRainManagement()
{
	// Demone Configurazione Board
	pthread_t ThNTP;

	// create tread
	pthread_create(&ThNTP, NULL, &rain_management, NULL);

} 


//-----------------------------------------------------------------------------
//	rain_management
//-----------------------------------------------------------------------------
void *rain_management()
{
    while(true)
    {
        sleep(1);

        apiario.rain = 1.4;
        //printf("Do stuff ...\n");
    }
}