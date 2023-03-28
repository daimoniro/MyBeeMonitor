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

#include <pigpio.h>

#include "apiario.h"
#include "debug.h"
#include "log.h"
#include "hardware_conf.h"

/* Private defines ------------------------------------------------------------*/

/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/
char debug_str_rain[256];
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

    /* prepare to read the pin */
	gpioSetMode( RAIN_PIN, PI_INPUT );

    while(true)
    {
        sleep(1);

        apiario.rain = (float)gpioRead( RAIN_PIN );
        sprintf(debug_str_rain,"RAIN --> %f", apiario.rain);
		TRACE4(1,"RAIN",BIANCO,NERO_BG,debug_str_rain,0);
      
        //printf("Do stuff ...\n");
    }
}