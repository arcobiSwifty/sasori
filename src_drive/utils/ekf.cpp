#include "utils/ekf.h"
#include <math.h>


EKF::EKF() {
    for (int i = 0; i < STATE_SIZE; i++) {
        x_hat[i] = 0.0;
    }

    for (int i = 0; i < STATE_SIZE; i++) {
        for (int j = 0; j < STATE_SIZE; j++) {
            P[i][j] = 0.0;
        }
        P[i][i] = 1000.0; // High initial uncertainty
    }

    // Initialize process noise Q TODO: tune
    // Higher values mean more trust in new sensor data over model prediction.
    // x, y, yaw, vx, vy, vyaw
    float q_diag[STATE_SIZE] = {
        0.001,  // x
        0.001,  // y
        0.005,  // yaw (can drift more)
        0.01,   // vx_body
        0.01,   // vy_body
        0.005   // vyaw_body
    };

    for (int i = 0; i < STATE_SIZE; i++) {
        for (int j = 0; j < STATE_SIZE; j++) {
            Q[i][j] = 0.0;
        }
        Q[i][i] = q_diag[i];
    }
}

void EKF::init(float initial_x, float initial_y, float initial_yaw) {
    x_hat[0] = initial_x;
    x_hat[1] = initial_y;
    x_hat[2] = wrapAngle(initial_yaw);
    // Velocities can start at zero or be initialized with first odometry reading.
    x_hat[3] = 0.0;
    x_hat[4] = 0.0;
    x_hat[5] = 0.0;

    // Reset covariance
    for (int i = 0; i < STATE_SIZE; i++) {
        for (int j = 0; j < STATE_SIZE; j++) {
            P[i][j] = 0.0;
        }
        P[i][i] = 1.0; // Moderate initial uncertainty after explicit init
    }
}

float EKF::wrapAngle(float angle) {
    while (angle > PI) angle -= 2 * PI;
    while (angle < -PI) angle += 2 * PI;
    return angle;
}

// Prediction Step
void EKF::predict(float dt, float vx_robot_body, float vy_robot_body, float vyaw_robot_body) {
    float yaw = x_hat[2];
    float cos_yaw = cos(yaw);
    float sin_yaw = sin(yaw);

    // 1. Predict the next state (x_hat_prime) based on the process model
    // x_k = f(x_{k-1}, u_k)
    // Non-linear function:
    // x = x + (vx_body * cos(yaw) - vy_body * sin(yaw)) * dt
    // y = y + (vx_body * sin(yaw) + vy_body * cos(yaw)) * dt
    // yaw = yaw + vyaw_body * dt
    // vx_body = vx_body (assume constant if no acceleration input)
    // vy_body = vy_body (assume constant)
    // vyaw_body = vyaw_body (assume constant)

    x_hat[0] += (vx_robot_body * cos_yaw - vy_robot_body * sin_yaw) * dt;
    x_hat[1] += (vx_robot_body * sin_yaw + vy_robot_body * cos_yaw) * dt;
    x_hat[2] += vyaw_robot_body * dt;
    x_hat[2] = wrapAngle(x_hat[2]);

    // We assume that the robot body velocities are "inputs" from odometry
    // If you were using IMU accelerations, these would also be part of the prediction.
    // For now, we'll keep them as estimated states.
    x_hat[3] = vx_robot_body; // Update our belief about current velocities
    x_hat[4] = vy_robot_body;
    x_hat[5] = vyaw_robot_body;


    // 2. Compute the Jacobian of the process model (F)
    // F is df/dx_hat evaluated at current x_hat and u.
    // x = x + (vx_body * cos(yaw) - vy_body * sin(yaw)) * dt
    // y = y + (vx_body * sin(yaw) + vy_body * cos(yaw)) * dt
    // yaw = yaw + vyaw_body * dt
    // vx_body = vx_body
    // vy_body = vy_body
    // vyaw_body = vyaw_body

    // F =
    // | 1   0   (-vx_body*sin(yaw) - vy_body*cos(yaw))*dt   cos(yaw)*dt   -sin(yaw)*dt   0 |
    // | 0   1   (vx_body*cos(yaw) - vy_body*sin(yaw))*dt    sin(yaw)*dt   cos(yaw)*dt    0 |
    // | 0   0   1                                           0             0              dt |
    // | 0   0   0                                           1             0              0  |
    // | 0   0   0                                           0             1              0  |
    // | 0   0   0                                           0             0              1  |

    matrix_identity(F, STATE_SIZE); // Start with Identity
    F[0][2] = (-vx_robot_body * sin_yaw - vy_robot_body * cos_yaw) * dt;
    F[0][3] = cos_yaw * dt;
    F[0][4] = -sin_yaw * dt;

    F[1][2] = (vx_robot_body * cos_yaw - vy_robot_body * sin_yaw) * dt;
    F[1][3] = sin_yaw * dt;
    F[1][4] = cos_yaw * dt;

    F[2][5] = dt; // yaw = yaw + vyaw_body * dt (this assumes vyaw_body is an input, not a state)
                  // If vyaw_body were a state that changes based on acceleration, this would be different.
                  // For a simplified EKF, we treat the direct gyro rate as the 'input' for yaw_dot.


    // 3. Predict the next covariance (P_prime)
    // P_k = F_k * P_{k-1} * F_k^T + Q
    matrix_transpose(F, FT, STATE_SIZE, STATE_SIZE);
    matrix_multiply(F, P, temp_F_P, STATE_SIZE);
    matrix_multiply(temp_F_P, FT, P, STATE_SIZE); // P = F * P * FT
    matrix_add(P, Q, P, STATE_SIZE);             // P = P + Q
}

// =========================================================================
// Measurement Update Steps
// =========================================================================

// Update for Yaw measurement (e.g., from an accurate compass/magnetometer)
void EKF::updateYaw(float measured_yaw, float R_yaw) {
    // Measurement vector z = [measured_yaw]
    // Predicted measurement h(x_hat) = [x_hat[2]] (current estimated yaw)
    // y = z - h(x_hat)

    float y = wrapAngle(measured_yaw - x_hat[2]);

    // H: Jacobian of the measurement model h(x_hat) = [x_hat[2]]
    // H = [0 0 1 0 0 0]
    for (int i = 0; i < STATE_SIZE; i++) {
        for (int j = 0; j < STATE_SIZE; j++) {
            H[i][j] = 0.0; // Clear H
        }
    }
    H[0][2] = 1.0; // Only yaw (index 2) is measured

    // R: Measurement noise covariance for yaw
    float R_mat[1][1] = {{R_yaw}}; 

    // Kalman Gain K = P * H^T * (H * P * H^T + R)^-1
    // (H * P * H^T + R) will be 1x1, so its inverse is just 1/value

    // Calculate S = H * P * H^T + R
    float HT_yaw[STATE_SIZE][1] = {{0},{0},{1},{0},{0},{0}}; // Transpose of H for yaw measurement
    float P_H_T[STATE_SIZE][1]; // P * H^T (6x1)
    matrix_multiply_vec(P, HT_yaw[0], P_H_T[0], STATE_SIZE, 1); // Specific multiply for 6x6 * 6x1

    float H_P_H_T = 0; // Will be 1x1
    for(int i=0; i<STATE_SIZE; ++i) {
        H_P_H_T += H[0][i] * P_H_T[i][0];
    }

    float S = H_P_H_T + R_yaw;

    // K = P * H^T * inv(S)
    float inv_S = 1.0 / S;
    float K_vec[STATE_SIZE]; // Kalman Gain will be 6x1
    for(int i=0; i<STATE_SIZE; ++i) {
        K_vec[i] = P_H_T[i][0] * inv_S;
    }

    // Update state estimate: x_hat = x_hat + K * y
    for(int i=0; i<STATE_SIZE; ++i) {
        x_hat[i] += K_vec[i] * y;
    }
    x_hat[2] = wrapAngle(x_hat[2]); // Re-wrap yaw

    // Update covariance estimate: P = (I - K * H) * P
    float I_minus_KH[STATE_SIZE][STATE_SIZE];
    matrix_identity(I_minus_KH, STATE_SIZE);
    float K_H[STATE_SIZE][STATE_SIZE]; // K is 6x1, H is 1x6. K*H is 6x6.
    for(int i=0; i<STATE_SIZE; ++i) {
        for(int j=0; j<STATE_SIZE; ++j) {
            K_H[i][j] = K_vec[i] * H[0][j]; // K_vec is 6x1, H[0] is 1x6
        }
    }
    matrix_subtract(I_minus_KH, K_H, I_minus_KH, STATE_SIZE);
    matrix_multiply(I_minus_KH, P, P, STATE_SIZE);
}


// Update for direct robot body velocities (e.g., from high-pass filtered IMU accelerations,
// or directly derived from combined wheel odometry)
void EKF::updateBodyVelocity(float measured_vx_body, float measured_vy_body, float R_vx, float R_vy) {
    // Measurement vector z = [measured_vx_body, measured_vy_body]^T
    // Predicted measurement h(x_hat) = [x_hat[3], x_hat[4]]^T
    // y = z - h(x_hat)

    float y[2];
    y[0] = measured_vx_body - x_hat[3];
    y[1] = measured_vy_body - x_hat[4];

    // H: Jacobian of the measurement model
    // h(x_hat) = [x_hat[3], x_hat[4]]^T
    // H =
    // | 0 0 0 1 0 0 |
    // | 0 0 0 0 1 0 |
    // (This is a 2x6 matrix)
    float H_vel[2][STATE_SIZE]; // H for velocity
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < STATE_SIZE; j++) {
            H_vel[i][j] = 0.0;
        }
    }
    H_vel[0][3] = 1.0; // vx_body is at index 3
    H_vel[1][4] = 1.0; // vy_body is at index 4

    float R_vel[2][2] = {{R_vx, 0.0}, {0.0, R_vy}}; // Measurement noise for vx, vy

    // Calculate S = H * P * H^T + R
    // H is 2x6, P is 6x6, H^T is 6x2. Result is 2x2.
    float HT_vel[STATE_SIZE][2];
    matrix_transpose(H_vel, HT_vel, 2, STATE_SIZE); // Transpose H_vel from 2x6 to 6x2

    float P_HT_vel[STATE_SIZE][2]; // P * H^T (6x6 * 6x2 = 6x2)
    for(int i=0; i<STATE_SIZE; ++i) {
        for(int j=0; j<2; ++j) {
            P_HT_vel[i][j] = 0;
            for(int k=0; k<STATE_SIZE; ++k) {
                P_HT_vel[i][j] += P[i][k] * HT_vel[k][j];
            }
        }
    }

    float H_P_HT_vel[2][2]; // H * (P * H^T) (2x6 * 6x2 = 2x2)
    for(int i=0; i<2; ++i) {
        for(int j=0; j<2; ++j) {
            H_P_HT_vel[i][j] = 0;
            for(int k=0; k<STATE_SIZE; ++k) {
                H_P_HT_vel[i][j] += H_vel[i][k] * P_HT_vel[k][j];
            }
        }
    }

    float S_vel[2][2];
    matrix_add(H_P_HT_vel, R_vel, S_vel, 2); // S = H P H^T + R (2x2)

    // Calculate inverse of S_vel (for a 2x2 matrix)
    float det_S = S_vel[0][0] * S_vel[1][1] - S_vel[0][1] * S_vel[1][0];
    if (abs(det_S) < 1e-6) {
        // division by zero. implement fail safe
        return;
    }
    float inv_S_vel[2][2];
    inv_S_vel[0][0] = S_vel[1][1] / det_S;
    inv_S_vel[0][1] = -S_vel[0][1] / det_S;
    inv_S_vel[1][0] = -S_vel[1][0] / det_S;
    inv_S_vel[1][1] = S_vel[0][0] / det_S;

    // Kalman Gain K = (P * H^T) * inv(S) (6x2 * 2x2 = 6x2)
    float K_vel[STATE_SIZE][2];
    for(int i=0; i<STATE_SIZE; ++i) {
        for(int j=0; j<2; ++j) {
            K_vel[i][j] = 0;
            for(int k=0; k<2; ++k) {
                K_vel[i][j] += P_HT_vel[i][k] * inv_S_vel[k][j];
            }
        }
    }

    // Update state estimate: x_hat = x_hat + K * y (6x1 + 6x2 * 2x1 = 6x1)
    for(int i=0; i<STATE_SIZE; ++i) {
        x_hat[i] += K_vel[i][0] * y[0] + K_vel[i][1] * y[1];
    }
    x_hat[2] = wrapAngle(x_hat[2]); // Re-wrap yaw

    // Update covariance estimate: P = (I - K * H) * P
    // I_minus_KH is 6x6, K is 6x2, H is 2x6. K*H is 6x6.
    float I_minus_KH_vel[STATE_SIZE][STATE_SIZE];
    matrix_identity(I_minus_KH_vel, STATE_SIZE);
    float K_H_vel[STATE_SIZE][STATE_SIZE];
    for(int i=0; i<STATE_SIZE; ++i) {
        for(int j=0; j<STATE_SIZE; ++j) {
            K_H_vel[i][j] = 0;
            for(int k=0; k<2; ++k) {
                K_H_vel[i][j] += K_vel[i][k] * H_vel[k][j];
            }
        }
    }
    matrix_subtract(I_minus_KH_vel, K_H_vel, I_minus_KH_vel, STATE_SIZE);
    matrix_multiply(I_minus_KH_vel, P, P, STATE_SIZE);
}
