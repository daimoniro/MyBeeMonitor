/* Includes ------------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>


#include "../include/version.h"
#include "../include/module.h"


/* Private defines ------------------------------------------------------------*/


/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/

/* Public variables -----------------------------------------------------------*/

/* Private function prototypes ------------------------------------------------*/

/* Public function bodies -----------------------------------------------------*/

/* Private function bodies ----------------------------------------------------*/


int main(int argc, char *argv[])
{
	StartModuleManagement();

	StartTemperatureManagement();

	StartHumManagement();

	StartRainManagement();

	StartWeightManagement();

	StartMqttManagement();

	
	while(true)
	{
		sleep(1);
	}

	return 0;
}











