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
#include <pigpio.h>

#include "apiario.h"
/* Private defines ------------------------------------------------------------*/
#define I2C_LUX_ADDR    0x23
#define I2C_LUX_REG_READ        0
#define I2C_LUX_REG_WRITE       0

#define I2C_LUX_CONFIGURATION_BYTE  0x10
/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/
int lux_i2c_handle = 0;
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
    lux_i2c_handle = i2cOpen(1,I2C_LUX_ADDR,0);

    if(lux_i2c_handle < 0)
    {
         printf("Sensore luce non disponibile \n");   
         apiario.lux = -1;
         return NULL;
    }
    else
    {
        printf("Lux I2C handle: %d\n",lux_i2c_handle);
        int ret = i2cWriteByte(lux_i2c_handle,I2C_LUX_CONFIGURATION_BYTE);

        printf("ret i2cWriteByte: %d\n",ret);
    }

    while(true)
    {
        sleep(1);

        int word=i2cReadWordData(lux_i2c_handle,I2C_LUX_REG_READ);
        int lux=((word & 0xff00)>>8) | ((word & 0x00ff)<<8);
        printf("word: %d --> lux : %d\n",word,lux);
        apiario.lux = (float)lux;
        //printf("Do stuff ...\n");
    }
}