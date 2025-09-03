#ifndef CANDATA 
#define CANDATA

#include <cstdint> 

struct __attribute__((packed)) SwerveUpdateData { 
    int16_t rx;
    int16_t ry;
    int16_t lx;
    int16_t ly;
};

struct __attribute__((packed)) SwerveModulePositionChange {
    int32_t dx; 
    int32_t dy;
};

struct __attribute__((packed))RobotPosData {
    int32_t absPosX;
    int32_t absPosY;
};

struct __attribute__((packed))RobotRotData {
    int32_t theta;
};

#endif