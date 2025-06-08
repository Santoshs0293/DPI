import numpy as np
from src.ml_classification import train_random_forest, build_cnn_model, extract_features, classify_traffic
import logging

logging.basicConfig(filename='dpi_log.txt', level=logging.INFO)

def test_ml_classification():
    # Mock data
    packets = [{'data': b'abc', 'timestamp': 1.0}, {'data': b'def', 'timestamp': 1.2}]
    X_train = np.array([[100, 10, 0.2, 1.5], [200, 20, 0.3, 1.7]])
    y_train = np.array([0, 1])

    # Train models
    rf_model = train_random_forest(X_train, y_train)
    cnn_model = build_cnn_model((4, 1))

    # Test classification
    rf_pred, cnn_pred = classify_traffic(packets, rf_model, cnn_model)
    logging.info(f"ML classification test: RF={rf_pred}, CNN={cnn_pred}")
    assert rf_pred in [0, 1], "RF classification failed"
    assert cnn_pred in range(10), "CNN classification failed"
    logging.info("ML classification test passed")

if __name__ == "__main__":
    test_ml_classification()