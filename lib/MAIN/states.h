/// @file states.h
/// @brief The enums here are specifically made
/// to be identifiable by value. Only this way a 
/// complete exclusion of e.g. transitional states 
/// can be achieved by simply checking "if(cmd < 100)"

#ifndef _STATES_H_
#define _STATES_H_

#include <Arduino.h>

typedef enum state_t{
    /// @brief Persistant states of FSM
    STATE_IDLE,
    STATE_WOEX,
    STATE_MEET,
    STATE_RECO,
    STATE_WAIT,
    STATE_WELC,
    N_STATES,
    /// @brief Transitional states of FSM
    STATE_TRANS_BORDER_C        = 10,
    STATE_BELL                  = 60,
    STATE_NO_RESPONSE           = 70,   
    STATE_NO_STATE              = 80,
    STATE_ERROR                 = 90,
    /// @brief Additional state attributes
    STATE_ATTRIBUTE_BORDER_C    = 100,
    STATE_ATTRIBUTE_LCD_DARK    = 100,
    STATE_ATTRIBUTE_BL_ON       = 101,
    STATE_ATTRIBUTE_BL_OFF      = 102,  
    STATE_ATTRIBUTE_LA_ON       = 103,
    STATE_ATTRIBUTE_LA_OFF      = 104  
}state_t;

/// @brief Command origins.
/// ORG_HW stems from hardware (interrupt),
/// ORG_SW stems from software (task callback)
typedef enum org{
    ORG_HW,
    ORG_SW
}org_t;

#endif