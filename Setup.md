# DPI Project Setup and Analysis Guide

## Overview

This guide provides step-by-step instructions to set up, run, and analyze the performance of a Deep Packet Inspection (DPI) system for high-speed internet traffic (10/40 Gbps). The system uses DPDK for packet capture, nDPI for protocol dissection, Intel Hyperscan for pattern matching, Scikit-learn/TensorFlow for ML-based classification of encrypted traffic, and Suricata for anomaly detection. It supports metadata and behavioral analysis to inspect encrypted data without decryption, meeting the project’s requirements.

## Prerequisites

### Hardware:

* Multi-core CPU (e.g., Intel Xeon or AMD EPYC).
* DPDK-compatible NIC (e.g., Intel X710, Mellanox ConnectX-5).
* 16 GB RAM, 100 GB storage.
* Optional: GPU (e.g., NVIDIA RTX 2080) for future ML acceleration.

### Software:

* Ubuntu 20.04.
* VS Code with extensions: C/C++ (Microsoft), Python (Microsoft), YAML (Red Hat).
* Dependencies: DPDK (v21.11), nDPI, Hyperscan, Scikit-learn, TensorFlow, NumPy, PyYAML, Suricata, libyaml.

### Network:

* Access to a network interface or testbed with 10/40 Gbps traffic.
* Traffic generator (e.g., TRex) for testing.

## File Structure

```
dpi_project/
├── src/
│   ├── packet_capture.c           # DPDK-based packet capture
│   ├── protocol_dissection.c      # nDPI-based metadata extraction
│   ├── pattern_matching.c         # Hyperscan-based pattern matching
│   ├── ml_classification.py       # ML-based application identification
│   ├── anomaly_detection.py       # Suricata-based anomaly detection
│   ├── dpi_engine.c               # Main DPI engine
│   ├── utils/
│   │   ├── logging.c              # Logging utilities
│   │   ├── config_parser.c        # YAML configuration parser
├── config/
│   ├── dpi_config.yaml            # DPI engine configuration
│   ├── suricata.yaml              # Suricata configuration
├── scripts/
│   ├── setup.sh                   # Installs dependencies
│   ├── run_dpi.sh                 # Runs DPI engine
│   ├── test_performance.sh        # Tests performance
├── tests/
│   ├── test_packet_capture.py     # Tests packet capture
│   ├── test_protocol_dissection.py # Tests protocol dissection
│   ├── test_ml_classification.py  # Tests ML classification
├── docs/
│   ├── README.md                  # Project documentation
│   ├── api_docs.md                # API documentation (placeholder)
├── .vscode/
│   ├── launch.json                # VS Code debug configuration
│   ├── tasks.json                 # VS Code build tasks
│   ├── settings.json              # VS Code project settings
├── requirements.txt               # Python dependencies
```

## Setup Instructions

### 1. Install OS and Tools

```bash
sudo apt-get update
sudo apt-get install -y code
code --install-extension ms-vscode.cpptools
code --install-extension ms-python.python
code --install-extension redhat.vscode-yaml
```

### 2. Create Project Directory

```bash
mkdir dpi_project
cd dpi_project
mkdir -p src/utils config scripts tests docs .vscode
```

### 3. Copy Source Files

Place files into respective directories as per file structure.

### 4. Install Dependencies

```bash
chmod +x scripts/setup.sh
./scripts/setup.sh
pip3 install -r requirements.txt

sudo mkdir -p /etc/suricata/rules
echo 'alert ip any any -> any any (msg:"Suspicious traffic"; sid:1000001; rev:1;)' | sudo tee /etc/suricata/rules/dpi.rules
```

### 5. Configure the NIC

```bash
sudo dpdk-devbind.py --status
sudo modprobe uio_pci_generic
sudo dpdk-devbind.py -b uio_pci_generic <NIC_DEVICE_ID>
```

Update `config/dpi_config.yaml` with correct port and paths.

### 6. Build the DPI Engine

In VS Code:

* Open the project folder.
* Press `Ctrl+Shift+B` to build.

Or manually:

```bash
cd src
gcc -o dpi_engine dpi_engine.c packet_capture.c protocol_dissection.c pattern_matching.c utils/logging.c utils/config_parser.c -lrte_eal -lrte_mbuf -lrte_ethdev -lndpi -lhs -lyaml
```

## Running the DPI System

### 1. Run the DPI Engine

```bash
./scripts/run_dpi.sh
```

Or debug via VS Code (`F5`).

### 2. Run ML Classification

```bash
echo "packet_size,inter_arrival_time,entropy,label" > packets.csv
echo "100,0.2,1.5,0" >> packets.csv
echo "200,0.3,1.7,1" >> packets.csv
python3 src/ml_classification.py
```

### 3. Run Anomaly Detection

```bash
python3 src/anomaly_detection.py
```

Generates `anomaly_report.json`.

## Performance Analysis

### 1. Setup Traffic Generator

```bash
sudo apt-get install -y trex
```

Create `cap.yaml` with traffic config.

### 2. Run Performance Test

```bash
./scripts/test_performance.sh
```

### 3. Analyze Results

* **Throughput:**

```bash
grep "Received packet" dpi_log.txt | wc -l
```

Expected: \~10 Gbps (may vary).

* **Latency:**

```bash
grep "Packet processing time" dpi_log.txt
```

* **Accuracy:**

```bash
sudo wireshark -r output.pcap
```

Compare nDPI results.

* **Resource Usage:**

```bash
cat performance.txt
sudo htop
```

### Encrypted Traffic Inspection

```bash
curl https://example.com
python3 tests/test_ml_classification.py
```

Expected: Encrypted traffic identified via metadata.

### 4. Run Unit Tests

```bash
python3 tests/test_packet_capture.py
python3 tests/test_protocol_dissection.py
python3 tests/test_ml_classification.py
```

## Capacities and Limitations

### Capacities

* Encrypted Traffic Inspection via nDPI + ML.
* \~10 Gbps throughput, scalable to 40 Gbps.
* Over 250 protocols.
* Suricata for anomaly detection.
* Multi-core DPDK support.

### Limitations

* Requires labeled ML dataset.
* No Python-C integration for ML.
* No FPGA/GPU acceleration.
* Partial Suricata integration.
* No XAI support.

## Recommendations

* Use datasets like CICIDS2017.
* Integrate ML using `ctypes` or socket IPC.
* Add CUDA/OpenCL acceleration.
* Deploy on 40 Gbps testbed.

## Conclusion

This guide covers the setup, execution, and analysis of the DPI system, enabling encrypted traffic inspection without decryption.

**Date:** June 08, 2025
