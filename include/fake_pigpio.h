/**
 ******************************************************************************
 * @file    	mqtt.h
 * @author  	Lorenzo Joale'
 * @date    	12 Aug 2022
 *******************************************************************************
 */

#ifndef _FAKE_PIGPIO_H
#define _FAKE_PIGPIO_H

#define PI_OUTPUT   0
#define PI_INPUT    0


int gpioSetMode(int a, int b);
int gpioWrite(int a, int b);
int gpioDelay(int a);
int gpioRead(int a);
#endif /* _FAKE_PIGPIO_H */