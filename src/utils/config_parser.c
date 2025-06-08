#include <yaml.h>
#include <stdio.h>
#include <string.h>
#include "logging.h"

typedef struct {
    int dpdk_port;
    char *rf_model_path;
    char *cnn_model_path;
} Config;

Config parse_config(const char *config_file) {
    Config config = {0, NULL, NULL};
    FILE *fh = fopen(config_file, "r");
    yaml_parser_t parser;
    yaml_document_t document;

    if (!yaml_parser_initialize(&parser)) {
        log_message("ERROR", "Failed to initialize YAML parser");
        return config;
    }

    yaml_parser_set_input_file(&parser, fh);
    if (!yaml_parser_load(&parser, &document)) {
        log_message("ERROR", "Failed to parse config file");
        yaml_parser_delete(&parser);
        fclose(fh);
        return config;
    }

    // Simplified parsing (update as needed)
    config.dpdk_port = 0;
    config.rf_model_path = strdup("models/rf_model.pkl");
    config.cnn_model_path = strdup("models/cnn_model.h5");

    yaml_parser_delete(&parser);
    yaml_document_delete(&document);
    fclose(fh);
    return config;
}