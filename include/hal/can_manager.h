#ifndef CAN_H
#define CAN_H

#include <cstdint> // For integer types like uint32_t

// A simple struct to represent a CAN frame
struct CANFrame {
    uint32_t id;      // The CAN ID of the message
    uint8_t data[8];  // The 8-byte data payload
    uint8_t len;      // Data length (0-8)
};

class CANInterface {
public:
    bool init(const char* port_name);

    bool send(const CANFrame& frame);

    bool receive(CANFrame& out_frame);


private:

    int can_socket_;
};

#endif