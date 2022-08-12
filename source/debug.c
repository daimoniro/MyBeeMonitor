/*
 * debug.c
 *
 *  Created on: 15 Aug 2018
 *      Author: pi
 */




#include	"../include/debug.h"



/*-----------------------------------------------------------------------*/
/**
*
*
* @param [IN] level: debug level
* @param [OUT] subsys: modules to debug
*
* @return
*/
int testSubsystem(short level,const char *subsys) {
    char *cp;                     /* Work pointer */
    char vbuf[128];               /* Buffer for variable */
    static short trace_level = -1;/* Trace level after init */
    static char *trace_subsys = 0;/* Pointer to environment val*/

    /*
     * One time initialization : Test for the presence
     * of the environment variables TRACE_LEVEL and
     * TRACE_SUBSYS.
     */
    if ( trace_level == -1 ) {
        trace_subsys = getenv("TRACE_MODULES"); /* Get variable */
        if ( (cp = getenv("TRACE_LEVEL")) != 0 )
            trace_level = atoi(cp);         /* Trace level */
        else
            trace_level = 0;                /* No trace */
    }

    /*
     * If the TRACE_LEVEL is lower than this macro
     * call, then return false :
     */
    if ( trace_level < level )    /* Tracing at lower lvl? */
        return 0;                 /* Yes, No trace required */

    /*
     * Return TRUE if no TRACE_SUBSYS environment
     * value is defined :
     */
    if ( !trace_subsys )          /* TRACE_SUBSYS defined? */
        return 1;                 /* No, Trace ALL subsystems */

    /*
     * Copy string so we don't modify env. variable :
     */
    strncpy(vbuf,trace_subsys,sizeof vbuf);
    vbuf[sizeof vbuf - 1] = 0;    /* Enforce nul byte */

    /*
     * Scan if we have a matching subsystem token :
     */
    for ( cp=strtok(vbuf,","); cp != 0; cp=strtok(NULL,",") )
        if ( !strcmp(subsys,cp) ) /* Compare strings? */
            return 1;             /* Yes, trace this call */
    return 0;                     /* Not in trace list */
}

/*-----------------------------------------------------------------------*/

//void TRACE4(short level,char *subsys,char *colorForeground, char *colorBackground, char* msg, unsigned char log)
void TRACE4(short level, char const *subsys,char const *colorForeground, char const *colorBackground, char const* msg, unsigned char log)
{
    unsigned char aubDate[DATETIME_LEN];

    char msg_local[256];
    strncpy(msg_local,msg,sizeof(msg_local));


    GetTime4Log(aubDate);

    if ( testSubsystem(level,subsys) )
    {
        printf("[%s] - ",aubDate);
        printf("[%7s] => ",subsys);
        printf("%s%s",colorForeground,colorBackground);
        printf("%s", msg_local);
        printf("\e[37m\e[40m\n");
        fflush(stdout);
    }

    if(log == 1)
    {
       // ScriviLogAppl(aubDate,subsys,msg_local);
    }
}


