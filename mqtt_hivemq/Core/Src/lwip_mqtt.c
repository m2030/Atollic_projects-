#include "lwip/apps/mqtt.h"
#include <string.h>
#include "stm32f7xx_hal.h"
  ip_addr_t mqttServerIP;

extern UART_HandleTypeDef huart3;
char buffer[1000];
/* The idea is to demultiplex topic and create some reference to be used in data callbacks
   Example here uses a global variable, better would be to use a member in arg
   If RAM and CPU budget allows it, the easiest implementation might be to just take a copy of
   the topic string and use it in mqtt_incoming_data_cb
*/
static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
  sprintf(buffer,"Incoming publish at topic %s with total length %u\n\r", topic, (unsigned int)tot_len);
HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);
  /* Decode topic string into a user defined reference */
  if(strcmp(topic, "print_payload") == 0) {
    inpub_id = 0;
  } else if(topic[0] == 'A') {
    /* All topics starting with 'A' might be handled at the same way */
    inpub_id = 1;
  } else if(strcmp(topic, "hello_world") == 0) {
	  inpub_id = 2;
  } else if(strcmp(topic, "hello_worldq") == 0) {
	 inpub_id = 3;
  }
  else {
    /* For all other topics */
    inpub_id = 9;
  }

}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
	  sprintf(buffer,"Incoming publish payload with length %d, flags %u\n\r", len, (unsigned int)flags);
	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);
	  sprintf(buffer,"mqtt_incoming_data_cb: %s\n\r", (const char *)data);
	   HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);
  if(flags & MQTT_DATA_FLAG_LAST) {


    if(inpub_id == 0) {
      /* Don't trust the publisher, check zero termination */
      if(data[len-1] == 0) {
    	  sprintf(buffer,"mqtt_incoming_data_cb: %s\n\r", (const char *)data);
    	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

      }
    } else if(inpub_id == 1) {
      /* Call an 'A' function... */
    } else if(inpub_id == 2) {
    	if(strcmp(data, "0") == 0){ //Then, turn off LED3
  //  		Board_LED_Set(LEDS_LED3, true);
    	}else if (strcmp(data, "1" == 0)){ //Then turn on LED3
    //		Board_LED_Set(LEDS_LED3, false);
    	}
    	/* Call an 'A' function... */
    } else if(inpub_id == 3) {
/* Not yet done. It's suppossed to turn on/off */
    	if(strcmp(data, "0") == 0){ //Then, turn off LED2
    	//      Board_LED_Set(LEDS_LED1, true);
    	  // 	  Board_LED_Set(LEDS_LED2, true);
    	   	//  Board_LED_Set(LEDS_LED3, true);
    	   	  //Board_LED_Set(LEDS_LED4, true);
    		// 	  Board_LED_Set(LEDS_LED5, true);
    		//Board_LED_Set(LEDS_LED6, true);
    		////Board_LED_Set(LEDS_LED7, true);
    		//Board_LED_Set(LEDS_LED8, true);
    	    } else if(strcmp(data, "1") == 0){ //Then turn on LED2
    	    	//Board_LED_Set(LEDS_LED1, false);
    	    	//Board_LED_Set(LEDS_LED2, true);
    	    	//Board_LED_Set(LEDS_LED3, true);
    	    	//   	Board_LED_Set(LEDS_LED4, true);
    	    	//	Board_LED_Set(LEDS_LED5, true);
    	    	//Board_LED_Set(LEDS_LED6, true);
    	    	//Board_LED_Set(LEDS_LED7, true);
    	    	//Board_LED_Set(LEDS_LED8, true);
    	    } else if(strcmp(data, "2") == 0){ //Then turn on LED2
    	    	//Board_LED_Set(LEDS_LED1, false);
    	    	//Board_LED_Set(LEDS_LED2, false);
    	    	//Board_LED_Set(LEDS_LED3, true);
    	    	//Board_LED_Set(LEDS_LED4, true);
    	    	//Board_LED_Set(LEDS_LED5, true);
    	    	//  	Board_LED_Set(LEDS_LED6, true);
    	    	//Board_LED_Set(LEDS_LED7, true);
    	    	//	Board_LED_Set(LEDS_LED8, true);
    	    }
    	    //else if (strcmp(data, "3") == 0){ //Then turn on LED2
    	//    	Board_LED_Set(LEDS_LED1, false);
    	//    	Board_LED_Set(LEDS_LED2, false);
    	//    	Board_LED_Set(LEDS_LED3, false);
    	//    	Board_LED_Set(LEDS_LED4, true);
    	//    	Board_LED_Set(LEDS_LED5, true);
    	//    	Board_LED_Set(LEDS_LED6, true);
    	//    	Board_LED_Set(LEDS_LED7, true);
    	//    	Board_LED_Set(LEDS_LED8, true);
    	//    } else if (strcmp(data, "4") == 0){ //Then turn on LED2
    	//    	Board_LED_Set(LEDS_LED1, false);
    	//    	Board_LED_Set(LEDS_LED2, false);
    	//    	Board_LED_Set(LEDS_LED3, false);
    	//    	Board_LED_Set(LEDS_LED4, false);
    	//    	Board_LED_Set(LEDS_LED5, true);
    	//    	Board_LED_Set(LEDS_LED6, true);
    	//    	Board_LED_Set(LEDS_LED7, true);
    	//    	Board_LED_Set(LEDS_LED8, true);
    	//    }else if (strcmp(data, "5") == 0){ //Then turn on LED2
    	//    	Board_LED_Set(LEDS_LED1, false);
    	//    	Board_LED_Set(LEDS_LED2, false);
    	//    	Board_LED_Set(LEDS_LED3, false);
    	//    	Board_LED_Set(LEDS_LED4, false);
    	//    	Board_LED_Set(LEDS_LED5, false);
    	//    	Board_LED_Set(LEDS_LED6, true);
    	//    	Board_LED_Set(LEDS_LED7, true);
    	//    	Board_LED_Set(LEDS_LED8, true);
    	//    }else if (strcmp(data, "6") == 0){ //Then turn on LED2
    	//    	Board_LED_Set(LEDS_LED1, false);
    	//    	Board_LED_Set(LEDS_LED2, false);
    	//    	Board_LED_Set(LEDS_LED3, false);
    	//    	Board_LED_Set(LEDS_LED4, false);
    	//    	Board_LED_Set(LEDS_LED5, false);
    	//    	Board_LED_Set(LEDS_LED6, false);
    	//    	Board_LED_Set(LEDS_LED7, true);
    	//    	Board_LED_Set(LEDS_LED8, true);
    	//    }else if (strcmp(data, "7") == 0){ //Then turn on LED2
    	//    	Board_LED_Set(LEDS_LED1, false);
    	//    	Board_LED_Set(LEDS_LED2, false);
    	//    	Board_LED_Set(LEDS_LED3, false);
    	//    	Board_LED_Set(LEDS_LED4, false);
    	//    	Board_LED_Set(LEDS_LED5, false);
    	//    	Board_LED_Set(LEDS_LED6, false);
    	//    	Board_LED_Set(LEDS_LED7, false);
    	//    	Board_LED_Set(LEDS_LED8, true);
    	//    }else if (strcmp(data, "8") == 0){ //Then turn on LED2
    	//    	Board_LED_Set(LEDS_LED1, false);
    	//    	Board_LED_Set(LEDS_LED2, false);
    	//    	Board_LED_Set(LEDS_LED3, false);
    	//    	Board_LED_Set(LEDS_LED4, false);
    	//    	Board_LED_Set(LEDS_LED5, false);
    	//    	Board_LED_Set(LEDS_LED6, false);
    	//    	Board_LED_Set(LEDS_LED7, false);
    	//    	Board_LED_Set(LEDS_LED8, false);
    	//    }
      } else {
      sprintf(buffer,"mqtt_incoming_data_cb: Ignoring payload...\n\r");
	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

    }
  } else {
    /* Handle fragmented payload, store in buffer, write to file or whatever */
  }

}
static void mqtt_sub_request_cb(void *arg, err_t result)
{
  /* Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server */
  sprintf(buffer,"Subscribe result: %d\n\r", result);
  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
  const char * topico = arg;
  err_t err;
  if(status == MQTT_CONNECT_ACCEPTED) {
    sprintf(buffer,"mqtt_connection_cb: Successfully connected\n");
	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

    /* Setup callback for incoming publish requests */
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

    /* Subscribe to a topic named "placa" with QoS level 0, call mqtt_sub_request_cb with result */
    err = mqtt_subscribe(client, topico, 0, mqtt_sub_request_cb, arg);

    if(err != ERR_OK) {
      sprintf(buffer,"mqtt_subscribe return: %d\n", err);
	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

    }
  } else {
    sprintf(buffer,"mqtt_connection_cb: Disconnected, reason: %d\n", status);
	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

    /* Its more nice to be connected, so try to reconnect */
    example_do_connect(client);
  }

}

void serverFound(const char *name,ip_addr_t ipaddr, void *arg)
{

	     mqttServerIP =ipaddr ;
	   sprintf(buffer,"mqttServerIP %d.%d.%d.%d\n\r",(mqttServerIP.addr & 0xff), ((mqttServerIP.addr >> 8) & 0xff), ((mqttServerIP.addr >> 16) & 0xff), (mqttServerIP.addr >> 24));
		  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

}
void example_do_connect(mqtt_client_t *client, const char *topic)
{
  struct mqtt_connect_client_info_t ci;
  err_t err;

  /* Setup an empty client info structure */
  memset(&ci, 0, sizeof(ci));

  /* Minimal amount of information required is client identifier, so set it here */
  ci.client_id = "xonga";
  //ci.client_user = "mosquitto";
  //ci.client_pass = "chupasangre"; /* Tiempo en mi caso */


  /* Initiate client and connect to server, if this fails immediately an error code is returned
     otherwise mqtt_connection_cb will be called with connection result after attempting
     to establish a connection with the server.
     For now MQTT version 3.1.1 is always used */
  dns_gethostbyname("broker.hivemq.com", &mqttServerIP,serverFound);
  sprintf(buffer,"mqtt_server :broker.hivemq.com \n\r server address %s\n\r",ip4addr_ntoa((const ip4_addr_t *)&mqttServerIP) );
  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);
//  IP4_ADDR(&mqttServerIP, 169, 254, 173, 71);
//  err = mqtt_client_connect(client, &mqttServerIP, MQTT_PORT, mqtt_connection_cb, 0, &ci);
  err = mqtt_client_connect(client, &mqttServerIP, MQTT_PORT, mqtt_connection_cb, topic, &ci);

  /* For now just print the result code if something goes wrong */
  if(err != ERR_OK) {
    sprintf(buffer,"mqtt_connect return %d\n\r", err);
	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

  }
}

/* Called when publish is complete either with sucess or failure */
static void mqtt_pub_request_cb(void *arg, err_t result)
{
  if(result != ERR_OK) {
    sprintf(buffer,"Publish result: %d\n", result);
	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

  }
}
void example_publish(mqtt_client_t *client, void *arg)
{
  //const char *pub_payload= "Hola mundo de mierda!";
  const char *pub_payload= arg;
  err_t err;
  u8_t qos = 0; /* 0 1 or 2, see MQTT specification */
  u8_t retain = 0; /* No don't retain such crappy payload... */
  err = mqtt_publish(client, "MYDEVICE3", pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, arg);
  if(err != ERR_OK) {
    sprintf(buffer,"Publish err: %d\n\r", err);
	  HAL_UART_Transmit(&huart3,buffer,strlen(buffer),1000);

  }
}





