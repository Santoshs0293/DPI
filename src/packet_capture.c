#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>
#include <stdio.h>
#include <stdint.h>

#define RX_RING_SIZE 1024
#define TX_RING_SIZE 1024
#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32

// Initialize DPDK environment and NIC for packet capture
static int init_dpdk(int argc, char *argv[]) {
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");
    }

    struct rte_mempool *mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS,
                                                            MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE,
                                                            rte_socket_id());
    if (mbuf_pool == NULL) {
        rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");
    }

    // Configure Ethernet device (port 0)
    struct rte_eth_conf port_conf = {0};
    ret = rte_eth_dev_configure(0, 1, 1, &port_conf);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d\n", ret);
    }

    // Setup RX and TX queues
    ret = rte_eth_rx_queue_setup(0, 0, RX_RING_SIZE, rte_socket_id(), NULL, mbuf_pool);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Cannot setup RX queue\n");
    }
    ret = rte_eth_tx_queue_setup(0, 0, TX_RING_SIZE, rte_socket_id(), NULL);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Cannot setup TX queue\n");
    }

    // Start Ethernet device
    ret = rte_eth_dev_start(0);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Cannot start device\n");
    }
    return 0;
}

// Main packet capture loop
void capture_packets(void (*process_packet)(struct rte_mbuf *)) {
    struct rte_mbuf *pkts[BURST_SIZE];
    while (1) {
        // Receive packets
        uint16_t nb_rx = rte_eth_rx_burst(0, 0, pkts, BURST_SIZE);
        for (uint16_t i = 0; i < nb_rx; i++) {
            process_packet(pkts[i]);
            rte_pktmbuf_free(pkts[i]);
        }
    }
}