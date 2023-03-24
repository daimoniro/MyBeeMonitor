/**
 ******************************************************************************
 * @file    network.c
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
#include <errno.h>
#include <stdint.h>

#include "network.h"


/* Private defines ------------------------------------------------------------*/
#define PROC_NETDEV_FILE "/proc/net/dev"
/* Private macros -------------------------------------------------------------*/

/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/
#define IFSTATS_NAME_LENGTH 64

typedef struct _ifstats_t
{
	char device[IFSTATS_NAME_LENGTH];
	uint64_t rxBytes;
	uint64_t rxPackets;
	uint64_t rxErrors;
	uint64_t rxDrop;
	uint64_t rxFifo;
	uint64_t rxFrame;
	uint64_t rxCompressed;
	uint64_t rxMulticast;
	uint64_t txBytes;
	uint64_t txPackets;
	uint64_t txErrors;
	uint64_t txDrop;
	uint64_t txFifo;
	uint64_t txCollisions;
	uint64_t txCarrier;
	uint64_t txCompressed;
	struct _ifstats_t *next;
} IFStats_t;

/* Private variables ----------------------------------------------------------*/

/* Public variables -----------------------------------------------------------*/
 unsigned long wlan_rx_bytes;
 unsigned long wlan_tx_bytes;
/* Private function prototypes ------------------------------------------------*/
static void evaluate_net_stats();
void *network_management();

/* Private function bodies ----------------------------------------------------*/


//-----------------------------------------------------------------------------
//	StartNetworkManagement
//-----------------------------------------------------------------------------
void StartNetworkManagement()
{
	// Demone Configurazione Board
	pthread_t ThNTP;

	// create tread
	pthread_create(&ThNTP, NULL, &network_management, NULL);

} 

//-----------------------------------------------------------------------------
//	network_management
//-----------------------------------------------------------------------------
void *network_management()
{
    while(true)
    {
		evaluate_net_stats();
		sleep(5);
    }
}



//-----------------------------------------------------------------------------
//	readProcNetDevFile
//-----------------------------------------------------------------------------
/**
   @brief Helper function to read in the /proc/net/dev file from the
   filesystem and return the text in the file. It also gives
   back the size of the text.
   @param output - The text pointer location to link the read text to
   @param size - The final size of the output text
   @return 0 on success, errno on error.
*/
static int readProcNetDevFile(char **output, int32_t *size)
{
	// Clear out errno for use during this function
	errno = 0;
	*output = NULL;
	*size = 0;

	int index = 0;
	int textSize = 64;
	char *filetext = (char *)malloc(sizeof(char) * textSize);

	if (!filetext)
	{
		return errno;
	}

	// Open the proc file that has the necessary info
	FILE *devFile = fopen(PROC_NETDEV_FILE, "r");

	if (!devFile)
	{
		printf("OS: File %s read error\n", PROC_NETDEV_FILE);
		return errno;
	}

	// Since proc files done report their size (at least not this one)
	// Read character by character until EOF is hit.
	int c = 0;
	while ((c = fgetc(devFile)) != EOF)
	{
		filetext[index++] = c;
		if (index >= textSize)
		{
			textSize += 64;
			// Increase memory as needed
			filetext = (char *)realloc(filetext, sizeof(char) * textSize);
			if (!filetext)
			{
				return errno;
			}
		}
	}

	fclose(devFile);

	filetext = (char *)realloc(filetext, sizeof(char) * (index + 1));
	if (!filetext)
	{
		return errno;
	}

	filetext[index] = '\0';
	*output = filetext;
	*size = index;

	// should be 0, unless fclose failed for some reason, but even if
	// it did it is not a game stopper.
	return errno;
}

//-----------------------------------------------------------------------------
//	releaseStats
//-----------------------------------------------------------------------------
/**
   @brief This will release the stats objects obtained by the getIfStats function
   @param stats - The IFStats_t object to free
*/
static void releaseStats(IFStats_t *stats)
{
	IFStats_t *next = NULL;
	while (stats)
	{
		next = stats->next;
		free(stats);
		stats = next;
	}
	return;
}

//-----------------------------------------------------------------------------
//	getIfStats
//-----------------------------------------------------------------------------
static IFStats_t *getIfStats()
{

	char *fileText = NULL;
	int32_t size = 0;
	int error = 0;

	// Read in the /proc/net/dev file
	if ((error = readProcNetDevFile(&fileText, &size)) != 0)
	{
		if (!fileText)
		{
			errno = error;
			return NULL;
		}
	}

	IFStats_t *returnStats = NULL, *stats = NULL;

	// Scan the file text and pull out the approperate data.
	char *lineptr, *tokenptr;
	char *line, *token;
	int i = 0;
	for (char *lines = fileText;; lines = NULL)
	{
		line = strtok_r(lines, "\n", &lineptr);
		if (line == NULL)
		{
			break;
		}

		// Throw away the first two lines of text
		if (++i <= 2)
		{
			continue;
		}

		stats = (IFStats_t *)malloc(sizeof(IFStats_t));
		if (!stats)
		{
			error = errno;
			releaseStats(returnStats);
			errno = error;
			return NULL;
		}

		memset(stats, 0, sizeof(IFStats_t));

		// Break down the line into the values needed.
		int j = 0;
		uint64_t *currentStat = &stats->rxBytes; // Going to do some C magic here
		for (char *tokens = line;; tokens = NULL, j++)
		{
			token = strtok_r(tokens, ": ", &tokenptr);
			if (token == NULL)
			{
				break;
			}

			if (j == 0)
			{
				strncpy(stats->device, token, IFSTATS_NAME_LENGTH - 1);
			}
			else
			{
				*currentStat = (uint64_t)strtoull(token, NULL, 10);
				currentStat++; // Move the pointer forward through the struct
			}
		}

		// Attach the structure just loaded
		if (returnStats == NULL)
		{
			returnStats = stats;
		}
		else
		{
			IFStats_t *current;
			for (current = returnStats; current->next != NULL; current = current->next)
				;
			current->next = stats;
		}
	}

	free(fileText);
	return returnStats;
}

//-----------------------------------------------------------------------------
//	evaluate_net_stats
//-----------------------------------------------------------------------------
static void evaluate_net_stats()
{
	IFStats_t *stats = getIfStats();
	if (!stats)
	{
		wlan_rx_bytes = 0;
		wlan_tx_bytes = 0;
		printf("OS: File %s read error\n", PROC_NETDEV_FILE);

		return;
	}

	for (IFStats_t *current = stats; current != NULL; current = current->next)
	{
		if (strcmp(current->device, "wlan0") == 0)
		{
			wlan_rx_bytes = current->rxBytes;
			wlan_tx_bytes = current->txBytes;
		}
	}
	releaseStats(stats);
}
