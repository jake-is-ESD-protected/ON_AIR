/// @file states.h
/// @brief The macros here are specifically made
/// to be identifiable by value. Only this way a 
/// complete exclusion of e.g. transitional states 
/// can be achieved by simply checking "if(cmd < 100)"

#ifndef _STATES_H_
#define _STATES_H_

/// @brief Persistant states of FSM
#define STATE_IDLE                0
#define STATE_WOEX                1
#define STATE_MEET                2
#define STATE_RECO                3
#define STATE_WAIT                4
#define STATE_WELC                5
#define N_STATES                  6

/// @brief Transitional states of FSM
#define STATE_TRANS_BORDER_C      10
#define STATE_BELL                60
#define STATE_NO_RESPONSE         70
#define STATE_NO_STATE            80
#define STATE_ERROR               90

/// @brief Additional state attributes
#define STATE_ATTRIBUTE_BORDER_C  100
#define STATE_ATTRIBUTE_LCD_DARK  100
#define STATE_ATTRIBUTE_BL_ON     101
#define STATE_ATTRIBUTE_BL_OFF    102
#define STATE_ATTRIBUTE_LA_ON     103
#define STATE_ATTRIBUTE_LA_OFF    104

/// @brief Command origins.
/// ORG_HW stems from hardware (interrupt),
/// ORG_SW stems from software (task callback)
/// ORG_WS stems from browser-client (master device)
#define ORG_HW  0
#define ORG_SW  1
#define ORG_WS  2

#endif