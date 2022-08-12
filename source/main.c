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
#include "../include/temperature.h"
#include "../include/humidity.h"
#include "../include/rain.h"
#include "../include/weight.h"
#include "../include/lux.h"
#include "../include/mqtt.h"

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
	(void)argc;
	(void)argv;
	
	StartTemperatureManagement();

	StartHumManagement();

	StartRainManagement();

	StartWeightManagement();

	StartLuxManagement();

	sleep(2);

	StartMqttManagement();

	
	while(true)
	{
		sleep(1);
	}

	return 0;
}











