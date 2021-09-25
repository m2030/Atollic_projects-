/**
  ******************************************************************************
  * @file    subscribe_publish_sensor_values.c
  * @author  MCD Application Team
  * @brief   Control of the measurement sampling and MQTT reporting loop.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"
#include "iot_flash_config.h"
#include "sensors_data.h"
#include "msg.h"

void MQTTcallbackHandler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen, IoT_Publish_Message_Params *params, void *pData);
int subscribe_publish_sensor_values(void);

/* Private defines ------------------------------------------------------------*/
#define MQTT_CONNECT_MAX_ATTEMPT_COUNT 3
#define TIMER_COUNT_FOR_SENSOR_PUBLISH 10

#define aws_json_pre        "{\"state\":{\"reported\":"
#define aws_json_desired    "{\"state\":{\"desired\":"
#define aws_json_post       "}}"
extern int temp_low, temp_high, rh_low, rh_high;

/* Private variables ---------------------------------------------------------*/
static char ledstate[] = { "Off" };
static char cPTopicName[MAX_SHADOW_TOPIC_LENGTH_BYTES] = "";
static char cSTopicName[MAX_SHADOW_TOPIC_LENGTH_BYTES] = "";
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
* @brief This parameter will avoid infinite loop of publish and exit the program after certain number of publishes
*/
static uint32_t publishCount = 60;

const  char CA_CREDIATION_AWS[]=
"-----BEGIN CERTIFICATE-----\n"
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"
"rqXRfboQnoZsG4q5WTP468SQvvG5\n"
"-----END CERTIFICATE-----\n";
const  char CLIENT_CERTIFICATION[]=
		"-----BEGIN CERTIFICATE-----\n"
		"MIIDWjCCAkKgAwIBAgIVAL50UATwEpQQD4+rKnO2/ynYsXxSMA0GCSqGSIb3DQEB\n"
		"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"
		"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDA5MDUyMDQ5\n"
		"NDBaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"
		"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDHPSAO/amDOyfKgjyX\n"
		"Slr5XT8dwVuUOBVyqBJEm5UbSCMZoiu6umCVSI1q34ROxbW9NohcsKX8Ve3n4JaK\n"
		"5gvyIbqZjAFtdZiK6dOMg8N69CRmyNz1i13wBI5LOQrd+3PhBmzwxj+o2Mo6tPFK\n"
		"QJ5edBJtLmSPELAwZdRerRrvfhFi2s3hxKqZvkzGI6AClfFq8ir3WOd7rBYemtUA\n"
		"ochFeqM6t8QAHxc71nIjPobt+cluOrhLWgcITJeci+cTe3at4uC2bw2Q2s1kaBgB\n"
		"GS1ENyUp9l8OftC/GQjkWKdBPMXoj9CC3jSz38z5DcAUuUwA9dD2OXgoaIxIfpzO\n"
		"uS37AgMBAAGjYDBeMB8GA1UdIwQYMBaAFJmVpiezn7ZlwkkgBy2egU8FQZqvMB0G\n"
		"A1UdDgQWBBRz3Z58J5+E/ckq3muPcdk23I8zdzAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"
		"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAPBMggO90gzgcjHnG2boo1kkL\n"
		"6sOkgTCVV5nDOub6Xz7TOTfl6eTQoe+jq9KYkS90a4vdmGbXmG7l2lXWZeiVEVH+\n"
		"N7CVptrz8Z4Q4Z3+And5UXyPdPZNoyDrmJI9jHy92vAXSEwqvtPPPGq8gwWw5ILQ\n"
		"elN5FolKqI/tRFM07R1LBOtM1nTVavUBK43gy1/2SFFB+zG/Hs6xATrTgYNPdBAr\n"
		"T+igr8xidruZldB6f5CWSFE2mutT8AV0kzvVLksaBHybI2M9Zpehj8eEhd8tFqvS\n"
		"C4EIO9H9el0fzGLHN6wdwgsHBABpJn72TzjXToDqwbsoefmSpdUfDPYFo8TGLA==\n"
		"-----END CERTIFICATE-----\n";
const  char CLIENT_PRIVATE_KEY[]=
"-----BEGIN RSA PRIVATE KEY-----\n"
		"MIIEpAIBAAKCAQEAxz0gDv2pgzsnyoI8l0pa+V0/HcFblDgVcqgSRJuVG0gjGaIr\n"
		"urpglUiNat+ETsW1vTaIXLCl/FXt5+CWiuYL8iG6mYwBbXWYiunTjIPDevQkZsjc\n"
		"9Ytd8ASOSzkK3ftz4QZs8MY/qNjKOrTxSkCeXnQSbS5kjxCwMGXUXq0a734RYtrN\n"
		"4cSqmb5MxiOgApXxavIq91jne6wWHprVAKHIRXqjOrfEAB8XO9ZyIz6G7fnJbjq4\n"
		"S1oHCEyXnIvnE3t2reLgtm8NkNrNZGgYARktRDclKfZfDn7QvxkI5FinQTzF6I/Q\n"
		"gt40s9/M+Q3AFLlMAPXQ9jl4KGiMSH6czrkt+wIDAQABAoIBAQCRLgZNRiOeP53I\n"
		"H8ydBY5zBLJ4MSxVPeTLDpN5MNGe9pq1FAm/YA3+2cZKqSLU75Z1kOYefI4G6Gqa\n"
		"O3oXLnhn8JHzTyS/KSCFoMsJKf5oeryHtTflKK/DtN4nDCLSDGetAiyh6iP7GjWK\n"
		"P81PKPT9qKDAtDyMT0HDkoAEmSj2d2ECiSPuc2L4Nu7xRGB9G5MDMebWzz1S61FF\n"
		"pZae4ky0AS208yTS1wiNQDEFfZFVNRjCTuezxqpmnjIBuR/6EHZK8hD94sxwO5O7\n"
		"ohFZQWSKJfJuyqunDwKEFMODtWOctej6FlI4r1zK482zlpPMrN4wKl0g79fD+c9Q\n"
		"MwitiChBAoGBAPg5dvbFomoBSw3O8833o3HtT0xku2SNT8IEp0luEj+w3ef4qdsM\n"
		"0NHeW0IQSw5IdnpL2I2HWQ2uhe0mdVJUP4dxYDPx2Vx+4SJtEmP/gZHr2mD/3645\n"
		"BpGcB//68QBn16seWramPxcDEIDj656ygRJtFuWomb0nlkeufPitj39bAoGBAM16\n"
		"1u/v/fwm/Hk62meLan29MsxtpcLGsCW3KOuTHP5mz6NgHNBcaeXbhJQJX6rJTHKs\n"
		"bbQFyG2v3Zh3QvFBaTAGBxx2ryn5pJrBGKk4ATEx9KtO6oLJdUzvVvY+RqONRTBR\n"
		"hT9DakSHDVa18SRtXGZNPZvgpuIcRpfwXCg5ie3hAoGAGAc4BNtMxYYqhcOqxh1a\n"
		"rQST0IlPuoQTXGym5OzKx2+Pi4z+pD2xWR91Jgf9u/w3u87rv4eCj1h9jl9SrA2L\n"
		"uMi4z05yBAokMPjt9tAc8ssp8InUVUEf6ah4/caE3c0vi1j83wFoygl554AopswQ\n"
		"wn/c345ak3zphzbU0h267lECgYBaw7kv0W36tOnvTvzsohvPIQTQYlrHN+bVMhvk\n"
		"O16iTZ3IiAsSStAIVbgxgBZ/JV4chWgMDkXjlbynvX6XfRJOwUrg2tgzDXAVX9hZ\n"
		"30A/j9aI2VojnYOwEUTzjry7QArg9hGKyWemj5VnyQSU163qDpJ5SXnOBTl1ohnD\n"
		"Nu9jYQKBgQCae6Y9mfjLI5f2wgDK92q2kfeS3z3GBoxvmsBHbwqoI6y4MTK1SD50\n"
		"3P1tYBgmPILeglLpjKjEZi6Lvl1rwMfIwmpI8QVhVS+sTxNsdwfnXkfeRbW/huvm\n"
		"/RW3Dx51x6TGN13KW1zFNdgFo4HrT2B/OOuo1hUGjtlJpz8ysQ2xeQ==\n"
		"-----END RSA PRIVATE KEY-----\n";
/* Private function prototypes -----------------------------------------------*/

/* Functions Definition ------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
int cloud_device_enter_credentials(void)
{
  int ret = 0;
  iot_config_t iot_config;

  memset(&iot_config, 0, sizeof(iot_config_t));
    
  printf("\nEnter server address: (example: xxx.iot.region.amazonaws.com) \n");
  getInputString(iot_config.server_name, USER_CONF_SERVER_NAME_LENGTH);
  msg_info("read: --->\n%s\n<---\n", iot_config.server_name);
  
  printf("\nEnter device name: (example: mything1) \n");
  getInputString(iot_config.device_name, USER_CONF_DEVICE_NAME_LENGTH);
  msg_info("read: --->\n%s\n<---\n", iot_config.device_name);

  if(setIoTDeviceConfig(&iot_config) != 0)
  {
    ret = -1;
    msg_error("Failed programming the IoT device configuration to Flash.\n");
  }
  
  return ret;
}

bool app_needs_device_keypair()
{
  return true;
}


/**
* @brief MQTT disconnect callback hander
*
* @param pClient: pointer to the AWS client structure
* @param data: 
* @return no return
*/
static void disconnectCallbackHandler(AWS_IoT_Client *pClient, void *data)
{
  msg_warning("MQTT Disconnect\n");
  IoT_Error_t rc = FAILURE;
  
  if(NULL == data)
  {
    return;
  }

  AWS_IoT_Client *client = (AWS_IoT_Client *)data;

  if(aws_iot_is_autoreconnect_enabled(client))
  {
    msg_info("Auto Reconnect is enabled, Reconnecting attempt will start now\n");
  }
  else
  {
    msg_warning("Auto Reconnect not enabled. Starting manual reconnect...\n");
    rc = aws_iot_mqtt_attempt_reconnect(client);

    if(NETWORK_RECONNECTED == rc)
    {
      msg_warning("Manual Reconnect Successful\n");
    }
    else
    {
      msg_warning("Manual Reconnect Failed - %d\n", rc);
    }
  }
}

/* Exported functions --------------------------------------------------------*/

/**
* @brief MQTT subscriber callback hander
*
* called when data is received from AWS IoT Thing (message broker)
* @param MQTTCallbackParams type parameter
* @return no return
*/
void MQTTcallbackHandler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen, IoT_Publish_Message_Params *params, void *pData)
{
  const char msg_on[]  = "{\"state\":{\"reported\":{\"LED_value\":\"On\"}}}";
  const char msg_off[] = "{\"state\":{\"reported\":{\"LED_value\":\"Off\"}}}";
  const char *msg = NULL;
  IoT_Publish_Message_Params paramsQOS1 = {QOS1, 0, 0, 0, NULL,0};
  
  msg_info("\nMQTT subscribe callback......\n");
  msg_info("%.*s\n", (int)params->payloadLen, (char *)params->payload);
  
  /* If a new desired LED state is received, change the LED state. */
  if (strstr((char *) params->payload, "\"desired\":{\"LED_value\":\"On\"}") != NULL)
  {
    Led_SetState(true);
    strcpy(ledstate, "On");    
    msg_info("LED On!\n");
    msg = msg_on;
  }
  else if (strstr((char *) params->payload, "\"desired\":{\"LED_value\":\"Off\"}") != NULL)
  {
    Led_SetState(false);
    strcpy(ledstate, "Off");
    msg_info("LED Off!\n");
    msg = msg_off;
  }
  
  /* Report the new LED state to the MQTT broker. */
  if (msg != NULL)
  { 
    paramsQOS1.payload = (void *) msg;
    paramsQOS1.payloadLen = strlen(msg) + 1;
    IoT_Error_t rc = aws_iot_mqtt_publish(pClient, cPTopicName, strlen(cPTopicName), &paramsQOS1);

    if (rc == AWS_SUCCESS)
    {
      msg_info("\nPublished the new LED status to topic %s:", cPTopicName);
      msg_info("%s\n", msg);
    }
  }
}

/**
* @brief main entry function to AWS IoT code
*
* @param no parameter
* @return AWS_SUCCESS: 0 
          FAILURE: -1
*/
int subscribe_publish_sensor_values(void)
{
  bool infinitePublishFlag = true;
  const char serverAddress[50];
  const char *pCaCert;
  const char *pClientCert;
  const char *pClientPrivateKey;
  const char *pDeviceName;
  char cPayload[AWS_IOT_MQTT_TX_BUF_LEN];
  char const  deviceName[20];
  sprintf(deviceName,"try");
  sprintf(pDeviceName,"try");
  sprintf(serverAddress,"a1fsx4m82pivc1-ats.iot.us-west-2.amazonaws.com");
  int i = 0;
  int connectCounter;
  IoT_Error_t rc = FAILURE;
#ifdef SENSOR
  int timeCounter = 0;
#endif
  uint8_t bp_pushed;

  AWS_IoT_Client client;
  memset(&client, 0, sizeof(AWS_IoT_Client));
  IoT_Client_Init_Params mqttInitParams = iotClientInitParamsDefault;
  IoT_Client_Connect_Params connectParams = iotClientConnectParamsDefault;

  getIoTDeviceConfig(&deviceName);
  if (strlen(deviceName) >= MAX_SIZE_OF_THING_NAME)
  {
    msg_error("The length of the device name stored in the iot user configuration is larger than the AWS client MAX_SIZE_OF_THING_NAME.\n");
    return -1;
  }
  
  snprintf(cPTopicName, sizeof(cPTopicName), AWS_DEVICE_SHADOW_PRE "%s" AWS_DEVICE_SHADOW_UPDATE_TOPIC, deviceName);
  snprintf(cSTopicName, sizeof(cSTopicName), AWS_DEVICE_SHADOW_PRE "%s" AWS_DEVICE_SHADOW_UPDATE_ACCEPTED_TOPIC, deviceName);
  
  /*
  IoT_Publish_Message_Params paramsQOS0;
  IoT_Publish_Message_Params paramsQOS1;
  */

  msg_info("AWS IoT SDK Version %d.%d.%d-%s\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_TAG);

///  getServerAddress(&serverAddress);
 /// getTLSKeys(&pCaCert, &pClientCert, &pClientPrivateKey);
  mqttInitParams.enableAutoReconnect = false; /* We enable this later below */
  mqttInitParams.pHostURL = (char *) serverAddress;
  mqttInitParams.port = AWS_IOT_MQTT_PORT;
  mqttInitParams.pRootCALocation = (char *) CA_CREDIATION_AWS;
  mqttInitParams.pDeviceCertLocation = (char *) CLIENT_CERTIFICATION;
  mqttInitParams.pDevicePrivateKeyLocation = (char *) CLIENT_PRIVATE_KEY;
  mqttInitParams.mqttCommandTimeout_ms = 20000;
  mqttInitParams.tlsHandshakeTimeout_ms = 5000;
  mqttInitParams.isSSLHostnameVerify = true;
  mqttInitParams.disconnectHandler = disconnectCallbackHandler;
  mqttInitParams.disconnectHandlerData = NULL;

  rc = aws_iot_mqtt_init(&client, &mqttInitParams);

  if(AWS_SUCCESS != rc)
  {
    msg_error("aws_iot_mqtt_init returned error : %d\n", rc);
    return -1;
  }

  getIoTDeviceConfig(&pDeviceName);
  connectParams.keepAliveIntervalInSec = 30;
  connectParams.isCleanSession = true;
  connectParams.MQTTVersion = MQTT_3_1_1;
  connectParams.pClientID = (char *) pDeviceName;
  connectParams.clientIDLen = (uint16_t) strlen(pDeviceName);
  connectParams.isWillMsgPresent = false;


  connectCounter = 0;
  
  do 
  {
    connectCounter++;
    printf("MQTT connection in progress:   Attempt %d/%d ...\n",connectCounter,MQTT_CONNECT_MAX_ATTEMPT_COUNT);
    rc = aws_iot_mqtt_connect(&client, &connectParams);
  } while((rc != AWS_SUCCESS) && (connectCounter < MQTT_CONNECT_MAX_ATTEMPT_COUNT));  

  if(AWS_SUCCESS != rc) 
  {
    msg_error("Error(%d) connecting to %s:%d\n", rc, mqttInitParams.pHostURL, mqttInitParams.port);
    return -1;
  }
  else
  {
    printf("Connected to %s:%d\n", mqttInitParams.pHostURL, mqttInitParams.port);
  }

  /*
  * Enable Auto Reconnect functionality. Minimum and Maximum time of Exponential backoff are set in aws_iot_config.h
  *  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
  *  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL
  */
  rc = aws_iot_mqtt_autoreconnect_set_status(&client, true);
  
  if(AWS_SUCCESS != rc)
  {
    msg_error("Unable to set Auto Reconnect to true - %d\n", rc);

    if (aws_iot_mqtt_is_client_connected(&client)) 
    {
      aws_iot_mqtt_disconnect(&client);
    }

    return -1;
  }
  
  rc = aws_iot_mqtt_subscribe(&client, cSTopicName, strlen(cSTopicName), QOS0, MQTTcallbackHandler, NULL);

  if(AWS_SUCCESS != rc)
  {
    msg_error("Error subscribing : %d\n", rc);
    return -1;
  } 
  else
  {
    msg_info("Subscribed to topic %s\n", cSTopicName);
  }

  sprintf(cPayload, "%s : %d ", "hello from STM", i);

  IoT_Publish_Message_Params paramsQOS1 = {QOS1, 0, 0, 0, NULL,0};
  paramsQOS1.payload = (void *) cPayload;

  if(publishCount != 0)
  {
    infinitePublishFlag = false;
  }
  
  
  printf("Press the User button (Blue) to publish the LED desired value on the %s topic\n", cPTopicName);
  
#ifdef SENSOR
  timeCounter = TIMER_COUNT_FOR_SENSOR_PUBLISH;
#endif
   
  while((NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc || AWS_SUCCESS == rc) && (publishCount > 0 || infinitePublishFlag))
  {
    /* Max time the yield function will wait for read messages */
    rc = aws_iot_mqtt_yield(&client, 10);

    if(NETWORK_ATTEMPTING_RECONNECT == rc)
    {
      /* Delay to let the client reconnect */
      HAL_Delay(1000); 
      msg_info("Attempting to reconnect\n");
      /* If the client is attempting to reconnect we will skip the rest of the loop */
      continue; 
    }
    if(NETWORK_RECONNECTED == rc)
    {
      msg_info("Reconnected.\n");
    }

    char temp_value[50];
    uint32_t  rh_high=60,rh_low=60,temp_low=20,temp_high=24;
    
    sprintf(temp_value,"%d.%d",temp_low , temp_high);
    char RH_value[50];
    
    sprintf(RH_value,"%d.%d",rh_low, rh_high);
      printf("Sending the desired temp and rh state to AWS.\n");
      /* create desired message */
      memset(cPayload, 0, sizeof(cPayload));
      strcat(cPayload, aws_json_desired);
      strcat(cPayload, "{\"temp_value\":\"");
      strcat(cPayload, temp_value);
      strcat(cPayload, "{\"RH_value\":\"");
      strcat(cPayload, RH_value);
      strcat(cPayload, "\"}");
      strcat(cPayload, aws_json_post);
      paramsQOS1.payloadLen = strlen(cPayload) + 1;

      do 
      {
        rc = aws_iot_mqtt_publish(&client, cPTopicName, strlen(cPTopicName), &paramsQOS1);
        if (rc == AWS_SUCCESS)
        {
          printf("\nPublished to topic %s:", cPTopicName);
          printf("%s\n", cPayload);
        }

        if (publishCount > 0)
        {
          publishCount--;
        }
      } while(MQTT_REQUEST_TIMEOUT_ERROR == rc && (publishCount > 0 || infinitePublishFlag));      
      } /* End of while */
  /* Wait for all the messages to be received */
  aws_iot_mqtt_yield(&client, 10);

  rc = aws_iot_mqtt_disconnect(&client);


  return rc;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
