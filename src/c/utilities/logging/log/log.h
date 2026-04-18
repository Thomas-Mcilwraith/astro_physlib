#ifndef LOG_H
#define LOG_H

// Standard libraries
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

// Local libraries

// Variable and Macro definitions
extern time_t start_time;
#define LOG(lvl, ...) logger(__FILE__, __LINE__, lvl, __VA_ARGS__)

// Function prototypes

/**
 * @brief 
 * Writes a message to the stdout. init_log() must have been called atleast
 * once before (this runtime) for this function to work.
 *
 * @param filepath The name of the file that called the logger function.
 * @param line The line number in the file that called the logger function.
 * @param lvl The level of the message, either INFO, WARNING, or ERROR.
 * @param fmt The message to be logged.
 */
void logger(
        const char* filepath,
        const int line,
        const char* lvl,
        const char* fmt, ...);

/**
 * @brief 
 * Initializes the logger. This must be called before any other logger
 * function is called.
 */
void init_log(void);

/**
 * @brief 
 * Converts a time in seconds to minutes and seconds.
 *
 * @param out_mins The output string for the minutes.
 * @param out_secs The output string for the seconds.
 * @param elapsed The time in seconds.
 */
void sec_to_mins_secs(char* out_mins, char* out_secs,
        long elapsed);

#endif
