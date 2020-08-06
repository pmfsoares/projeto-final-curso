#ifndef __MQTT_CONFIG_H__
#define __MQTT_CONFIG_H__

#define MQTT_SSL_ENABLE

/*DEFAULT CONFIGURATIONS*/

#define MQTT_HOST     "test.mosquitto.org" //or "mqtt.yourdomain.com"
#define MQTT_PORT     1883
#define MQTT_BUF_SIZE   1024
#define MQTT_KEEPALIVE    120  /*second*/
#define MQTT_CLIENT_ID    "sensor_mic"
#define MQTT_USER     "USER"
#define MQTT_PASS     "PASS"
#define MQTT_CLEAN_SESSION 1
#define MQTT_KEEPALIVE 120
#define MQTT_RECONNECT_TIMEOUT  5 /*second*/
#define DEFAULT_SECURITY  0
#define QUEUE_BUFFER_SIZE       2048

#define PROTOCOL_NAMEv31  /*MQTT version 3.1 compatible with Mosquitto v0.15*/
//PROTOCOL_NAMEv311     /*MQTT version 3.11 compatible with https://eclipse.org/paho/clients/testing/*/

#if defined(DEBUG_ON)
#define INFO( format, ... ) os_printf( format, ## __VA_ARGS__ )
#else
#define INFO( format, ... )
#endif


/**
 * Application Settings
 * 
 */
#define num_samples 4000
#define AP_SSID "Soares Wifi"
#define AP_PASSWORD "gjY5qDkQD3"



#endif // __MQTT_CONFIG_H__