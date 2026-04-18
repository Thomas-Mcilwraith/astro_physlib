#ifndef LOG_H
#define LOG_H

// Standard libraries
#include <stdio.h>
#include <time.h>
#include <string.h>

// Local libraries

// Variable and Macro definitions
extern time_t start_time;
#define LOG(lvl, msg) logger(__FILE__, __LINE__, lvl, msg)

// Function prototypes
void init_log(void);
void sec_to_mins_secs(char* out_mins, char* out_secs,
        long elapsed);
void logger(
        const char* fname,
        const int line,
        const char* lvl,
        const char* msg);

#endif
