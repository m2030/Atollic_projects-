#ifndef __PARSEMSG_H
#define __PARSEMSG_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t header[3];
    uint8_t id[4];
    uint8_t command[3];
    uint8_t data[128];
} AT_Msg_Data;
typedef enum {
    DeviceProfile,
    SetDevice,
    Error,
    NOTME
} AT_Msg_Type;
static void at_wait_msg(char *text);
static void at_parse_msg(AT_Msg_Data *data);
static AT_Msg_Type at_check_format(AT_Msg_Data *data);
#endif /* __PARSEMSG_H */
