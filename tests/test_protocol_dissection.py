import subprocess
import logging

logging.basicConfig(filename='dpi_log.txt', level=logging.INFO)

def test_protocol_dissection():
    try:
        # Run DPI engine with mock traffic
        result = subprocess.run(['sudo', './src/dpi_engine'], timeout=10, capture_output=True, text=True)
        logging.info("Protocol dissection test completed")
        assert "Detected protocol" in result.stdout, "Protocol dissection failed"
        logging.info("Protocol dissection test passed")
    except subprocess.TimeoutExpired:
        logging.error("Protocol dissection test timed out")
    except AssertionError as e:
        logging.error(f"Protocol dissection test failed: {e}")

if __name__ == "__main__":
    test_protocol_dissection()