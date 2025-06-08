import numpy as np
from sklearn.ensemble import RandomForestClassifier
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv1D, MaxPooling1D, Dense, Flatten
import logging

logging.basicConfig(filename='dpi_log.txt', level=logging.INFO)

# Random Forest model for lightweight classification
def train_random_forest(X_train, y_train):
    rf_model = RandomForestClassifier(n_estimators=100, random_state=42)
    rf_model.fit(X_train, y_train)
    logging.info("Random Forest model trained")
    return rf_model

# CNN model for encrypted traffic classification
def build_cnn_model(input_shape):
    model = Sequential([
        Conv1D(64, 3, activation='relu', input_shape=input_shape),
        MaxPooling1D(2),
        Conv1D(32, 3, activation='relu'),
        MaxPooling1D(2),
        Flatten(),
        Dense(128, activation='relu'),
        Dense(10, activation='softmax')  # Assuming 10 application classes
    ])
    model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
    logging.info("CNN model built")
    return model

# Extract statistical features
def extract_features(packets):
    packet_sizes = [len(pkt['data']) for pkt in packets]
    inter_arrival_times = [packets[i+1]['timestamp'] - packets[i]['timestamp'] for i in range(len(packets)-1)] if len(packets) > 1 else [0]
    entropy = -sum(p * np.log2(p) for p in np.bincount(packet_sizes) / len(packet_sizes) if p > 0)
    features = np.array([np.mean(packet_sizes), np.std(packet_sizes), np.mean(inter_arrival_times), entropy])
    logging.info(f"Extracted features: {features}")
    return features

# Classify traffic
def classify_traffic(packets, rf_model, cnn_model):
    features = extract_features(packets)
    rf_pred = rf_model.predict([features])[0]
    cnn_pred = cnn_model.predict(features.reshape(1, -1, 1))
    logging.info(f"RF prediction: {rf_pred}, CNN prediction: {np.argmax(cnn_pred, axis=1)[0]}")
    return rf_pred, np.argmax(cnn_pred, axis=1)[0]

# Example usage
if __name__ == "__main__":
    # Mock data for testing
    packets = [{'data': b'abc', 'timestamp': 1.0}, {'data': b'def', 'timestamp': 1.2}]
    X_train = np.array([[100, 10, 0.2, 1.5], [200, 20, 0.3, 1.7]])  # Example training data
    y_train = np.array([0, 1])  # Example labels
    rf_model = train_random_forest(X_train, y_train)
    cnn_model = build_cnn_model((4, 1))
    rf_pred, cnn_pred = classify_traffic(packets, rf_model, cnn_model)