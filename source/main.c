/* Includes ------------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>
#include <pigpio.h>

#include "../include/version.h"
#include "../include/temperature.h"
#include "../include/humidity.h"
#include "../include/rain.h"
#include "../include/weight.h"
#include "../include/lux.h"
#include "../include/mqtt.h"
#include "../include/network.h"

/* Private defines ------------------------------------------------------------*/

/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/

/* Public variables -----------------------------------------------------------*/

/* Private function prototypes ------------------------------------------------*/
void sig_handler(int signo);
void closePigpioLybrary();

/* Public function bodies -----------------------------------------------------*/

/* Private function bodies ----------------------------------------------------*/


int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	gpioInitialise();

	gpioSetSignalFunc(SIGINT, sig_handler);

	
	StartTemperatureManagement();

	StartHumManagement();

	StartRainManagement();

	StartWeightManagement();

	StartLuxManagement();

	StartNetworkManagement();

	sleep(2);

	StartMqttManagement();

	
	while(true)
	{
		sleep(1);
	}

	return 0;
}

//--------------------------------------------------
// sig_handler
//--------------------------------------------------
void sig_handler(int signo)
{
	 closePigpioLybrary();
	 exit(0);

}

//--------------------------------------------------
// closePigpioLybrary
//--------------------------------------------------
void closePigpioLybrary()
{
	sleep(1);

	gpioTerminate();
}










