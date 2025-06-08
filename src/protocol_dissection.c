#include <ndpi_api.h>
#include <stdio.h>
#include <stdint.h>
#include <rte_mbuf.h>
#include "utils/logging.h"

struct ndpi_detection_module_struct *ndpi_module;

// Initialize nDPI
void init_ndpi() {
    ndpi_module = ndpi_init_detection_module();
    if (!ndpi_module) {
        log_message("ERROR", "Failed to initialize nDPI");
        exit(1);
    }
    NDPI_PROTOCOL_BITMASK all;
    NDPI_BITMASK_SET_ALL(all);
    ndpi_set_protocol_detection_bitmask2(ndpi_module, &all);
}

// Process packet for protocol identification
void dissect_packet(struct rte_mbuf *mbuf) {
    struct ndpi_flow_struct *flow = ndpi_flow_malloc(sizeof(struct ndpi_flow_struct));
    if (!flow) {
        log_message("ERROR", "Failed to allocate nDPI flow");
        return;
    }
    memset(flow, 0, sizeof(struct ndpi_flow_struct));

    // Extract packet data
    uint8_t *packet_data = rte_pktmbuf_mtod(mbuf, uint8_t *);
    uint32_t packet_len = rte_pktmbuf_pkt_len(mbuf);

    // Perform protocol detection
    ndpi_protocol protocol = ndpi_detection_process_packet(ndpi_module, flow, packet_data, packet_len, 0, NULL);
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Detected protocol: %u, application: %u", protocol.protocol, protocol.app_protocol);
    log_message("INFO", log_msg);

    ndpi_flow_free(flow);
}

// Cleanup nDPI
void cleanup_ndpi() {
    ndpi_exit_detection_module(ndpi_module);
}