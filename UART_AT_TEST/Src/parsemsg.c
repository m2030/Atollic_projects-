#include "parsemsg.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"
static const char *AT_HEADER = "AT";
static const char *AT_DEVICE_PROFILE = "DR";
static const char *AT_SET_DEVICE = "SD";
static const char *Led_off= "led_off";
static const char *Led_on= "led_On";
static const char *relay1_off= "relay1_off";
static const char *relay1_on= "relay1_on";
static const char *relay2_off= "relay2_off";
static const char *relay2_on= "relay2_on";
static const char *relay3_off= "relay3_off";
static const char *relay3_on= "relay3_on";
static const char AT_EOF = '$';


static void at_parse_msg(AT_Msg_Data *data)
{
    AT_Msg_Type type = at_check_format(data);
    switch (type) {
    case DeviceProfile:
        printf("device profile:Stm32F401RENucleoBoard MCU ID = %d\r\n",HAL_GetDEVID);
        break;
    case SetDevice:
      if(strcmp(data->data,Led_off)==0){
        printf("settings %s\r\n", data->data);
     // HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
      }
      if(strcmp(data->data,Led_on)==0){
        printf("settings %s\r\n", data->data);
    //  HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
        }
      if(strcmp(data->data,relay1_off)==0){
        printf("settings %s\r\n", data->data);
    //  HAL_GPIO_WritePin(RELAY1_GPIO_Port,RELAY1_Pin,GPIO_PIN_RESET);
        }
      if(strcmp(data->data,relay1_on)==0){
        printf("settings %s\r\n", data->data);
   //   HAL_GPIO_WritePin(RELAY1_GPIO_Port,RELAY1_Pin,GPIO_PIN_SET);
        }
            if(strcmp(data->data,relay2_off)==0){
        printf("settings %s\r\n", data->data);
   //   HAL_GPIO_WritePin(RELAY2_GPIO_Port,RELAY2_Pin,GPIO_PIN_RESET);
        }
            if(strcmp(data->data,relay2_on)==0){
        printf("settings %s\r\n", data->data);
     // HAL_GPIO_WritePin(RELAY2_GPIO_Port,RELAY2_Pin,GPIO_PIN_SET);
        }
            if(strcmp(data->data,relay3_off)==0){
        printf("settings %s\r\n", data->data);
  //    HAL_GPIO_WritePin(RELAY3_GPIO_Port,RELAY3_Pin,GPIO_PIN_RESET);
        }
            if(strcmp(data->data,relay3_on)==0){
        printf("settings %s\r\n", data->data);
    //  HAL_GPIO_WritePin(RELAY2_GPIO_Port,RELAY2_Pin,GPIO_PIN_SET);
        }
        printf("settings %s\r\n", data->data);
        break;
        case NOTME:
           printf("NOT ME\r\n");

     case Error:
       printf("Error\r\n");
        break;

    default:
        printf("Error\r\n");
        break;
    }
}


static void at_wait_msg(char *text)
{
char * pFch;
AT_Msg_Data data;

char * pLch;
char target[250];
uint8_t r_ip=0;
for (int k=0 ;k<250;k++){target[k]='\0';}
for (int x=0 ;x<sizeof(data.command);x++){data.command[x]='\0';}
for (int q=0 ;q<sizeof(data.header);q++){data.header[q]='\0';}
for (int m=0 ;m<sizeof(data.data);m++){data.data[m]='\0';}
for (int n=0 ;n<sizeof(data.id);n++){data.id[n]='\0';}

int y=0;
pFch=strchr(text,AT_EOF);
pLch=strrchr(text,AT_EOF);

y=pLch-pFch;
for(int g=0;g<(y-1);g++){
  if(g==0){*text++;}
target[g]=*(text++);
}
data.header[0]=target[0];
data.header[1]=target[1];
data.id[0]=target[2];
data.id[1]=target[3];
data.id[2]=target[4];
data.command[0]=target[5];
data.command [1]=target[6];
for(int y=0;y<strlen(target);y++){if(y>6){data.data[y-7]=target[y];}}
r_ip=atoi(data.id);

at_parse_msg(&data);

}

static AT_Msg_Type at_check_format(AT_Msg_Data *data)
{
 uint8_t DEv_id=0;
 DEv_id=atoi(data->id);
 if(strcmp(data->header, AT_HEADER) != 0)
 {
        return Error;
 }
   if ( DEv_id != 244) {
        return NOTME;
    }
    if (strcmp(data->command, AT_DEVICE_PROFILE) == 0) {
        return DeviceProfile;
    }

    if (strcmp(data->command, AT_SET_DEVICE) == 0) {
        return SetDevice;
    }

    return Error;
}
