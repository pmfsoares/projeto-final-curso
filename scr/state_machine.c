#include "user_config.h"
#include "cJSON.h"
#include "dataPacket.h"
#include "detection.h"
#include "state_machine.h"
#include "fifo.h"

#ifdef SIMULACAO
#include <unistd.h>
#include <simulation.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mqtt_simul.h>
#include <mqtt_pal_simul.h>
#include <time.h>
FILE * csv_file;

void publish_callback(void** unused, struct mqtt_response_publish *published){
  /* Not used*/
}
void* client_refresher(void* client){
    while(1) 
    {
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

struct mqtt_client client;
const char* addr;
const char* port;
const char* topic;
const char* client_id = "esp8266";
int sockfd;
uint8_t sendbuf[MQTT_BUF_SIZE];
uint8_t recvbuf[MQTT_BUF_SIZE];

time_t t;
#endif

#ifndef SIMULACAO

#include <ets_sys.h>
#include <driver/uart.h>
#include <osapi.h>
#include <mqtt.h>
#include <user_interface.h>
#include <mem.h>
#include <mqtt.h>
#endif



void state_engine(states_t state){
  fifo_t samples_fifo;
  samples_fifo = fifo_create(5, sizeof(double) * num_samples);
  /**
   * @brief Local variables definitions & declarations
   * 
   */
  bool file_status = true, finish = false;
  
  
  int t = 2;
  uint16_t k = 0;
  int total = 0;
  char* ss;
  int rec_more = 0;
  double sample_arr[num_samples], power_arr[num_samples/2], th, tmp_arr [num_samples*t];
  while(1){
    switch(state){
      case(SETUP):{
        printf("\nSETUP");
        #ifdef SIMULACAO
          csv_file = openFile("samples.csv");
          
          addr = "192.168.1.80";
          port = "1883";
          topic = "sensor/esp8266";
          sockfd = open_nb_socket(addr, port);

          mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback);
          /* Ensure we have a clean session */
          uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
          /* Send connection request to the broker. */
          mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);
          
          checkError(client.error);
          
          /* start a thread to refresh the client (handle egress and ingree client traffic) */
          pthread_t client_daemon;
          if(pthread_create(&client_daemon, NULL, client_refresher, &client)) {
              fprintf(stderr, "Failed to start client daemon.\n");
          }
        #endif

        #ifndef SIMULACAO
          wifi_init();
        #endif
        state = IDLE;
        break;
      }
      case(IDLE):{
        #ifdef SIMULACAO
          int s;
          s = readSamples(csv_file, sample_arr);
          total += s;
          if(s < num_samples){
            file_status = false;
          }
          fifo_add(samples_fifo, sample_arr);
        #endif
        #ifndef SIMULACAO


        #endif
        if(rec_more == 1){
          state = RECORDING;
        }else if(rec_more == 0){
          state = PRE_RECORDING;
        }
        break;
      }
      case(PRE_RECORDING):{
        k = powerSequence(&sample_arr, &power_arr, k);
        double mean_var, std_var;
        mean_var = mean(power_arr);
        std_var = std(power_arr, mean_var);
        th = threshold(power_arr, mean_var, std_var);

        int noise = 0;
        noise = (power_arr[k] > th ? 1 : 0);
        if(noise == 1){
          state = RECORDING;
        }else if(noise == 0){
          state = IDLE;
        }
        break;
      }
      case(RECORDING):{
        int fifo_sz = samples_fifo->storedbytes/samples_fifo->itemsize;
        int xj=0;
        //printf("%d : Fifo_sz: %d\n", total, fifo_sz);
        if( fifo_sz >= t){
          for(int j=0; j < t; j++){
            double tmp[num_samples];
            fifo_get(samples_fifo, &tmp);
            for(int jj=0; jj < num_samples; jj++, xj++){
              tmp_arr[xj] = tmp[jj];
            }
          }
          rec_more = 0;
          state = TRANSMISSION;
        }else if(fifo_sz < t){
          rec_more = 1;
          state = IDLE;
        }
        break;
      }
      case(TRANSMISSION):{  
          //struct tm tm = *localtime(&t);
          //printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
          printf("Sending.\n");
          ss = create_monitor("d1", 16000, num_samples*t, (long int) time(NULL), tmp_arr);
          mqtt_publish(&client, topic, ss, strlen(ss) + 1, MQTT_PUBLISH_QOS_2);
          //printf("%s\n\n", ss);
          sleep(5);
          if(client.error != MQTT_OK){
            printf("\nErro: %s\n", mqtt_error_str(client.error));
          }
        state = IDLE;
        #ifdef SIMULACAO
        if(!file_status){
          finish = true;
        }
        #endif
        break;
      }
    }
    #ifdef SIMULACAO
      if(finish){
        printf("Ending\n");
        break;
      }
    #endif
  }
}
