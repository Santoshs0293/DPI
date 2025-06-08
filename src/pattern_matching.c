#include <hs.h>
#include <stdio.h>
#include <string.h>
#include "utils/logging.h"

hs_database_t *database = NULL;
hs_scratch_t *scratch = NULL;

// Initialize Hyperscan with regex patterns
int init_hyperscan() {
    const char *patterns[] = {"HTTP/1\\.[0-1]", "GET /", "POST /"};
    unsigned int ids[] = {1, 2, 3};
    hs_compile_error_t *compile_err;

    if (hs_compile_multi(patterns, NULL, ids, 3, HS_MODE_BLOCK, NULL, &database, &compile_err) != HS_SUCCESS) {
        log_message("ERROR", compile_err->message);
        hs_free_compile_error(compile_err);
        return -1;
    }

    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        log_message("ERROR", "Failed to allocate scratch space");
        hs_free_database(database);
        return -1;
    }
    return 0;
}

// Callback for pattern matches
static int on_match(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *context) {
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Pattern match: ID %u at offset %llu-%llu", id, from, to);
    log_message("INFO", log_msg);
    return 0;
}

// Process packet for pattern matching
void match_packet(const char *data, unsigned int len) {
    if (hs_scan(database, data, len, 0, scratch, on_match, NULL) != HS_SUCCESS) {
        log_message("ERROR", "Failed to scan packet");
    }
}

// Cleanup Hyperscan
void cleanup_hyperscan() {
    hs_free_scratch(scratch);
    hs_free_database(database);
}