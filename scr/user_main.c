#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "lwip/ip_addr.h"
#include "user_interface.h"
#include "espconn.h"
#include "mqtt.h"
#include <math.h>
#include <mem.h>

#include <estados.h>

static const int pin = 2;
static const int pin_externo = 5;
static volatile os_timer_t some_timer;

MQTT_Client* mqttClient;

LOCAL uint32_t last_addr = 0;

static void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args){
  MQTT_Client* client = (MQTT_Client*) args;
  MQTT_PUBLISH(client, "/projeto/sensor/esp8266", "TestePublish 1", 6, 0, 0);
  MQTT_PUBLISH(client, "/projeto/sensor/esp8266", "TestePublish 2", 6, 1, 0);
  MQTT_PUBLISH(client, "/projeto/sensor/esp8266", "TestePublish 3", 6, 2, 0);
}
static void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args){
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Disconnected\r\n");
}

static void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args){
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Published\r\n");
}

static void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len){

}
void some_timerfunc(void *arg){

  //Do blinky stuff
  if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & (1 << pin)){
    // set gpio low
    gpio_output_set(0, (1 << pin), 0, 0);
  }
  else{
    // set gpio high
    gpio_output_set((1 << pin), 0, 0, 0);
  }
}

void ICACHE_FLASH_ATTR user_init(){
  // init gpio subsytem
  gpio_init();

  // configure UART TXD to be GPIO1, set as output
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1); 
  gpio_output_set(0, 0, (1 << pin), 0);
  

  /* MQTT SETUP
  MQTT_InitConnection(&mqttClient, MQTT_HOST, MQTT_PORT, DEFAULT_SECURITY);

  if ( !MQTT_InitClient(&mqttClient, MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS, MQTT_KEEPALIVE, MQTT_CLEAN_SESSION) ) {
    INFO("Failed to initialize properly. Check MQTT version.\r\n");
    return;
  }
  //MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);
  MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
  MQTT_OnConnected(&mqttClient, mqttConnectedCb);
  MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
  MQTT_OnPublished(&mqttClient, mqttPublishedCb);
  MQTT_OnData(&mqttClient, mqttDataCb);
  */




  // setup timer (500ms, repeating)
  os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);
  os_timer_arm(&some_timer, 1000, 1);
}
