#!/bin/bash
# Start DPI engine in the background
sudo ./src/dpi_engine &
DPI_PID=$!

# Run traffic generator (TRex example)
sudo trex -f cap.yaml -m 10

# Monitor performance
sudo top -p $DPI_PID -b -n 1 > performance.txt
sudo tcpdump -i eth0 -w output.pcap &

# Wait for 60 seconds
sleep 60

# Stop processes
sudo kill $DPI_PID
sudo pkill tcpdump

echo "Performance test complete. Check performance.txt and output.pcap"