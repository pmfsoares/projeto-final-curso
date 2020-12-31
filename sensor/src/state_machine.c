/**
 * @file state_machine.c
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief Main file of the firmware, has the state_machine
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "state_machine.h"
#include "user_config.h"
#include "cJSON.h"
#include "detection.h"
#include "fifo.h"
#include "dataPacket.h"
#include <stdlib.h>


#ifdef SIMULATION
  #include <unistd.h>
  #include <simulation.h>
  #include <stdio.h>
  #include <stdbool.h>
  #include <mqtt_simul.h>
  #include <mqtt_pal_simul.h>
  #include <time.h>
  #include <stdint.h>
  #include <posix_sockets.h>
#endif
#ifndef SIMULATION
  #include <ets_sys.h>
  #include <driver/uart.h>
  #include <osapi.h>
  #include "network.h"
  #include <mqtt.h>
  #include <user_interface.h>
  #include <mem.h>
  #include <mqtt.h>
  
static inline int32_t asm_ccount(void){
  int32_t r; asm volatile("rsr %0, ccount" : "=r"(r)); return r;} 
#endif

#ifdef SIMULATION
void mqttDataCb(void** unused, struct mqtt_response_publish *published);
#endif
#ifndef SIMULATION
void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len);
#endif


#ifdef SIMULATION
/**
 * @brief Function to check for MQTT client errors
 * 
 * @param error mqtt_client.error 
 */

void checkError(enum MQTTErrors error);
/**
 * @brief Samples file pointer
 * 
 */
FILE * csv_file;

/**
 * @brief MQTT client variable
 * 
 */
struct mqtt_client client;

/**
 * @brief Socket descriptor 
 * 
 */
int sockfd;

/**
 * @brief MQTT send and recv buffers of size MQTT_BUF_SIZE
 * 
 */
uint8_t sendbuf[MQTT_BUF_SIZE];
uint8_t recvbuf[MQTT_BUF_SIZE];
/**
 * @brief Retry thread variable
 * 
 */

pthread_t retry_daemon;

/**
 * @brief Total number of lines currently read 
 * 
 */
int total = 0;
#endif
/**
 * @brief General variables
 * 
 */
int ready = 0, setup_done = 0, next = 0, retry_count = 0;


/**
 * @brief Global variable containing the current state of the state machine
 * 
 */
states_t general_state;
/**
 * @brief Declaration of the pointers used for storing samples and power sequence
 * 
 */
double* sample_ptr = NULL;
double* power_ptr = NULL;
double* tmp_arr = NULL;

/**
 * @brief Samples FIFO declaration, initialized with NULL, and inited on IDLE state
 * 
 */
fifo_t samples_fifo = NULL;


/**
 * @brief Declaration of number of samples per block, and sample rate
 * 
 */
uint16_t num_samples, sample_rate;

/**
 * @brief Number of sample blocks to save
 * 
 */
int num_blocks = 1;

/**
 * @brief Sample pointer current position
 * 
 */
int ptr_byte_pos = 0, pos = 0, ready_recv = 1;

#ifndef SIMULATION

int time, run_count = 0;
double test;
static os_timer_t val_timer, byte_timer, resp_timer;
int32_t btime, etime;
responses_t resp;

/**
 * @brief Function that disarms the timer that requests a value
 * 
 */
void end_f(void){
  os_timer_disarm(&val_timer);
  ready = 2;
  pos = 0;
  os_printf("Disarmed timer\n");
  general_state = IDLE;
  state_engine(general_state);        
}
/**
 * @brief Function that disarms the timer related to the response waiting timer
 * 
 */
void end_ptr_byte_timer(void){
  os_timer_disarm(&byte_timer);
  turnOnTimer(BINARY_ACK);
  char* end = "finish";
  MQTT_Publish(&mqttClient, MQTT_TOPIC_DONE, end, strlen(end), 2, 0);
  /*
  int32_t time =((int32_t) (etime-btime)) ;
  char* time_str = create_time(MQTT_CLIENT_ID, btime, etime, time, num_samples);
  MQTT_Publish(&mqttClient, "timing/", time_str, strlen(time_str), 2, 0);
  */
}

/**
 * @brief Publish byte by byte array function
 * 
 * @param arg 
 */
void ptr_byte_timer(void *arg){
  if(ptr_byte_pos < (sizeof(double) * num_samples)-8){
    unsigned char* byte_ptr = sample_ptr;
    MQTT_Publish(&mqttClient, MQTT_TOPIC_BINARY, byte_ptr+ptr_byte_pos, 8, 2, 0);
    os_printf("[%d]Ptr_pos\n", ptr_byte_pos);
    ptr_byte_pos += 8;
    return;
  }else{
    end_ptr_byte_timer();
  }
}
/**
 * @brief Get the val timer object
 * 
 * @param arg 
 */
void get_val_timer(void *arg){
    os_printf("val\n");
    if(pos >= num_samples){
      os_printf("end\n");
      end_f();
    }
}
/**
 * @brief Function to verify that the device recieves server answer
 * 
 * @param arg 
 */
void resend_timer(void *arg){
  switch(resp){
    case(CONFIGURATION):{
      char* ss_config = (char *) create_monitor_sensor_config("d4", MQTT_CLIENT_ID);
      MQTT_Publish(&mqttClient, MQTT_TOPIC_REGISTRATION, ss_config, strlen(ss_config), 2, 0);
      return;
    }
    case(EVENT_ACK):{
      char* ss = create_monitor_without_arr("d4", sample_rate, num_samples, time);
      MQTT_Publish(&mqttClient, MQTT_TOPIC_EVENT, ss, strlen(ss), 2, 0);
      return;
    }
    case(BINARY_ACK):{
      if(retry_count = 0){
        retry_count++;
        return; 
      }else if(retry_count > 0){
        ptr_byte_pos = 0;
        os_timer_setfn(&byte_timer, (os_timer_func_t *)ptr_byte_timer, NULL);
        os_timer_arm(&byte_timer, 100, 1);
      }
      return;
    }
  }
}
/**
 * @brief Function to turn on the waiting timer
 * 
 * @param r 
 */
void turnOnTimer(responses_t r){
  resp = r;
  os_timer_setfn(&resp_timer, (os_timer_func_t *)resend_timer, NULL);
  os_timer_arm(&resp_timer, 15000, 1);
}
#endif
int power_ptr_pos = 0, rec_more = 0;

/**
 * @brief Main function of the state machine
 * 
 * @param state Initial state
 */
#ifdef SIMULATION
void state_engine(states_t state){
#endif
#ifndef SIMULATION
void ICACHE_FLASH_ATTR state_engine(states_t state){
#endif 
  

  /**
   * @brief Local variables definitions & declarations
   * 
   */
  bool file_end = true, finish = false;
  
  /**
   * @brief Latest power_ptr position
   * 
   */
  while(1){
    
    switch(state){
      case(SETUP):{
        #ifdef SIMULATION
          printf("TESTE: %s\n", MQTT_CLIENT_ID);
          csv_file = openFile("samples.csv");
          MQTT_setup();
          MQTT_init();
        #endif
        #ifndef SIMULATION
          if(setup_done == 0){
            os_printf("Setting up mqtt\n");
            MQTT_setup();
            wifi_init();
            //wifi_set_opmode(STATION_MODE);
            setup_done = 1;
            return;
          }
        #endif
        state = IDLE;
        break;
      }
      case(IDLE):{
        //os_printf("On IDLE\n\n");
        #ifdef SIMULATION
          int s;
          if(total == 0){
            s = readSamples(csv_file, sample_ptr, 1, num_samples);
          }else if(total != 0){
            s = readSamples(csv_file, sample_ptr, 2, num_samples);
          }
          
          total += s;
          if(s < num_samples){
            file_end = false;
          }
          if(samples_fifo == NULL){
            samples_fifo = fifo_create(num_blocks+1, sizeof(double) * num_samples);
          }
          fifo_add(samples_fifo, sample_ptr);
        #endif
        #ifndef SIMULATION
          if(ready == 1){
            os_timer_setfn(&val_timer, (os_timer_func_t *)get_val_timer, NULL);
            os_timer_arm(&val_timer, 5, 1);
            return;
          }else if(ready == 2){
            os_printf("Allocing to fifo\n");
            ready = 1;
          }
        #endif
        if(rec_more == 1){
          state = RECORDING;
        }else if(rec_more == 0){
          state = PRE_RECORDING;
        }
       break; 
      }
      case(PRE_RECORDING):{
        power_ptr[power_ptr_pos] = powerSequence(sample_ptr, power_ptr, power_ptr_pos, num_samples);
        power_ptr_pos++;
        //power_ptr_pos = powerSequence(sample_ptr, power_ptr, power_ptr_pos, num_samples);
        double mean_var, std_var, th_var;
        mean_var = mean(power_ptr, power_ptr_pos);
        std_var = std(power_ptr, mean_var, power_ptr_pos);
        th_var = threshold(mean_var, std_var);
        int noise = -1;
        #ifndef SIMULATION

        /*
        for(int x=0; x < power_ptr_pos; x++){
          os_printf("Power[%d]: ", x);
          printDouble(power_ptr[x]);
        }
        os_printf("Mean: ");
        printDouble(mean_var);
        os_printf("Std: ");
        printDouble(std_var);
        os_printf("Th: ");
        printDouble(th_var);
        */
        #endif
       // noise = threshold_check(power_ptr, power_ptr_pos, num_samples);
        noise = (power_ptr[power_ptr_pos] > th_var ? 1 : 0);
        #ifdef SIMULATION
        if(noise == 1){
          state = RECORDING;
        }else if(noise == 0){
          state = IDLE;
        }
        #endif
        #ifndef SIMULATION
        if(noise == 1){
          state = RECORDING;
        }else if(noise == 0){
          state = IDLE;
        }
        #endif
        break;
      }
      case(RECORDING):{
        #ifdef SIMULATION
        uint8_t fifo_sz = samples_fifo->storedbytes/samples_fifo->itemsize;
        #endif
        #ifndef SIMULATION
        uint8_t fifo_sz = 2;//samples_fifo->storedbytes/samples_fifo->itemsize;
        #endif
        uint8_t xj=0;
        if( fifo_sz >= num_blocks){
          #ifdef SIMULATION
          if(tmp_arr == NULL){
              tmp_arr = malloc((num_samples * num_blocks) * sizeof(*tmp_arr));
              /**
              #ifndef SIMULATION
              tmp_arr = os_malloc((num_samples * num_blocks) * sizeof(*tmp_arr));
              #endif
          }
          os_printf("Heap: %d\n", system_get_free_heap_size());
          *
           * @brief Loop to join the desired sample blocks
           * 
           * 
           */
          }
          for(uint8_t j=0; j < num_blocks; j++){
            double tmp[num_samples];
            fifo_get(samples_fifo, &tmp);
            for(uint16_t jj=0; jj < num_samples; jj++, xj++){
              tmp_arr[xj] = tmp[jj];
            }
          }
          #endif
          rec_more = 0;
          state = TRANSMISSION;
        }else if(fifo_sz < num_blocks){
          rec_more = 1;
          state = IDLE;
        }
        break;
      }
      case(TRANSMISSION):{  
          char* ss;
          #ifdef SIMULATION
            ss = create_monitor("d3", 16000, num_samples*num_blocks, (long int) time(NULL), tmp_arr);
            mqtt_publish(&client, MQTT_TOPIC_EVENT, ss, strlen(ss) + 1, MQTT_PUBLISH_QOS_2);
            
            printf("\n");
            sleep(3);
            if(client.error != MQTT_OK){
              printf("\nErro: %s\n", mqtt_error_str(client.error));
            }
          #endif
          #ifndef SIMULATION
            os_printf("On transmission state\n");
            //ss = create_monitor_bytes("d4", 16000, num_samples*num_blocks, 1601841883, tmp_arr);
            time = sntp_get_current_timestamp();
            ss = create_monitor_without_arr(MQTT_CLIENT_ID, sample_rate, num_samples, time);
            MQTT_Publish(&mqttClient, MQTT_TOPIC_EVENT, ss, strlen(ss), 2, 0);
            //turnOnTimer(EVENT_ACK);
            //fifo_get(samples_fifo, &tmp_arr);
            btime = system_get_time();
            os_timer_setfn(&byte_timer, (os_timer_func_t *)ptr_byte_timer, NULL);
            os_timer_arm(&byte_timer, 100, 1);
            /*
            unsigned char* ptr = sample_ptr;
            for(ptr_byte_pos=0; ptr_byte_pos < sizeof(double) ; ptr_byte_pos++){
              MQTT_Publish(&mqttClient, "binary/data", ptr+ptr_byte_pos, 1, 2, 0);
            }*/
            return;
          #endif
        state = IDLE;
        #ifdef SIMULATION
        if(!file_end){
          finish = true;
        }
        #endif
        break;
      }
    }
    #ifdef SIMULATION
      if(finish){
        printf("Ending\n");
        break;
      }
    #endif
  }
}
/**
 * @brief Function to allocate/reallocate memory to the sample/power pointers
 * 
 */
void allocMem(){

    if(sample_ptr == NULL){
      #ifdef SIMULATION
      sample_ptr = malloc(num_samples  * sizeof(*sample_ptr) );
      #endif
      #ifndef SIMULATION
      sample_ptr = os_malloc(num_samples  * sizeof(*sample_ptr) );
      #endif
    }else if(sample_ptr != NULL){
      #ifdef SIMULATION
        realloc(sample_ptr, num_samples * sizeof(*sample_ptr) );
      #endif
      #ifndef SIMULATION
        os_realloc(sample_ptr, num_samples * sizeof(*sample_ptr) );
      #endif
    }
    if(power_ptr == NULL){
      #ifdef SIMULATION
        power_ptr = malloc(100 * sizeof(*power_ptr));
      #endif
      #ifndef SIMULATION
        power_ptr = os_malloc(100 * sizeof(*power_ptr));
      #endif
    }else if(power_ptr != NULL){
      #ifdef SIMULATION
        realloc(power_ptr, 100 * sizeof(*power_ptr) );
      #endif
      #ifndef SIMULATION
        os_realloc(power_ptr, 100 * sizeof(*power_ptr) );
      #endif
    }
}

/**
 * @brief Message handler, recieves data and topic, parses json for the content
 * 
 * @param topic MQTT Topic 
 * @param data Message
 * @return int Returns 1 if sucessufull, -1 if unsucessfull
 */
int handleMessage(char* topic, char* data){
  #ifndef SIMULATION
  os_timer_disarm(&resp_timer);
  os_printf("%s\n%s\n", topic, data);
  #endif
  switch(check_topic(topic)){
    case CONFIG:{
      sensor_config tmp_conf = parse_config(data);
      num_samples = tmp_conf.sample_size;
      sample_rate = tmp_conf.sample_rate;
      if(num_samples != NULL && sample_rate != NULL){
        allocMem(); 
        #ifndef SIMULATION
          os_printf("N: %d\nRt: %d\n", num_samples, sample_rate);
          general_state = IDLE;
          ready = 1;
          char* ack_s = "ok";
          MQTT_Publish(&mqttClient, MQTT_TOPIC_ACK_CONFIG, ack_s , strlen(ack_s), 2, 0);
          state_engine(general_state);
        #endif
        return 1;
      }else{
        return -1;
      }
      break;
    }
    case ACK_EVENT:{
      if(ptr_byte_pos != 0 || ptr_byte_pos == 0){
        os_printf("Received ack\n");
        ptr_byte_pos = 0;
        pos = 0;
        ready = 1;
        general_state = IDLE;
        state_engine(general_state);
      }
      break;
    }
  }
}

#ifdef SIMULATION
/**
 * @brief Callback function for when the device receives a mqtt message(simulation only)
 * 
 * @param unused 
 * @param published MQTT
 */
void mqttDataCb(void** unused, struct mqtt_response_publish *published){
    /* note that published->topic_name is NOT null-terminated (here we'll change it to a c-string) */
    printf("On pub callback");
    char* topic_name = (char*) malloc(published->topic_name_size + 1);
    memcpy(topic_name, published->topic_name, published->topic_name_size);
    topic_name[published->topic_name_size] = '\0';
    handleMessage(topic_name, (char*) published->application_message);
    free(topic_name);
}
#endif

#ifndef SIMULATION
/**
 * @brief Callback function for when the device receives a mqtt message(non-simulation only)
 * 
 * @param args 
 * @param topic MQTT message topic
 * @param topic_len Topic length
 * @param data MQTT message content
 * @param data_len Message length
 */
void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len){
    os_printf("Recebi uma mensagem\n");
    char *topicBuf = (char*)os_zalloc(topic_len + 1),
        *dataBuf = (char*)os_zalloc(data_len + 1);
    os_memcpy(topicBuf, topic, topic_len);
    topicBuf[topic_len] = 0;
    os_memcpy(dataBuf, data, data_len);
    dataBuf[data_len] = 0;
    handleMessage(topicBuf, dataBuf);
}
#endif


#ifdef SIMULATION

/**
 * @brief Daemon for 
 * 
 * @param client 
 * @return void* 
 */
void* retry_daemon_func(void* client){
  if(retry_count < 0){

  }
}

/**
 * @brief MQTT client synchronizer which checks for new MQTT messages
 * 
 * @param client mqtt_client to check for new messages
 * @return void* 
 */
void* client_refresher(void* client){
    while(1){
        mqtt_sync((struct mqtt_client*) client);
        usleep(100000U);
    }
    return NULL;
}
/**
 * @brief Function that check if there are errors in the client.
 * @param MQTTErrors
 * @return (void)
 */
void checkError(enum MQTTErrors error){
  if(error != MQTT_OK){
    printf("error: %s\n", mqtt_error_str(error));
  }
};
#endif

/**
 * @brief MQTT setup function, creates the client, and configures callbacks and connection
 * 
 */
#ifdef SIMULATION
void MQTT_setup(void ){
#endif
#ifndef SIMULATION
void ICACHE_FLASH_ATTR MQTT_setup(void ){
#endif

  #ifdef SIMULATION
  sockfd = open_nb_socket(MQTT_HOST, MQTT_PORT);

  mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), mqttDataCb);
  /* Ensure we have a clean session */
  #endif

  #ifndef SIMULATION
  MQTT_InitConnection(&mqttClient, MQTT_HOST, MQTT_PORT_ESP, DEFAULT_SECURITY);
  if ( !MQTT_InitClient(&mqttClient, MQTT_CLIENT_ID, NULL, NULL, MQTT_KEEPALIVE, MQTT_CLEAN_SESSION) ) {
    os_printf("Failed to initialize properly. Check MQTT version.\r\n");
    return;
  }
  //MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);
  MQTT_OnConnected(&mqttClient, mqttConnectedCb);
  MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
  MQTT_OnPublished(&mqttClient, mqttPublishedCb);
  MQTT_OnData(&mqttClient, mqttDataCb);
  #endif

}
/**
 * @brief MQTT initializer function, uses the already created client, and connects to the broker
 * 
 */
#ifdef SIMULATION
void MQTT_init(void){
#endif
#ifndef SIMULATION
void ICACHE_FLASH_ATTR MQTT_init(void){
#endif
  #ifdef SIMULATION
    uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    /* Send connection request to the broker. */
    mqtt_connect(&client, MQTT_CLIENT_ID, NULL, NULL, 0, NULL, NULL, connect_flags, 400);
    checkError(client.error);
  
    /* start a thread to refresh the client (handle egress and ingree client traffic) */
    pthread_t client_daemon;
    if(pthread_create(&client_daemon, NULL, client_refresher, &client)) {
        fprintf(stderr, "Failed to start client daemon.\n");
    }
    mqtt_subscribe(&client, MQTT_TOPIC_CONFIG, MQTT_PUBLISH_QOS_2);
    char* ss_config = (char *) create_monitor_sensor_config("d3", MQTT_CLIENT_ID);
    printf("%s\n", ss_config);
    mqtt_publish(&client, MQTT_TOPIC_REGISTRATION, ss_config, strlen(ss_config) + 1, MQTT_PUBLISH_QOS_2);
    if(pthread_create(&retry_daemon, NULL, retry_daemon_func, &client)){
      fprintf(stderr, "Failed to start client daemon.\n");
    }
    sleep(4);
  #endif
  #ifndef SIMULATION



  #endif
  
}

#ifndef SIMULATION
/**
 * @brief Callback funtion for when the device connects to the MQTT broker
 * 
 * @param args mqttClient
 */
void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args){
  MQTT_Client* client = (MQTT_Client*) args;
  if(ready == 0){
    MQTT_Subscribe(client, MQTT_TOPIC_CONFIG, 2);
    MQTT_Subscribe(client, MQTT_TOPIC_ACK, 2);
    char* ss_config = (char *) create_monitor_sensor_config("d4", MQTT_CLIENT_ID);
    turnOnTimer(CONFIGURATION);
    os_printf("JSON: \n%s\n", ss_config);
    MQTT_Publish(&mqttClient, MQTT_TOPIC_REGISTRATION, ss_config, strlen(ss_config), 2, 0);  
  }
}
/**
 * @brief Callback function for when the device disconnects from the MQTT broker
 * 
 * @param args mqttClient
 */
void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args){
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Disconnected\r\n");
}

void ICACHE_FLASH_ATTR printDouble(double num){
  unsigned char* tmp = &num;
  for(int i=0; i < sizeof(double); i++){
    os_printf("%02X", *(tmp+i));
  }
  os_printf("\n");
}

/**
 * @brief Callback function for uart communications
 * 
 * @param events 
 */
void uart_rx_task(os_event_t *events) {
    if (events->sig == 0) {
        // Sig 0 is a normal receive. Get how many bytes have been received.
        uint64_t rx_len = (READ_PERI_REG(UART_STATUS(UART0)) >> UART_RXFIFO_CNT_S) & UART_RXFIFO_CNT;
        // Parses the received bytes and stores them as such in an array.
        unsigned char b_arr[rx_len];
        for (uint8_t ii=0; ii < rx_len; ii++) {
            b_arr[ii] = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;
        }
        if(ready != 2) {
          //converts the receives bytes to a double
          //test = unpackDouble(b_arr);
          os_memcpy(&test, b_arr, sizeof(double));
          sample_ptr[pos] = test;
          pos++;
        }

        // Clear the interrupt condition flags and re-enable the receive interrupt.
        WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR | UART_RXFIFO_TOUT_INT_CLR);
        uart_rx_intr_enable(UART0);
    }
}
#endif

