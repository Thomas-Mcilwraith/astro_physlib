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
#define OK(...) ok()
#define WARNING(...) warning(__FILE__, __LINE__, __VA_ARGS__)
#define ERROR(...) error(__FILE__, __LINE__, __VA_ARGS__)
#define WORKDIR_LOGS "logs"

/**
 * @brief
 * A very simple form of error handling is used in this project. The return
 * value of all functions is reserved for a StatusCode in all cases except when
 * the function is trivial and it is certain that, given the correct arguement
 * types, a function cannot fail in any capacity.
 * 
 * After calling any function which may return a StatusCode, the caller must
 * check the status of the status code and propagate an ERROR to its parent
 * scope if detected. This is done simply by an early return ERR;
 * 
 * Since nothing more is required in terms of error handling, and all errors
 * should be accompanied by an appropriate log message so that the error is
 * traceable, we include StatusCode here to avoid extensive imports etc.
 */
typedef enum {
    OK,
    ERROR,
    WARNING,
    INFO
} StatusCode;

// Function prototypes

/**
 * @brief 
 * Writes a message to the stdout. init_log() must have been called atleast
 * once before (this runtime) for this function to work.
 *
 * @param filepath The name of the file that called the logger function.
 * @param line The line number in the file that called the logger function.
 * @param lvl The StatusCode associated to the message.
 * @param fmt The message to be logged.
 */
void logger(
    // Inputs
    const char* filepath,
    const int line,
    const StatusCode lvl,
    const char* fmt, ...);

/**
 * @brief 
 * Initializes the logger. This must be called before any other logger
 * function is called.
 *
 * @param run_title The name of the program for this run.
 * @param working_dir The working directory of the program.
 * @param program_name The name of the program.
 */
void init_log(
    // Inputs
    const char *run_title,
    const char *working_dir,
    const char* program_name);

/**
 * @brief 
 * Closes the log file. If the log file is stdout, this function does nothing.
 */
void close_log(void);

/**
 * @brief 
 * Writes a message to a stream. This function is used by logger() and
 * init_log() to write the log messages to the log file and stdout.
 *
 * @param stream The stream to write the message to.
 * @param timestamp The timestamp of the message.
 * @param level The StatusCode associated to the message.
 * @param filename The name of the file that called the logger function.
 * @param line The line number in the file that called the logger function.
 * @param fmt The message to be logged.
 * @param args The arguments to be logged.
 */
void log_to_stream(
    // Inputs
    FILE *stream,
    const char* timestamp,
    const char *level,
    const char *filename,
    int line,
    const char *fmt,
    va_list args);

StatusCode ok();

StatusCode warning(
    const char* filepath,
    const int line,
    const char* fmt, ...);

StatusCode error(
    const char* filepath,
    const int line,
    const char* fmt, ...);

#endif
