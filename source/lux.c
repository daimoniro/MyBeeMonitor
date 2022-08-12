/**
 ******************************************************************************
 * @file    lux.c
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
void *lux_management();

/* Private function bodies ----------------------------------------------------*/

//-----------------------------------------------------------------------------
//	StartLuxManagement
//-----------------------------------------------------------------------------
void StartLuxManagement()
{
	// Demone Configurazione Board
	pthread_t ThNTP;

	// create tread
	pthread_create(&ThNTP, NULL, &lux_management, NULL);

} 


//-----------------------------------------------------------------------------
//	lux_management
//-----------------------------------------------------------------------------
void *lux_management()
{
    while(true)
    {
        sleep(1);

        apiario.lux = 160.7;
        //printf("Do stuff ...\n");
    }
}