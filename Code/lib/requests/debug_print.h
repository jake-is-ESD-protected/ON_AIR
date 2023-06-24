#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_

    // debug helper
    #ifdef VERBOSE_DEBUG
    #define DEBUG(msg) {Serial.print(msg);}
    #else
    #define DEBUG(msg)
    #endif

#endif