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


void mailbox::push(cmd_t cmd, bool fromISR)
{
    portENTER_CRITICAL(&mux);
    if(fromISR)
    {
        xQueueSendFromISR(qCMD, &cmd, NULL);
    }
    else
    {
        xQueueSend(qCMD, &cmd, 1);
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


void mailbox::notify(TaskHandle_t task, bool fromISR)
{
    if(fromISR)
    {
        BaseType_t _;
        xTaskNotifyFromISR(task, 0, eNoAction, &_);
    }
    else
    {
        xTaskNotify(task, 0, eNoAction);
    }
}


bool mailbox::data_avail(void)
{
    uint8_t n = uxQueueMessagesWaiting(qCMD);
    Serial.printf("\tCurrently %d items in mailbox\r\n", n);
    if(n == 0)
    {
        return false;
    }
    else
    {
        return true;
    }

}


void mailbox::wait()
{
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
}