#!/bin/bash

# Install dependencies
sudo apt-get update
sudo apt-get install -y build-essential libpcap-dev python3-pip libyaml-dev
pip3 install -r requirements.txt

# Install DPDK
wget https://fast.dpdk.org/rel/dpdk-21.11.tar.xz
tar -xf dpdk-21.11.tar.xz
cd dpdk-21.11
make config T=x86_64-native-linuxapp-gcc
make install
cd ..

# Install nDPI
git clone https://github.com/ntop/nDPI.git
cd nDPI
./autogen.sh
./configure
make
sudo make install
cd ..

# Install Hyperscan
git clone https://github.com/intel/hyperscan.git
cd hyperscan
mkdir build
cd build
cmake ..
make
sudo make install
cd ../..

# Install Suricata
sudo apt-get install -y suricata

# Build DPI engine
cd src
gcc -o dpi_engine dpi_engine.c packet_capture.c protocol_dissection.c pattern_matching.c utils/logging.c utils/config_parser.c -lrte_eal -lrte_mbuf -lrte_ethdev -lndpi -lhs -lyaml

echo "Setup complete. Run './scripts/run_dpi.sh' to start."