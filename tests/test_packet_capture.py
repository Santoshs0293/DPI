import subprocess
import logging

logging.basicConfig(filename='dpi_log.txt', level=logging.INFO)

def test_packet_capture():
    try:
        # Run DPI engine for a short period
        result = subprocess.run(['sudo', './src/dpi_engine'], timeout=10, capture_output=True, text=True)
        logging.info("Packet capture test completed")
        assert "Received packet" in result.stdout, "Packet capture failed"
        logging.info("Packet capture test passed")
    except subprocess.TimeoutExpired:
        logging.error("Packet capture test timed out")
    except AssertionError as e:
        logging.error(f"Packet capture test failed: {e}")

if __name__ == "__main__":
    test_packet_capture()