/*
 * log.c
 * 
 * Author: Thomas McIlwraith
 * url: /utilities/logging/log/log.c
 *
 * Description:
 *   This file contains...
 */

#include "log.h"

time_t start_time;

void init_log(void){
    start_time = time(NULL);
}

void sec_to_mins_secs(char* out_mins, char* out_secs,
        long elapsed){
    // Local variables
    long mins = elapsed / 60;
    long secs = elapsed % 60;

    sprintf(out_mins, "%02ld", mins);
    sprintf(out_secs, "%02ld", secs);
}

void logger(
        const char* fname,
        const int line,
        const char* lvl,
        const char* msg){
    // Local variables
    time_t now = time(NULL);
    long elapsed = (long)difftime(now, start_time);
    char mins[10], secs[10];
    const char* fname_last_element;

    sec_to_mins_secs(mins, secs, elapsed);

    // Extract the filename from the __FILE__ macro, it is the last element
    // in the path.
    fname_last_element = strrchr(fname, '/');
    fname_last_element = (fname_last_element) ? fname_last_element + 1 : fname;

    if (strcmp(lvl, "INFO") == 0) {
        printf("%s:%s [%s] %s\n", mins, secs, lvl, msg);
    } else {
        printf("%s:%s [%s] [%s ln%d] %s\n",
                mins, secs, lvl, fname_last_element, line, msg);
    }

    return;
}
