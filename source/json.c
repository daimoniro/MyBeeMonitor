
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

#include "apiario.h"

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

char *build_get_apiario_json()
{
	json_t *obj = json_pack("{"
							"s:{s:f,s:f,s:f},"      //common
                            "s:{s:f,s:f,s:f},"  //arnia_1
                            "s:{s:f,s:f,s:f},"  //arnia_2
                            "s:{s:f,s:f,s:f}"   //arnia_3
							"}",

							"common",
							    "rain", apiario.rain,
							    "temperature_external", apiario.temperature_external,
                                "lux",apiario.lux,
                            "arnia_1",
							    "temperature_internal", apiario.arnie[0].temperature_internal,
							    "humidity_internal", apiario.arnie[0].humidity_internal,
                                "weight", apiario.arnie[0].weight,
                            "arnia_2",
							     "temperature_internal", apiario.arnie[1].temperature_internal,
							    "humidity_internal", apiario.arnie[1].humidity_internal,
                                "weight", apiario.arnie[1].weight,
                            "arnia_3",
							     "temperature_internal", apiario.arnie[2].temperature_internal,
							    "humidity_internal", apiario.arnie[2].humidity_internal,
                                "weight", apiario.arnie[2].weight
	);

	
	char *json = json_dumps(obj, JSON_PRESERVE_ORDER | JSON_REAL_PRECISION(4));

	//printf("json: %s\n",json);
	json_decref(obj);
	return json;
}