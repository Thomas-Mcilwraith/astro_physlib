/*
 * log.c
 * 
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /utilities/logging/log/log.c
 */

#include "log.h"

time_t start_time;

/**
  * See log.h for IO description.
  *
  * The offending file name and line number are added to WARNING and ERROR
  * messages. This is not neccessary for INFO messages so we don't do it to
  * save space.
 */
void logger(
        const char* filepath,
        const int line,
        const char* lvl,
        const char* fmt,
        ...){
    // Local variables
    time_t now = time(NULL);
    long elapsed = (long)difftime(now, start_time);
    char mins[10], secs[10];
    const char* filename;
    va_list args;

    // Initialise the variable argument list
    va_start(args, fmt);

    sec_to_mins_secs(mins, secs, elapsed);

    // Extract the filename from the __FILE__ macro, it is the last element
    // in the path.
    filename = strrchr(filepath, '/');
    filename = (filename) ? filename + 1 : filepath;

    if (strcmp(lvl, "INFO") == 0) {
        printf("%s:%s [%s] ", mins, secs, lvl);
        vprintf(fmt, args);
        printf("\n");
    } else {
        printf("%s:%s [%s] [%s ln%d] ", mins, secs, lvl, filename, line);
        vprintf(fmt, args);
        printf("\n");
    }

    va_end(args);

    return;
}

/**
 * See log.h for IO description.
 *
 * This function initialises the time reference for logger()
 */
void init_log(void){
    start_time = time(NULL);
}

/**
 * See log.h for IO description.
 *
 * This function converts a time in seconds to minutes and seconds.
 */
void sec_to_mins_secs(char* out_mins, char* out_secs,
        long elapsed){
    // Local variables
    long mins = elapsed / 60;
    long secs = elapsed % 60;

    sprintf(out_mins, "%02ld", mins);
    sprintf(out_secs, "%02ld", secs);
}
