#ifndef KALMAN 
#define KALMAN

#include "utils/matrix6.h"

class EKF {
public:
    EKF();

    // Initialize state and covariance
    void init(float initial_x, float initial_y, float initial_yaw);

    // dt in seconds
    // Inputs: vx_robot_body, vy_robot_body, vyaw_robot_body are estimated from wheel odometry/IMU gyro
    void predict(float dt, float vx_robot_body, float vy_robot_body, float vyaw_robot_body);

    // Update step for Position (X,Y) measurement (e.g., from GPS, if available)
    // For this example, we don't have direct XY measurements, so we'll rely on odometry
    // fused with yaw.
    // However, if you had a sensor giving global X, Y (like an AprilTag detector or GPS),
    // you would call this.
    // void updatePosition(float measured_x, float measured_y, float R_pos_x, float R_pos_y);

    // Update step for Yaw measurement (e.g., from an accurate compass/magnetometer)
    // NOTE: An MPU6050 does NOT provide absolute yaw. An MPU9250 or external compass would.
    void updateYaw(float measured_yaw, float R_yaw);

    // Update step for a direct robot linear velocity in body frame (e.g., from high-pass filtered accelerometer)
    // Or for fusing with wheel odometry directly.
    void updateBodyVelocity(float measured_vx_body, float measured_vy_body, float R_vx, float R_vy);

    // Accessors for estimated state
    float getX() { return x_hat[0]; }
    float getY() { return x_hat[1]; }
    float getYaw() { return x_hat[2]; }
    float getVxBody() { return x_hat[3]; }
    float getVyBody() { return x_hat[4]; }
    float getVyawBody() { return x_hat[5]; }

private:
    float x_hat[STATE_SIZE];     // State estimate: [x, y, yaw, vx_body, vy_body, vyaw_body]
    float P[STATE_SIZE][STATE_SIZE]; // Covariance matrix

    // Process Noise Covariance (Q) - how much we trust our process model
    // This needs careful tuning!
    float Q[STATE_SIZE][STATE_SIZE];

    // Temporary matrices for calculations
    float F[STATE_SIZE][STATE_SIZE];     // Jacobian of the process model
    float FT[STATE_SIZE][STATE_SIZE];    // F Transpose
    float H[STATE_SIZE][STATE_SIZE];     // Jacobian of the measurement model (for full state, or smaller for specific measurements)
    float HT[STATE_SIZE][STATE_SIZE];    // H Transpose
    float R[STATE_SIZE][STATE_SIZE];     // Measurement Noise Covariance (R)
    float temp_P[STATE_SIZE][STATE_SIZE];
    float temp_F_P[STATE_SIZE][STATE_SIZE];
    float K[STATE_SIZE][STATE_SIZE];     // Kalman Gain

    // Helper to wrap yaw angle to -PI to PI
    float wrapAngle(float angle);
};

const float PI = 3.14159


#endif