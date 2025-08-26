#include <ESP32-TWAI-CAN.hpp>

// Default for ESP32
#define CAN_TX 37
#define CAN_RX 36

// Define the structure to hold our data.
// The __attribute__((packed)) ensures that the compiler doesn't add any padding bytes,
// which is important for consistent serialization.
// The total size should be <= 8 bytes for a standard CAN frame.
struct __attribute__((packed)) SensorData {
    float temperature; // 4 bytes
    int counter;       // 4 bytes
};

CanFrame rxFrame;
SensorData myData;

// Function to serialize and send the SensorData struct
void sendSensorData(const SensorData &data) {
    CanFrame txFrame = {0};
    txFrame.identifier = 0x123; // Choose a custom identifier for your data
    txFrame.extd = 0;           // Standard frame
    txFrame.data_length_code = sizeof(SensorData); // Set DLC to the size of our struct

    // Serialize the struct into the data field of the CAN frame
    memcpy(txFrame.data, &data, sizeof(SensorData));

    // Send the frame
    if (ESP32Can.writeFrame(txFrame)) {
        Serial.println("Successfully sent SensorData frame.");
    } else {
        Serial.println("Failed to send SensorData frame.");
    }
}

void setup() {
    // Setup serial for debugging.
    Serial.begin(115200);

    // Initialize some data
    myData.temperature = 25.5;
    myData.counter = 0;

    // Set CAN bus pins
    ESP32Can.setPins(CAN_TX, CAN_RX);

    // Set custom queue sizes (optional)
    ESP32Can.setRxQueueSize(5);
    ESP32Can.setTxQueueSize(5);

    // Start the CAN bus at 500 kbps
    if (ESP32Can.begin(ESP32Can.convertSpeed(500))) {
        Serial.println("CAN bus started!");
    } else {
        Serial.println("CAN bus failed to start!");
        while(1); // Halt on failure
    }
}

void loop() {
    static uint32_t lastSendTime = 0;
    uint32_t currentTime = millis();

    // Send the struct data every second
    if (currentTime - lastSendTime > 1000) {
        lastSendTime = currentTime;

        // Update the data before sending
        myData.temperature += 0.1;
        myData.counter++;

        if(myData.temperature > 50.0) {
          myData.temperature = 25.0;
        }

        Serial.printf("Sending Temp: %.2f, Counter: %d\n", myData.temperature, myData.counter);
        sendSensorData(myData);
    }

    // Check for incoming CAN frames with a 1 second timeout
    if (ESP32Can.readFrame(rxFrame, 1000)) {
        // Check if the received frame has the identifier we expect for SensorData
        if (rxFrame.identifier == 0x123 && rxFrame.data_length_code == sizeof(SensorData)) {
            
            // Create a struct to hold the received data
            SensorData receivedData;

            // Deserialize the data from the CAN frame back into our struct
            memcpy(&receivedData, rxFrame.data, sizeof(SensorData));

            Serial.println("---");
            Serial.printf("Received frame with ID: 0x%03X\n", rxFrame.identifier);
            Serial.printf("Deserialized Temperature: %.2f °C\n", receivedData.temperature);
            Serial.printf("Deserialized Counter: %d\n", receivedData.counter);
            Serial.println("---");
        }
    }
}