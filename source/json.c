
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
#include <jansson.h>
#include <time.h>
#include <sys/time.h>

#include "apiario.h"
#include "network.h"
/* Private defines ------------------------------------------------------------*/

/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/

/* Public variables -----------------------------------------------------------*/

/* Private function prototypes ------------------------------------------------*/

/* Private function bodies ----------------------------------------------------*/
extern apiario_t apiario;
extern unsigned long wlan_rx_bytes;
extern unsigned long wlan_tx_bytes;

//----------------------------------------------------------------------
//  gettime_4log_journal
//----------------------------------------------------------------------
void gettime_4log_journal(unsigned char *pubTime)
{
    time_t stCurTime;
    struct tm *stTimeInfo;
    struct timeval stTimeval;
    unsigned char aubTime[48];

    time(&stCurTime);
    gettimeofday(&stTimeval, NULL);

    stTimeInfo = gmtime(&stCurTime);
    strftime((char *)aubTime, 48, "%Y-%m-%d %H:%M:%S", stTimeInfo);

    sprintf((char *)pubTime, "%s.%03ld", aubTime, stTimeval.tv_usec / 1000);

}

char *build_get_apiario_json()
{
	unsigned char timestamp[48];
	gettime_4log_journal(timestamp);

	json_t *obj = json_pack("{"
							"s:{s:s},"      //Timestamp
							"s:{s:f,s:f,s:f,s:i,s:i},"      //common
                            "s:{s:f,s:f,s:f,s:f},"  //arnia_1
                            "s:{s:f,s:f,s:f,s:f},"  //arnia_2
                            "s:{s:f,s:f,s:f,s:f}"   //arnia_3
							"}",
							"Timestamp",
								"ts",timestamp,
							"common",
							    "rain", apiario.rain,
							    "temperature_external", apiario.temperature_external,
                                "lux",apiario.lux,
								"wlan_rx_bytes",wlan_rx_bytes,
								"wlan_tx_bytes",wlan_tx_bytes,
                            "arnia_1",
							    "temperature_internal", apiario.arnie[0].temperature_internal,
								"temperature_internal_2", apiario.arnie[0].temperature_internal_2,
							    "humidity_internal", apiario.arnie[0].humidity_internal,
                                "weight", apiario.arnie[0].weight,
                            "arnia_2",
							     "temperature_internal", apiario.arnie[1].temperature_internal,
								 "temperature_internal_2", apiario.arnie[1].temperature_internal_2,
							    "humidity_internal", apiario.arnie[1].humidity_internal,
                                "weight", apiario.arnie[1].weight,
                            "arnia_3",
							     "temperature_internal", apiario.arnie[2].temperature_internal,
								 "temperature_internal_2", apiario.arnie[2].temperature_internal_2,
							    "humidity_internal", apiario.arnie[2].humidity_internal,
                                "weight", apiario.arnie[2].weight
	);

	
	char *json = json_dumps(obj, JSON_PRESERVE_ORDER | JSON_REAL_PRECISION(4));

	json_decref(obj);
	return json;
}