/*
 * log.c
 *
 *  Created on: 15 Aug 2018
 *      Author: pi
 */


//---------------------------
//			includes
//---------------------------

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>

#include "../include/log.h"
#include "../include/debug.h"


#include "version.h"




//*****************************************************************************
//	Global Variable
//*****************************************************************************
FILE * FileLog;
char NomeFileLog[64];
static sem_t sem_log;
unsigned char logFileAperto = 0;




//*****************************************************************************
//	extern Variable
//*****************************************************************************
extern char debugSTR[];


//----------------------------------------------------------------------
//  InitFileLog
//----------------------------------------------------------------------
void InitFileLog()
{

    OpenFileLog();

    if (sem_init(&sem_log, 0, 1) != 0) {
        //TRACE3(2,"LOG",("Error create semaphore log\n");)

    	const char* msg = "Error create semaphore log";
        TRACE4(1,"LOG",ROSSO,NERO_BG,msg,1);
        return;
    }
}


void OpenFileLog(void)
{
    char aubDate[40];
    time_t stCurTime;
    struct tm* stTimeInfo;
    time(&stCurTime);
    stTimeInfo = localtime(&stCurTime);

//	strftime(aubDate, sizeof(aubDate), "%Y%m%d", stTimeInfo);
    strftime(aubDate, sizeof(aubDate), "%Y%m%d_%H%M", stTimeInfo);

    sprintf(NomeFileLog, "/home/root/log/appl_%s.log", aubDate);

    FileLog = fopen(NomeFileLog,"a");
    if(FileLog==NULL)
    {
        printf("\e[31mImpossibile aprire file %s\e[37m\n",NomeFileLog);
    }
    else
    {
        logFileAperto = 1;
    }

}


//----------------------------------------------------------------------
//  CloseFileLog
//----------------------------------------------------------------------
void CloseFileLog()
{
    fclose(FileLog);
    logFileAperto = 0;
}


//----------------------------------------------------------------------
//  printIntestazioneFiles
//----------------------------------------------------------------------
void printIntestazioneFiles()
{

    TRACE4(1,"MAIN",BIANCO,NERO_BG,"-------------------------------------------------",1);
    sprintf(debugSTR,"Start ATM Algorab ULB ver: 1.1");
    TRACE4(1,"MAIN",BIANCO,NERO_BG,debugSTR,1);
    TRACE4(1,"MAIN",BIANCO,NERO_BG,"-------------------------------------------------",1);

}

//----------------------------------------------------------------------
//  checkDimensioneFileLog
//----------------------------------------------------------------------
void checkDimensioneFileLog()
{
    long sizeLogFile;

    fseek(FileLog, 0, SEEK_END);
    sizeLogFile=ftell(FileLog);

    if(sizeLogFile > MAX_DIMENSIONE_FILELOG)
    {
        fclose(FileLog);
        //TRACE3(1,"LOG",("\e[31mCancello il file Log.txt\e[37m\n"););
        TRACE4(1,"LOG",ROSSO,NERO_BG,"Cancello il file appl.log",1);

        FileLog = fopen(NomeFileLog,"w");
        fclose(FileLog);

        InitFileLog();
    }


    //TRACE3(2,"LOG",("\e[31mIl file e' grande: %ld\e[37m\n",sizeLogFile););
}


//----------------------------------------------------------------------
//  ScriviLog
//----------------------------------------------------------------------
void ScriviLog(char * string2log)
{

    time_t	dt_tmp;		// date
    struct tm dc_tmp;	// date components
    char dataora[64];
    int returnWrite = 0;

    if(logFileAperto == 0)
        return;

    sem_wait(&sem_log);

    checkDimensioneFileLog();

    time(&dt_tmp);
    dc_tmp = *localtime(&dt_tmp);

    sprintf(dataora,"%.2d-%.2d-%.4d %.2d:%.2d:%.2d",dc_tmp.tm_mday,dc_tmp.tm_mon+1,dc_tmp.tm_year + 1900,dc_tmp.tm_hour,dc_tmp.tm_min,dc_tmp.tm_sec);

    returnWrite = fprintf(FileLog,"%s --> %s",dataora,string2log);

    if(returnWrite < 0)
        printf("\e[31mScrittura file fallita: returnWrite: %d\e[37m\n",returnWrite);

    fflush(FileLog);

    sem_post(&sem_log);

}


//----------------------------------------------------------------------
//  ScriviLogAppl
//----------------------------------------------------------------------
void ScriviLogAppl(unsigned char * aubDate,char const * subsys,char const * msg)
{

    int returnWrite = 0;

    if(logFileAperto == 0)
        return;

    sem_wait(&sem_log);

    checkDimensioneFileLog();

    returnWrite = fprintf(FileLog,"[%s] - [%7s] => %s\n",aubDate,subsys,msg);

    if(returnWrite < 0)
        printf("\e[31mScrittura file fallita: returnWrite: %d\e[37m\n",returnWrite);

    fflush(FileLog);

    sem_post(&sem_log);

}



/*************************************************************************
 *  FileMng_GetTime
 */
void GetTime4Log(unsigned char* pubTime)
{
  time_t stCurTime;
  struct tm* stTimeInfo;
  struct timeval stTimeval;
  unsigned char aubTime[DATETIME_LEN];

  time(&stCurTime);
  gettimeofday(&stTimeval, NULL);

  stTimeInfo = localtime(&stCurTime);
  strftime((char *)aubTime, DATETIME_LEN, "%Y-%m-%d %H:%M:%S", stTimeInfo);

  sprintf((char *)pubTime, "%s.%03ld", aubTime, stTimeval.tv_usec/1000);
}




