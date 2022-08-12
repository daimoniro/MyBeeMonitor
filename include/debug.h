/*
 * debug.h
 *
 *  Created on: 15 Aug 2018
 *      Author: pi
 */

#ifndef INCLUDE_DEBUG_H_
#define INCLUDE_DEBUG_H_



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "log.h"


#define	NERO 			"\e[30m"
#define	ROSSO 			"\e[31m"
#define	ROSSO_VIVO 		"\e[1;31m"
#define	VERDE 			"\e[32m"
#define	VERDE_VIVO 		"\e[1;32m"
#define	GIALLO 			"\e[33m"
#define	GIALLO_VIVO 	"\e[1;33m"
#define	BLU 			"\e[34m"
#define	BLU_VIVO 		"\e[1;34m"
#define	VIOLA			"\e[35m"
#define	VIOLA_VIVO 		"\e[1;35m"
#define	CELESTE			"\e[36m"
#define	CELESTE_VIVO 	"\e[1;36m"
#define	BIANCO			"\e[37m"
#define	BIANCO_VIVO 	"\e[1;37m"

#define	NERO_BG 		"\e[40m"
#define	ROSSO_BG 		"\e[41m"
#define	VERDE_BG 		"\e[42m"
#define	GIALLO_BG 		"\e[43m"
#define	BLU_BG 			"\e[44m"
#define	VIOLA_BG 		"\e[45m"
#define	CELESTE_BG 		"\e[46m"
#define	GRIGIO_BG 		"\e[47m"


// Underline
#define 	K_4Black  	"\e[4;30m"       // Black
#define 	K_URed  	"\e[4;31m"       // Red
#define 	K_UGreen    "\e[4;32m"       // Green
#define 	K_UYellow 	"\e[4;33m"       // Yellow
#define 	K_UBlue  	"\e[4;34m"       // Blue
#define 	K_UPurple  	"\e[4;35m"       // Purple
#define 	K_UCyan  	"\e[4;36m"       // Cyan
#define 	K_UWhite  	"\e[4;37m"       // White


//	Trace by TRACE_LEVEL & TRACE_SUBSYS macro :
#define TRACE(level,subsys,msg) { \
    if ( testSubsystem(level,subsys) ) { \
        printf("%s @ %u : ",__FILE__,__LINE__); \
        printf msg; \
        fflush(stdout); \
    } \
}

#define TRACE2(level,subsys,msg) { \
    if ( testSubsystem(level,subsys) ) { \
        printf("%s @ %u : ",__FILE__,__LINE__); \
        printf msg; \
        fflush(stdout); \
         ScriviLog(msg);\
    } \
}

#define TRACE3_(level,subsys,msg) { \
    if ( testSubsystem(level,subsys) ) { \
    	unsigned char aubDate[DATETIME_LEN]; \
    	GetTime4Log(aubDate); \
        printf("[%s] - ",aubDate); \
        printf("[%7s] => ",subsys); \
        printf msg; \
        printf("\e[37m"); \
        fflush(stdout); \
    } \
}







    //if(level < 4) printf("fuori %s\n",subsys);
    //fflush(stdout);

int testSubsystem(short,const char*);
//void TRACE4(short level,char *subsys,char* msg);
void TRACE4(short level, char const *subsys,char const *colorForeground, char const *colorBackground, char const* msg, unsigned char log);




#endif /* INCLUDE_DEBUG_H_ */
