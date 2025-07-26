#pragma once

namespace Constants {

    constexpr double WHEEL_DIAMETER_MM = 67.0; // mm
    constexpr double ROBOT_RADIUS = 120; // mm

    namespace ArmPID {
        constexpr double kP = 0.5;
        constexpr double kI = 0.01;
        constexpr double kD = 0.1;
    }

}