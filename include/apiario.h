/**
 ******************************************************************************
 * @file    	apiario.h
 * @author  	Lorenzo Joale'
 * @date    	12 Aug 2022
 *******************************************************************************
 */

#ifndef _APIARIO_H
#define _APIARIO_H

#define MAX_NUM_ARNIE    10

typedef struct arnia_s {
    float temperature_internal; 
    float temperature_internal_2; 
    float humidity_internal;
    float weight;
} arnia_t;

typedef struct apiario_s {
    float rain;
    float temperature_external;
    float lux;
    arnia_t arnie[MAX_NUM_ARNIE];
} apiario_t;


#endif /* _APIARIO_H */