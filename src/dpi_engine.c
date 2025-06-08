#include <rte_eal.h>
#include <rte_mbuf.h>
#include <time.h>
#include "protocol_dissection.h"
#include "pattern_matching.h"
#include "utils/logging.h"
#include "utils/config_parser.h"

void process_packet(struct rte_mbuf *mbuf) {
    clock_t start = clock();
    dissect_packet(mbuf);
    char *data = rte_pktmbuf_mtod(mbuf, char *);
    uint32_t len = rte_pktmbuf_pkt_len(mbuf);
    match_packet(data, len);
    clock_t end = clock();
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Packet processing time: %f ms", ((double)(end - start) * 1000) / CLOCKS_PER_SEC);
    log_message("DEBUG", log_msg);
}

int main(int argc, char *argv[]) {
    // Initialize logging
    init_logging("dpi_log.txt");

    // Parse configuration
    Config config = parse_config("config/dpi_config.yaml");
    log_message("INFO", "Configuration parsed");

    // Initialize DPDK
    if (init_dpdk(argc, argv) != 0) {
        log_message("ERROR", "Failed to initialize DPDK");
        return -1;
    }

    // Initialize nDPI and Hyperscan
    init_ndpi();
    if (init_hyperscan() != 0) {
        log_message("ERROR", "Failed to initialize Hyperscan");
        return -1;
    }

    // Packet capture loop
    capture_packets(process_packet);

    // Cleanup
    cleanup_ndpi();
    cleanup_hyperscan();
    cleanup_logging();
    return 0;
}