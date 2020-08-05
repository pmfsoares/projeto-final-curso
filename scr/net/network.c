#include "network.h"

#include <c_types.h>
#include <osapi.h>
#include <user_interface.h>
#include <ets_sys.h>
#include <gpio.h>

LOCAL void ICACHE_FLASH_ATTR udp_tx_data(uint8_t *data, uint16_t len, uint32_t ip_addr){
  ip_addr_t addr;
  IP4_ADDR(&addr, 192, 168, 1, 80);
  os_memcpy(udp_proto_tx.remote_ip, &addr.addr, 4);
  udp_proto_tx.remote_port = 1234;
  os_printf("\ndest.ip = "IPSTR"\n", IP2STR(&addr.addr));
  udp_tx.type = ESPCONN_UDP;
  udp_tx.state = ESPCONN_NONE;
  udp_tx.proto.udp = &udp_proto_tx;

  if(espconn_create(&udp_tx)){
    os_printf("espconn_create failed\n");
  }
  if(espconn_send(&udp_tx, data, len)){ 
    os_printf("espconn_sendto failed\n");
  }
  espconn_delete(&udp_tx);
}