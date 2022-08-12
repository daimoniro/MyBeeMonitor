
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

/* Private defines ------------------------------------------------------------*/

/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/

/* Public variables -----------------------------------------------------------*/

/* Private function prototypes ------------------------------------------------*/

/* Private function bodies ----------------------------------------------------*/


char *build_get_test_json()
{
	json_t *obj = json_pack("{"
							// inbound
							"s:{s:i,s:i,s:f,s:s}"
							"}",

							/* test start */
							"inbound",
							"current_iteration", 1,
							"current_step", 2,
							"current_iteration_time", 3.5,
							"remaining_iteration","ITdsdsdsd"
	);

	
	char *json = json_dumps(obj, JSON_PRESERVE_ORDER | JSON_REAL_PRECISION(4));

	//printf("json: %s\n",json);
	json_decref(obj);
	return json;
}