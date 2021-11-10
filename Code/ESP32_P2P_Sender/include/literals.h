#ifndef _LITERALS_H_
#define _LITERALS_H_

#define N_CMDS  6
#define CMD_LEN 5

char cmd[N_CMDS][CMD_LEN] = 
{
    "WOEX",
    "MEET",
    "RECO",
    "WAIT",
    "WELC",
    "UERR"
};

#define ID_BELL_INT             "BELL"

#define TXT_WORKING_EXCUSE      "Arbeite gerade."
#define TXT_MEETING_EXCUSE      "In einem Meeting"
#define TXT_RECORDING_EXCUSE    "Nehme was auf!"
#define TXT_WAIT_EXCUSE         "Bitte warte kurz"
#define TXT_WELCOME             "Komm doch rein!"
#define TXT_RINGING             "Es klingelt..."
#define TXT_UNKNOWN             "error: unknown"

#define TXT_SUB_SOON            "bin gleich da"
#define TXT_SUB_LATER           "später bitte..."

#define STATUS_SUCCESS          'S'
#define STATUS_ERROR            'E'

#endif