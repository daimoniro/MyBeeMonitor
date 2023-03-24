/**
 ******************************************************************************
 * @file    log.c
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
#include <mosquitto.h>


#include "json.h"
/* Private defines ------------------------------------------------------------*/
#define OPENVPN_STOP_COMMAND 		"/etc/init.d/openvpn stop"
#define OPENVPN_START_COMMAND 		"/etc/init.d/openvpn start"

/* Private macros -------------------------------------------------------------*/
#define SEC_WITHOUT_BROKER_CONNECTION 	120*60
#define FREQUENCY_PUBLISH_MQTT		60
/* Private data types ---------------------------------------------------------*/

/* Private enumerations -------------------------------------------------------*/

/* Private structures ---------------------------------------------------------*/

/* Private variables ----------------------------------------------------------*/
struct mosquitto *mosq;

bool g_bConnected = false; //global flag for connection status
pthread_t g_reconnthr = 0;

char topic[128];

char cafile[64];
char certfile[64];
char keyfile[64];
char g_strTLCertSPW[37] = "05155853"; // SSL/TLS private key or pre-shared-key

long lastPublishMqtt_tick = 0;
long now_tick = 0;

/* Public variables -----------------------------------------------------------*/

/* Private function prototypes ------------------------------------------------*/
int init_mosquitto();
void calculate_topic();
void on_publish_callback(struct mosquitto *mosq_local, void *obj, int mid);
void on_connect_callback(struct mosquitto *mosq_local, void *obj, int reason_code);
void on_disconnect_callback(struct mosquitto *mosq_local, void *obj, int rc);
void* thread_reconnect(void* args);
int on_password_check(char *buf, int size, int rwflag, void *userdata);
long GetTickCount(void);
void restart_vpn();
void checkLastPublish();
void *mqtt_management();
int send_mqtt_publish(char *str2send);

/* Private function bodies ----------------------------------------------------*/

//-----------------------------------------------------------------------------
//	StartMqttManagement
//-----------------------------------------------------------------------------
void StartMqttManagement()
{
	// Demone Configurazione Board
	pthread_t ThNTP;

	// create tread
	pthread_create(&ThNTP, NULL, &mqtt_management, NULL);

} 


//-----------------------------------------------------------------------------
//	mqtt_management
//-----------------------------------------------------------------------------
void *mqtt_management()
{
	time_t rawtime;
    init_mosquitto();


    while(true)
    {
        usleep(500000);

		time ( &rawtime );
		if((rawtime % FREQUENCY_PUBLISH_MQTT) == 0)
		{
			char* string2send = build_get_apiario_json();
			if (string2send != NULL)
			{
				send_mqtt_publish(string2send);
				free(string2send);
			}

			sleep(2);
		}
    }
}









/* Callback called when the client knows to the best of its abilities that a
 * PUBLISH has been successfully sent. For QoS 0 this means the message has
 * been completely written to the operating system. For QoS 1 this means we
 * have received a PUBACK from the broker. For QoS 2 this means we have
 * received a PUBCOMP from the broker. */
void on_publish_callback(struct mosquitto *mosq_local, void *obj, int mid)
{
	(void)mosq_local;
	(void)obj;
	(void)mid;
	//printf("Message with mid %d has been published.\n", mid);
}


/* Callback called when the client receives a CONNACK message from the broker. */
void on_connect_callback(struct mosquitto *mosq_local, void *obj, int reason_code)
{
	(void)obj;
	(void)mosq_local;
	
	/* Print out the connection result. mosquitto_connack_string() produces an
	 * appropriate string for MQTT v3.x clients, the equivalent for MQTT v5.0
	 * clients is mosquitto_reason_string().
	 */

	//printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
	if(reason_code != 0)
	{
		g_bConnected = false;
		if(pthread_create(&g_reconnthr, NULL, thread_reconnect, mosq)!=0)
			g_reconnthr = 0;
	}
	else
	{
		/* You may wish to set a flag here to indicate to your application that the
		* client is now connected. */
		g_bConnected = true;
	}
}

void on_disconnect_callback(struct mosquitto *mosq_local, void *obj, int rc)
{

	(void)obj;
	(void)rc;
	(void)mosq_local;

	g_bConnected = false;

	if(g_reconnthr)
	{
		pthread_cancel(g_reconnthr);
		pthread_join(g_reconnthr, NULL);
		g_reconnthr = 0;
	}
	g_bConnected = false;


	if(pthread_create(&g_reconnthr, NULL, thread_reconnect, mosq)!=0)
		g_reconnthr = 0;

}

//----------------------------------------------------
// Reconnect thread body
//----------------------------------------------------
void* thread_reconnect(void* args)
{
	int sleep_time = 10;

	if(args == NULL)
	{
		pthread_exit(0);
		return 0;
	}

	sleep(sleep_time);

	if(!g_bConnected)	
	{
		mosquitto_destroy(mosq);

		init_mosquitto();
	}

	pthread_exit(0);
	return 0;
}


void calculate_topic()
{
	sprintf(topic,"myBeeMonitor/it040tn034");
	//sprintf(topic,"myBeeMonitor/1");
}




/* Public function bodies -----------------------------------------------------*/
int init_mosquitto()
{
	int rc = 0;
    int type_security = 1;
	/* Required before calling other mosquitto functions */
	mosquitto_lib_init();


	/* Create a new client instance.
	 * id = NULL -> ask the broker to generate a client id for us
	 * clean session = true -> the broker should remove old sessions when we connect
	 * obj = NULL -> we aren't passing any of our private data for callbacks
	 */
	mosq = mosquitto_new(NULL, true, NULL);
	if(mosq == NULL){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}

	/* Configure callbacks. This should be done before connecting ideally. */
	mosquitto_connect_callback_set(mosq, on_connect_callback);
	mosquitto_publish_callback_set(mosq, on_publish_callback);
	mosquitto_disconnect_callback_set(mosq, on_disconnect_callback);
		//mosquitto_publish_callback_set(mosq, MQTT_publish_callback);
	// Setup WISECore connection SSL/TLS,
	//   SSLMode=0 disable the SSL/TLS.

	//   SSLMode=2 certificate based SSL/TLS.
	//     If the server you are connecting to requires clients to provide a
	//     certificate, define certfile and keyfile with your client certificate and
	//     private key. If your private key is encrypted, provide a password callback
	//     function or you will have to enter the password at the command line.
	//   SSLMode=2  pre-shared-key based TLS.
	//      If the server you are connecting to provide a pre-shared-key, define the pre-shared-key and an ID with your client.
	//     private key.

	if(type_security == 1)
	{
		mosquitto_username_pw_set(mosq,"sec","s3c");
	}
	else if(type_security == 2)
	{
		mosquitto_tls_insecure_set(mosq, true);

		memset(cafile,0,sizeof(cafile));
		memset(certfile,0,sizeof(certfile));
		memset(keyfile,0,sizeof(keyfile));

		sprintf(cafile,"/data/certificates/vpn-updater/ca_cert.pem");
		sprintf(certfile,"/data/certificates/vpn-updater/board_cert.pem");
		sprintf(keyfile,"/data/certificates/vpn-updater/board_key.pem");

		mosquitto_tls_set(mosq, cafile, NULL, certfile, keyfile, on_password_check);
	}

	/* Connect to test.mosquitto.org on port 1883, with a keepalive of 60 seconds.
	 * This call makes the socket connection only, it does not complete the MQTT
	 * CONNECT/CONNACK flow, you should use mosquitto_loop_start() or
	 * mosquitto_loop_forever() for processing net traffic. */


	while(true)
	{
		rc = mosquitto_connect(mosq, "192.168.1.2",1883, 60);

		if(rc != MOSQ_ERR_SUCCESS)
		{
			//mosquitto_destroy(mosq);
			checkLastPublish();
			sleep(10);
		}
		else{break;}
	}


	/* Run the network loop in a background thread, this call returns quickly. */
	rc = mosquitto_loop_start(mosq);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return 1;
	}

	/* At this point the client is connected to the network socket, but may not
	 * have completed CONNECT/CONNACK.
	 * It is fairly safe to start queuing messages at this point, but if you
	 * want to be really sure you should wait until after a successful call to
	 * the connect callback.
	 * In this case we know it is 1 second before we start publishing.
	 */


	calculate_topic();


	//mosquitto_lib_cleanup();
	return 0;
}

//----------------------------------------------------
// send_mqtt_publish
//----------------------------------------------------
int send_mqtt_publish(char *str2send)
{

	if(g_bConnected == true)
	{
		int ret = mosquitto_publish(mosq, NULL,topic, strlen(str2send), str2send, 2, false);	

		if(ret == MOSQ_ERR_SUCCESS)
			lastPublishMqtt_tick = GetTickCount();
	}
		
	return 0;
}

//----------------------------------------------------
// password check callback function to handle SSL/TLS private password
//----------------------------------------------------
int forzaDisconnessione()
{
	mosquitto_disconnect(mosq);
	return 0;
}


//----------------------------------------------------
// password check callback function to handle SSL/TLS private password
//----------------------------------------------------
int on_password_check(char *buf, int size, int rwflag, void *userdata)
{
	(void)rwflag;
	(void)userdata;


	int length = 0;
	if(!buf)
		return 0;
	length = strlen(g_strTLCertSPW);
	memset(buf, 0, size);
	if(length+1 >= size)
	{
		strncpy(buf,g_strTLCertSPW,size);
		return size;
	}
	else
	{
		strncpy(buf, g_strTLCertSPW, length+1);
		return length;
	}
}

//----------------------------------------------------
// GetTickCount
//----------------------------------------------------
long GetTickCount(void)
{
	struct timespec TimeSpec;
	clock_gettime(CLOCK_MONOTONIC_RAW, &TimeSpec);
	return((long)(TimeSpec.tv_nsec/1000000)+(long)(TimeSpec.tv_sec)*1000);
}

//----------------------------------------------------
// init_lastpublish_ticks
//----------------------------------------------------
void init_lastpublish_ticks()
{
	lastPublishMqtt_tick = GetTickCount();
}


//----------------------------------------------------
// checkLastPublish
//----------------------------------------------------
void checkLastPublish()
{
	long nowTicks = GetTickCount();
	long gap = nowTicks - lastPublishMqtt_tick;

	if(gap >= (SEC_WITHOUT_BROKER_CONNECTION * 1000))
	{
		lastPublishMqtt_tick = GetTickCount();
		restart_vpn();
	}
}


//--------------------------------------
// 	restart_vpn
//--------------------------------------
void restart_vpn()
{
	system("curl -X POST 127.0.0.1:8888/restartVPN -s -d 'json={\"value\":true}' -m 5 > /dev/null");
}

