#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <stdio.h>
#include "conf.h"

#define MSGHEADSIZE (sizeof(Message) - MAXMSG)

typedef struct Message {
    long msg_len;
    long msg_type;
    char msg_data[MAXMSG];
} Message;


#endif
