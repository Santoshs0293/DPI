DPI Project: Metadata Analysis & Application Identification

Overview

This project implements a Deep Packet Inspection (DPI) system for high-speed internet traffic (10/40 Gbps) using DPDK, nDPI, Hyperscan, and ML frameworks (Scikit-learn, TensorFlow). It supports metadata analysis and behavioral classification for encrypted traffic without decryption.

Setup





Prerequisites:





Ubuntu 20.04, multi-core CPU, DPDK-compatible NIC, 16 GB RAM.



Install VS Code with C/C++ and Python extensions.



Install Dependencies:

chmod +x scripts/setup.sh
./scripts/setup.sh



Configure NIC:

sudo dpdk-devbind.py --status
sudo modprobe uio_pci_generic
sudo dpdk-devbind.py -b uio_pci_generic <NIC_DEVICE_ID>



Run DPI Engine:

./scripts/run_dpi.sh



Test Performance:

./scripts/test_performance.sh

File Structure





src/: Core source code (C and Python).



config/: Configuration files.



scripts/: Setup and run scripts.



tests/: Test scripts.



docs/: Documentation.



.vscode/: VS Code configurations.

Limitations





ML classification requires a labeled dataset.



No GPU/FPGA acceleration in the current setup.



Limited to 10 Gbps; scaling to 40 Gbps requires optimization.