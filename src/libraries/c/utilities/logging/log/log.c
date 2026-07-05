/*
 * log.c
 * 
 * Author: Thomas McIlwraith
 * Date: 18/04/2026
 * url: /utilities/logging/log/log.c
 */

#include "log.h"

// start time and log file pointer are stored globally
time_t start_time;
static FILE *log_fp = NULL;

void logger(
        const char* filepath,
        const int line,
        const StatusCode lvl,
        const char* fmt,
        ...){
    // Local variables
    time_t now = time(NULL);
    long elapsed = (long)difftime(now, start_time);
    char mins[10], secs[10];
    const char* filename;
    const char *lvl_string;
    va_list args;

    // Select the level string, based on the StatusCode
    switch (lvl) {
        case 0:
            lvl_string = "INFO";
            break;
        case 1:
            lvl_string = "ERROR";
            break;
        case 2:
            lvl_string = "WARNING";
            break;
        case 3:
            lvl_string = "INFO";
            break;
        default:
            lvl_string = "INFO";
            break;
    }

    // Initialise the variable argument list
    va_start(args, fmt);

    sec_to_mins_secs(mins, secs, elapsed);

    // Extract the filename from the __FILE__ macro, it is the last element
    // in the path.
    filename = strrchr(filepath, '/');
    filename = (filename) ? filename + 1 : filepath;

    // Write the messages to both the log file and stdout
    FILE *out = log_fp ? log_fp : stdout;

    if (strcmp(lvl_string, "INFO") == 0) {
        fprintf(out, "%s:%s [%s] ", mins, secs, lvl_string);
        vfprintf(out, fmt, args);
        fprintf(out, "\n");
    } else {
        fprintf(out, "%s:%s [%s] [%s ln%d] ",
                mins, secs, lvl_string, filename, line);
        vfprintf(out, fmt, args);
        fprintf(out, "\n");
    }

    va_end(args);

    return;
}

void init_log(const char *run_title, const char *working_dir){
    start_time = time(NULL);

    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s/%s.log", working_dir, WORKDIR_LOGS, run_title);

    log_fp = fopen(filepath, "w");
    if (!log_fp) {
        perror("Failed to open log file");
        log_fp = stdout; // fallback
    }

    struct tm *utc = gmtime(&start_time);
    LOG(INFO, "Program Start Time: %04d-%02d-%02dT%02d:%02d:%02dZ (UTC)",
        utc->tm_year + 1900,
        utc->tm_mon + 1,
        utc->tm_mday,
        utc->tm_hour,
        utc->tm_min,
        utc->tm_sec);
}

void sec_to_mins_secs(char* out_mins, char* out_secs,
        long elapsed){
    // Local variables
    long mins = elapsed / 60;
    long secs = elapsed % 60;

    sprintf(out_mins, "%02ld", mins);
    sprintf(out_secs, "%02ld", secs);
}

void close_log(void){
    if (log_fp && log_fp != stdout) {
        fclose(log_fp);
    }
}
