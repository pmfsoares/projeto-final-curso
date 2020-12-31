/**
 * @file user_config.h
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief Sensor configuration file, WIFI credecentials, e broker ip come in here
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __MQTT_CONFIG_H__
#define __MQTT_CONFIG_H__

#define MQTT_SSL_ENABLE

/*DEFAULT CONFIGURATIONS*/


#define MQTT_HOST     "192.168.12.1" //or "mqtt.yourdomain.com"
#define MQTT_PORT     "1883"
#define MQTT_PORT_ESP 1883
#define MQTT_KEEPALIVE    120  /*second*/
#define MQTT_USER     "USER"
#define MQTT_PASS     "PASS"
#define MQTT_CLEAN_SESSION 1
#define MQTT_KEEPALIVE 120
#define MQTT_RECONNECT_TIMEOUT  5 /*second*/
#define DEFAULT_SECURITY  0
#define QUEUE_BUFFER_SIZE       2048

//#define PROTOCOL_NAMEv31  /*MQTT version 3.1 compatible with Mosquitto v0.15*/
#define PROTOCOL_NAMEv311     /*MQTT version 3.11 compatible with https://eclipse.org/paho/clients/testing/*/


#define MQTT_DEBUG_ON
/**
 * @brief 
 * 
 */
#if defined(DEBUG_ON)
#define INFO( format, ... ) os_printf( format, ## __VA_ARGS__ )
#else
#define INFO( format, ... )
#endif


/**
 * Application Settings
 * 
 */

/**
 * @brief MAC Octet defines
 * 
 */
//d1 7B:36:BD:13:ED:17
//d2 3D:81:D2:D5:77:14
//d3 D7:16:56:9F:FA:3B
//d4 71:C3:C2:A6:88:CE
#define MAC_OCTET_1 "7B" 
#define MAC_OCTET_2 "36"
#define MAC_OCTET_3 "BD"
#define MAC_OCTET_4 "1E"
#define MAC_OCTET_5 "ED"
#define MAC_OCTET_6 "17"

//#define SIMULATION


/**
 * @brief ifdef to 
 * 
 */
#ifdef SIMULATION
#define MQTT_BUF_SIZE  340000
#endif
#ifndef SIMULATION
#define MQTT_BUFFER_SIZE 1024
#define MQTT_BUF_SIZE 1024
#endif

#define MQTT_CLIENT_ID  MAC_OCTET_1 ":" MAC_OCTET_2 ":" MAC_OCTET_3 ":" MAC_OCTET_4 ":" MAC_OCTET_5 ":" MAC_OCTET_6
#define MQTT_CLIENT_UNIQUE_TOPIC  MAC_OCTET_1 "/" MAC_OCTET_2 "/" MAC_OCTET_3 "/" MAC_OCTET_4 "/" MAC_OCTET_5 "/" MAC_OCTET_6 "/"


#define MQTT_TOPIC_MAIN_PRE     "sensor/" MQTT_CLIENT_UNIQUE_TOPIC   
#define MQTT_TOPIC_CONFIG_SUFF  "config"
#define MQTT_TOPIC_REGISTRATION_SUFF "registration"
#define MQTT_TOPIC_ACK_SUFF     "event/ack"
#define MQTT_TOPIC_ACK_CONF_SUFF "config/ack"
#define MQTT_TOPIC_EVENT_SUFF   "event/data"
#define MQTT_TOPIC_BINARY_SUFF "event/binary"
#define MQTT_TOPIC_DONE_SUFF "event/done"

#define MQTT_TOPIC_EVENT       MQTT_TOPIC_MAIN_PRE MQTT_TOPIC_EVENT_SUFF
#define MQTT_TOPIC_CONFIG   MQTT_TOPIC_MAIN_PRE MQTT_TOPIC_CONFIG_SUFF
#define MQTT_TOPIC_REGISTRATION MQTT_TOPIC_MAIN_PRE MQTT_TOPIC_REGISTRATION_SUFF
#define MQTT_TOPIC_ACK     MQTT_TOPIC_MAIN_PRE MQTT_TOPIC_ACK_SUFF
#define MQTT_TOPIC_ACK_CONFIG MQTT_TOPIC_MAIN_PRE MQTT_TOPIC_ACK_CONF_SUFF
#define MQTT_TOPIC_BINARY MQTT_TOPIC_MAIN_PRE MQTT_TOPIC_BINARY_SUFF
#define MQTT_TOPIC_DONE MQTT_TOPIC_MAIN_PRE MQTT_TOPIC_DONE_SUFF

#define num_samples_define 4000

//WiFi network credentials
#define AP_SSID "WSN_Projeto"
#define AP_PASSWORD "a123456789"

#endif // __MQTT_CONFIG_H__
