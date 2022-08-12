/*
 * log.h
 *
 *  Created on: 15 Aug 2018
 *      Author: pi
 */

#ifndef INCLUDE_LOG_H_
#define INCLUDE_LOG_H_

//-----------------------------------------------------------------------------
//		define
//-----------------------------------------------------------------------------

#define  	MAX_DIMENSIONE_FILELOG 	40000000

#define		DATETIME_LEN	40

//-----------------------------------------------------------------------------
//		Funzioni
//-----------------------------------------------------------------------------

void printIntestazioneFiles();

void ScriviLog(char * string2log);




void InitFileLog(void);
void OpenFileLog(void);
void ScriviLogAppl(unsigned char * aubDate,char const * subsys,char const * msg);



void checkDimensioneFileLog();
void GetTime4Log(unsigned char* pubTime);
void checkDataLog(void);



#endif /* INCLUDE_LOG_H_ */
