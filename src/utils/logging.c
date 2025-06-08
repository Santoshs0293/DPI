#include <stdio.h>
#include <string.h>
#include <time.h>

FILE *log_file;

void init_logging(const char *log_path) {
    log_file = fopen(log_path, "a");
    if (!log_file) {
        fprintf(stderr, "Failed to open log file: %s\n", log_path);
        exit(1);
    }
}

void log_message(const char *level, const char *message) {
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0'; // Remove newline
    fprintf(log_file, "[%s] %s: %s\n", time_str, level, message);
    fflush(log_file);
}

void cleanup_logging() {
    fclose(log_file);
}