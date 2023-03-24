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

#include "debug.h"
#include "apiario.h"

/* Private defines ------------------------------------------------------------*/

#define FREQUENZA_LETTURA_TEMPERATURE	2

#define ID_SENDORE_INTERNO	0
#define ID_SENDORE_ESTERNO	1


#define FILE_SENSORE_1W_INTERNO "/sys/bus/w1/devices/28-000000103925/w1_slave"
#define FILE_SENSORE_1W_ESTERNO "/sys/bus/w1/devices/28-000000105769/w1_slave"

/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/

static char debug_str_temp[256];
static const char delim[] = "=";

/* Public variables -----------------------------------------------------------*/
int valoriTemperatura[3];
char pathDeviceTemperatureSensor[64];

float tempDS18D20 = 0;

extern apiario_t apiario;

/* Private function prototypes ------------------------------------------------*/
void *temperature_management();
int letturaTemperatura(int idSensore);

/* Private function bodies ----------------------------------------------------*/

//-----------------------------------------------------------------------------
//	StartTemperatureManagement
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
    time_t now;
	int returnFunz = 0;


	//system("sudo modprobe w1-gpio");
	//system("sudo modprobe w1-therm");

	valoriTemperatura[ID_SENDORE_INTERNO] = 0xFFFF;
	valoriTemperatura[1] = 0xFFFF;
	valoriTemperatura[2] = 0xFFFF;

    while(true)
    {

        sleep(1);
		time(&now);


		returnFunz = letturaTemperatura(ID_SENDORE_INTERNO);

		if(returnFunz >0 )
		{

			tempDS18D20 = (float)((int)valoriTemperatura[ID_SENDORE_INTERNO])/((float)1000);

			sprintf(debug_str_temp,"valoreTemperatura int: %f",tempDS18D20);
			TRACE4(1,"TEMP",BIANCO,NERO_BG,debug_str_temp,0);

			apiario.arnie[0].temperature_internal = tempDS18D20;
		}
		else
		{
			sprintf(debug_str_temp,"Errore lettura temperatura. path: %s",pathDeviceTemperatureSensor);
			TRACE4(1,"TEMP",NERO,ROSSO_BG,debug_str_temp,0);
		}


		returnFunz = letturaTemperatura(ID_SENDORE_ESTERNO);

		if(returnFunz >0 )
		{

			tempDS18D20 = (float)((int)valoriTemperatura[ID_SENDORE_ESTERNO])/((float)1000);

			sprintf(debug_str_temp,"valoreTemperatura est: %f",tempDS18D20);
			TRACE4(1,"TEMP",BIANCO,NERO_BG,debug_str_temp,0);

			apiario.temperature_external = tempDS18D20;
		}
		else
		{
			sprintf(debug_str_temp,"Errore lettura temperatura. path: %s",pathDeviceTemperatureSensor);
			TRACE4(1,"TEMP",NERO,ROSSO_BG,debug_str_temp,0);
		}
		
    }
}



//---------------------------------------------------------------------------------------
// letturaTemperatura
//---------------------------------------------------------------------------------------
int letturaTemperatura(int idSensore)
{
	FILE * SensoreFile;
	char Line[256];
	char *svptr;						// puntatore aggiornato dalla funzione strtok_r
	char *s;

	int lineNumber = 0;
	int yesLetto = 0;


	switch(idSensore)
	{
		case ID_SENDORE_INTERNO:
			SensoreFile = fopen(FILE_SENSORE_1W_INTERNO,"r");
			break;
		case ID_SENDORE_ESTERNO:
			SensoreFile = fopen(FILE_SENSORE_1W_ESTERNO,"r");
			break;
		/*case ID_SENDORE_FERMENTATORE:
			SensoreFile = fopen(FILE_SENSORE_1W_FERMENTATORE,"r");
			break;*/

        default:
           valoriTemperatura[idSensore] = 0xFFFF;
		    return -1;

	}


	if(SensoreFile==NULL)
	{
		//printf("Impossibile leggere file con ID %d\n", idSensore);

		valoriTemperatura[idSensore] = 0xFFFF;
		return -1;
	}

	while(fgets(Line,256,SensoreFile)!=NULL)
	{
		//printf("Leggo %s da file %s\n",Line,pathDeviceTemperatureSensor);

		//Leggo 15 00 4b 46 7f ff 0b 10 a4 : crc=a4 YES
		//Leggo 15 00 4b 46 7f ff 0b 10 a4 t=1312

		if((Line[0]!='#')&&(Line[0]!='\0')&&(Line[0]!='\n'))
		{


			if(lineNumber == 0)
			{
				if(strstr(Line,"YES") != NULL)
				{
					yesLetto = 1;
				}
			}

			if(lineNumber == 1)
			{
				if(yesLetto == 1)
				{
					//printf("YES letto\n");
					s = strtok_r(Line,delim,&svptr);
					s=strtok_r(NULL,delim,&svptr);
					//printf("Dopo = --> %d\n",atol(s));

					valoriTemperatura[idSensore] = atol(s);

				}
				else
				{
					valoriTemperatura[idSensore] = 0xFFFF;
				}


			}

			lineNumber++;
		}

	}

	fclose(SensoreFile);

	return 1;

}

