#include "Arduino.h"
// #include "MPU9250.h"
#include "drive.h"
#include <math.h>
#include <algorithm>

#define MOTA_DIR 27
#define MOTA_PWM 12
#define MOTB_DIR 14
#define MOTB_PWM 33
#define MOTC_DIR 25
#define MOTC_PWM 26

float toRadians(float degrees)
{
    return degrees * 2 * PI / 360.0;
}

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
    
    if (is_jiritu) {
        unsigned long current_time = millis();
        if (fase == 0) { // go forward 1.2 seconds 
            target_yaw = 0;
            if (abs(yaw) < 2.0) {
                fase = 1;
                jiritu_start = millis();
                analogWrite(MOTA_PWM, 0);
        analogWrite(MOTB_PWM, 0);
        analogWrite(MOTC_PWM, 0);
            }
        } else if (fase == 1) {
            if (current_time - jiritu_start > 1200) {
                fase = 2; 
                analogWrite(MOTA_PWM, 0);
        analogWrite(MOTB_PWM, 0);
        analogWrite(MOTC_PWM, 0);
            }
        } else if (fase == 2) { // rotate
            target_yaw = 90;
            if (abs(yaw - target_yaw) < 2.0) {

            }

        } else if (fase == 3) { // go forward 2.5 seconds 

        } else if (fase == 4) {// rotate 90 degrees

        } else if (fase == 5) { // go forward 8.5 seconds 
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

        float jiritu_vx = 0.9;
        float jiritu_vy = 0;

        // Use the inverse kinematics formula for each wheel
        double v_wheel1 = -jiritu_vx * sin(theta1) + jiritu_vy * cos(theta1) + w;
        double v_wheel2 = -jiritu_vx * sin(theta2) + jiritu_vy * cos(theta2) + w;
        double v_wheel3 = -jiritu_vx * sin(theta3) + jiritu_vy * cos(theta3) + w;

        float v1_t = jiritu_vy + w;
        float v2_t = -SQRT3_DIV_2 * jiritu_vx - 0.5 * jiritu_vy + w;
        float v3_t = SQRT3_DIV_2 * jiritu_vx - 0.5 * jiritu_vy + w;

        float abs_v1 = abs(v1_t);
        float abs_v2 = abs(v2_t);
        float abs_v3 = abs(v3_t);
        float biggest = max({abs_v1, abs_v2, abs_v3})*2;
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

            if (current_time - jiritu_start > 6200) {
                jiritu_start = 0; 
                fase == 6;
                analogWrite(MOTA_PWM, 0);
        analogWrite(MOTB_PWM, 0);
        analogWrite(MOTC_PWM, 0);
            }
            


        } else if (fase == 6) { // rotate 90 degrees 
             analogWrite(MOTA_PWM, 0);
        analogWrite(MOTB_PWM, 0);
        analogWrite(MOTC_PWM, 0);

        } else if (fase == 7) { // forward 1.5 seconds 
            
        }
    } else {
        perry.move();
    }
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
        float biggest = max({abs_v1, abs_v2, abs_v3})*2;
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
