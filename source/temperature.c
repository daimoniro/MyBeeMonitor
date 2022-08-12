/**
 ******************************************************************************
 * @file    module.c
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


/* Private defines ------------------------------------------------------------*/

/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/

/* Public variables -----------------------------------------------------------*/

/* Private function prototypes ------------------------------------------------*/
void *temperature_management();

/* Private function bodies ----------------------------------------------------*/

//-----------------------------------------------------------------------------
//	StartModuleManagement
//-----------------------------------------------------------------------------
void StartTemperatureManagement()
{
	// Demone Configurazione Board
	pthread_t ThNTP;

	// create tread
	pthread_create(&ThNTP, NULL, &temperature_management, NULL);

} 


//-----------------------------------------------------------------------------
//	temperature_management
//-----------------------------------------------------------------------------
void *temperature_management()
{
    while(true)
    {
        sleep(1);
        printf("Do stuff ...\n");
    }
}