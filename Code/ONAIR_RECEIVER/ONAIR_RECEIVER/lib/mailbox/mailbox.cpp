/*
*****************************************************************************************
header name:        mailbox.cpp
auth:               Jakob T.
date:               08.07.22
brief:              mailbox abstraction object for inter-thread-communication
version:            V1.2
*****************************************************************************************
*/

#include "mailbox.h"
#include "hardware.h"
#include "states.h"

mailbox mbox;

mailbox::mailbox()
{
    qCMD = xQueueCreate(QUEUE_SIZE, sizeof(cmd_t));
    vPortCPUInitializeMutex(&mux);
}


void mailbox::push(cmd_t cmd, TaskHandle_t task, bool fromISR)
{
    portENTER_CRITICAL(&mux);
    if(fromISR)
    {
        xQueueSendFromISR(qCMD, &cmd, NULL);
        BaseType_t _;
        xTaskNotifyFromISR(task, 0, eNoAction, &_);
    }
    else
    {
        xQueueSend(qCMD, &cmd, 1);
        xTaskNotify(task, 0, eNoAction);
    }
    Serial.printf("[PUSH] '%d' - command of origin '%d'\r\n", cmd.content, cmd.origin);
    portEXIT_CRITICAL(&mux);
}


cmd_t mailbox::pop(bool blocking)
{
    if(blocking)
    {
        wait();
    }

    portENTER_CRITICAL(&mux);
    cmd_t cmd = {
        .origin = ORG_SW,
        .content = STATE_NO_STATE
    };
    xQueueReceive(qCMD, &cmd, 1);
    Serial.printf("[POP]  '%d' - command of origin '%d'\r\n", cmd.content, cmd.origin);
    portEXIT_CRITICAL(&mux);
    return cmd;
}


void mailbox::wait()
{
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
}