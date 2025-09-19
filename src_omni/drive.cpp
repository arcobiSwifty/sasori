#include "Arduino.h"
// #include "MPU9250.h"
#include "drive.h"
#include <math.h>
#include <algorithm>

#define MOTA_DIR 4
#define MOTA_PWM 15
#define MOTB_DIR 22
#define MOTB_PWM 23
#define MOTC_DIR 13
#define MOTC_PWM 12

Robot::Robot()
{
}

void Robot::update()
{
    // get data from sensors
    // if (mpu.update()) {
    // float measuredYaw = mpu.getYaw();
    //  Serial.println(mpu.getYaw());
    // perry.yaw = measuredYaw;
    // }
    // pid on values
    perry.move();
}
void Robot::setup()
{
    pinMode(MOTA_DIR, OUTPUT);
    pinMode(MOTB_DIR, OUTPUT);
    pinMode(MOTC_DIR, OUTPUT);
    pinMode(MOTA_PWM, OUTPUT);
    pinMode(MOTB_PWM, OUTPUT);
    pinMode(MOTC_PWM, OUTPUT);
    lastMove = millis();
}

void get_abs(const float A[3][1], float B[3][1])
{
    for (int i = 0; i < 3; i++)
    {
        B[i][0] = abs(A[i][0]);
    }
}

float toRadians(float degrees)
{
    return degrees * 2 * PI / 360.0;
}

void Robot::move()
{

    unsigned long current_time = millis();
    if (current_time - this->lastMove < 10)
    {
        return;
    }
    else
    {
        lastMove = current_time;
    }

    if ((abs(target_vx) > 0.0) || (abs(target_vy) > 0.0))
    {

        const float SQRT3_DIV_2 = 0.86602540378;
        // float w = target_yaw - yaw / 2.0;
        float w = 0.0;
        /*
        Serial.print(target_vx);
        Serial.print("x");
        Serial.print(target_vy);
        Serial.println("y");*/

        float theta1 = toRadians(0.0 + yaw);
        float theta2 = toRadians(120.0 + yaw);
        float theta3 = toRadians(240.0 + yaw);

        // Use the inverse kinematics formula for each wheel
        double v_wheel1 = -target_vx * sin(theta1) + target_vy * cos(theta1) + w;
        double v_wheel2 = -target_vx * sin(theta2) + target_vy * cos(theta2) + w;
        double v_wheel3 = -target_vx * sin(theta3) + target_vy * cos(theta3) + w;

        float v1_t = target_vy + w;
        float v2_t = -SQRT3_DIV_2 * target_vx - 0.5 * target_vy + w;
        float v3_t = SQRT3_DIV_2 * target_vx - 0.5 * target_vy + w;
        /*
        Serial.print(v1_t);
        Serial.print(", ");
        Serial.print(v3_t);
        Serial.print(", ");
        Serial.println(v3_t);
        */

        float abs_v1 = abs(v1_t);
        float abs_v2 = abs(v2_t);
        float abs_v3 = abs(v3_t);
        float biggest = max({abs_v1, abs_v2, abs_v3})*1.2;
        if (biggest > 0)
        {
            v1_t /= biggest;
            v2_t /= biggest;
            v3_t /= biggest;
            abs_v1 /= biggest;
            abs_v2 /= biggest;
            abs_v3 /= biggest;
        }
        else
        {
            v1_t = 0;
            v2_t = 0;
            v3_t = 0;
            abs_v1 = 0;
            abs_v2 = 0;
            abs_v3 = 0;
        }

        digitalWrite(MOTA_DIR, (v1_t >= 0) ? 1 : 0);
        digitalWrite(MOTB_DIR, (v2_t >= 0) ? 1 : 0);
        digitalWrite(MOTC_DIR, (v3_t >= 0) ? 1 : 0);
        analogWrite(MOTA_PWM, abs_v1 * 255);
        analogWrite(MOTB_PWM, abs_v2 * 255);
        analogWrite(MOTC_PWM, abs_v3 * 255);
    }
    else
    {
        analogWrite(MOTA_PWM, 0);
        analogWrite(MOTB_PWM, 0);
        analogWrite(MOTC_PWM, 0);
    }
}
