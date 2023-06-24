/*
*****************************************************************************************
header name:        mailbox.h
auth:               Jakob T.
date:               08.07.22
brief:              mailbox abstraction object for inter-thread-communication
version:            V1.2
*****************************************************************************************
*/

#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include "Arduino.h"

#define QUEUE_SIZE      30
#define BLOCKING        true
#define NON_BLOCKING    false

// command data type
typedef struct {
    uint8_t origin;
    uint8_t content;
    uint16_t pad;
} cmd_t;

class mailbox
{
    public:

    mailbox();


    void push(cmd_t cmd, bool fromISR);


    cmd_t pop(bool blocking);


    void notify(TaskHandle_t task, bool fromISR);


    bool data_avail();


    void wait();


    private:

    portMUX_TYPE mux;
    QueueHandle_t qCMD;

};

extern mailbox mbox;

#endif