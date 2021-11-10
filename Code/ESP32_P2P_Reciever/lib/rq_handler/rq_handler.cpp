#include "rq_handler.h"
#include "string.h"

int handle_rq(char* rq)
{
    char cmd[N_CMDS][CMD_LEN] = 
    {
        "WOEX",
        "MEET",
        "RECO",
        "WAIT",
        "WELC",
        "UERR"
    };
    int i;
    for(i = 0; i < N_CMDS; i++)
    {
        if(strcmp((const char*)rq, cmd[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}