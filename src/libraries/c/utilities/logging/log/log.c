/*
 * log.c
 * 
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /utilities/logging/log/log.c
 */

#include "log.h"

// start time and log file pointer are stored globally
static FILE *log_fp = NULL;

void logger(
        const char* filepath,
        const int line,
        const StatusCode lvl,
        const char* fmt,
        ...){
    // Local variables
    time_t now = time(NULL);
    const char* filename;
    const char *lvl_string;
    va_list args, copy;

    struct tm utc;
    char timestamp[21];  // "YYYY-MM-DDTHH:MM:SSS"
    gmtime_r(&now, &utc);
    strftime(timestamp, sizeof(timestamp),
             "%Y-%m-%dT%H:%M:%S", &utc);

    // Select the level string, based on the StatusCode
    switch (lvl) {
        case INFO:
            lvl_string = "INFO";
            break;
        case ERROR:
            lvl_string = "ERROR";
            break;
        case WARNING:
            lvl_string = "WARNING";
            break;
        default:
            lvl_string = "INFO";
            break;
    }

    // Initialise the variable argument list
    va_start(args, fmt);

    // Extract the filename from the __FILE__ macro, it is the last element
    // in the path.
    filename = strrchr(filepath, '/');
    filename = (filename) ? filename + 1 : filepath;

    va_start(args, fmt);
    if (log_fp) {
        va_copy(copy, args);
        log_to_stream(log_fp, timestamp, lvl_string, filename, line, fmt, copy);
        va_end(copy);
    }

    va_copy(copy, args);
    log_to_stream(stdout, timestamp, lvl_string, filename, line, fmt, copy);
    va_end(copy);
    va_end(args);

    return;
}

void init_log(
    // Inputs
    const char *run_title,
    const char *working_dir,
    const char* program_name){

    // Local variables
    char filepath[1024];

    // Open the log file
    snprintf(filepath, sizeof(filepath), "%s/%s/%s.log", working_dir, WORKDIR_LOGS, run_title);
    log_fp = fopen(filepath, "w");
    if (!log_fp) {
        perror("Failed to open log file");
        log_fp = stdout; // fallback to stdout
    }

    LOG(INFO, "Program Started: %s %s", program_name, run_title);
    return;
}

void close_log(void){
    if (log_fp && log_fp != stdout) {
        fclose(log_fp);
    }
}

void log_to_stream(
    FILE *stream,
    const char* timestamp,
    const char *level,
    const char *filename,
    int line,
    const char *fmt,
    va_list args) {
    if (strcmp(level, "INFO") == 0)
        fprintf(stream, "%s %s ", timestamp, level);
    else
        fprintf(stream, "%s %s [%s ln%d] ",
                timestamp, level, filename, line);

    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}
