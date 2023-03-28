/**
 ******************************************************************************
 * @file    humidity.c
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
#include <stdint.h>

#include <pigpio.h>
#include "debug.h"
#include "apiario.h"
#include "hardware_conf.h"

/* Private defines ------------------------------------------------------------*/

#define MAX_TIMINGS	85

#define NUM_ARNIE_UMIDITY_SENSOR	1

/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/
int data_dht22[5] = { 0, 0, 0, 0, 0 };

float tempDHT22 = 0;
float humDHT22 = 0;

char debug_str_hum[256];

int array_hardware_pin_dht[] = {DHT_PIN_1};
/* Public variables -----------------------------------------------------------*/
extern apiario_t apiario;

/* Private function prototypes ------------------------------------------------*/
void *humidity_management();
void read_dht_data(int num_arnia);

/* Private function bodies ----------------------------------------------------*/

//-----------------------------------------------------------------------------
//	StartModuleManagement
//-----------------------------------------------------------------------------
void StartHumManagement()
{
	// Demone Configurazione Board
	pthread_t ThNTP;

	// create tread
	pthread_create(&ThNTP, NULL, &humidity_management, NULL);

} 


//-----------------------------------------------------------------------------
//	humidity_management
//-----------------------------------------------------------------------------
void *humidity_management()
{
	int count = 0;

    while(true)
    {
        sleep(2);
		for(count = 0; count < NUM_ARNIE_UMIDITY_SENSOR; count ++ )
		{
			read_dht_data(count);
		}
    }
}


//-----------------------------------------------------------------------------
//	read_dht_data
//-----------------------------------------------------------------------------
void read_dht_data(int num_arnia)
{
	uint8_t laststate	= 1;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;

	data_dht22[0] = data_dht22[1] = data_dht22[2] = data_dht22[3] = data_dht22[4] = 0;

	/* pull pin down for 18 milliseconds */
	gpioSetMode( array_hardware_pin_dht[num_arnia], PI_OUTPUT );
	gpioWrite( array_hardware_pin_dht[num_arnia], 0 );
	gpioDelay( 18000 );

	/* prepare to read the pin */
	gpioSetMode( array_hardware_pin_dht[num_arnia], PI_INPUT );

	/* detect change and read data */
	for ( i = 0; i < MAX_TIMINGS; i++ )
	{
		counter = 0;
		while ( gpioRead( array_hardware_pin_dht[num_arnia] ) == laststate )
		{
			counter++;
			gpioDelay( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = gpioRead( array_hardware_pin_dht[num_arnia] );

		if ( counter == 255 )
			break;

		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			data_dht22[j / 8] <<= 1;
			if ( counter > 16 )
				data_dht22[j / 8] |= 1;
			j++;
		}
	}

	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) &&
	     (data_dht22[4] == ( (data_dht22[0] + data_dht22[1] + data_dht22[2] + data_dht22[3]) & 0xFF) ) )
	{
		float h = (float)((data_dht22[0] << 8) + data_dht22[1]) / 10;
		if ( h > 100 )
		{
			h = data_dht22[0];	// for DHT11
		}
		float c = (float)(((data_dht22[2] & 0x7F) << 8) + data_dht22[3]) / 10;
		if ( c > 125 )
		{
			c = data_dht22[2];	// for DHT11
		}
		if ( data_dht22[2] & 0x80 )
		{
			c = -c;
		}


		sprintf(debug_str_hum,"Arnia #%d ==> DHT22 --> Humidity = %.1f %% Temperature = %.1f *C",num_arnia, h, c);
		TRACE4(1,"HUM",BIANCO,NERO_BG,debug_str_hum,0);

		tempDHT22 = c;
		humDHT22 = h;


		apiario.arnie[num_arnia].humidity_internal = humDHT22;
		apiario.arnie[num_arnia].temperature_internal_2 = tempDHT22;
		  


	}
	else
	{
		//printf( "Data not good, skip\n" );
	}
}


