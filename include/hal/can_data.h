#ifndef CANDATA 
#define CANDATA

struct __attribute__((packed)) SwerveUpdateData {
    double targetRelSpeed;
    double targetRelAngle;
    double targetAngleSpeed;
};
struct __attribute__((packed))SwervePosData {
    float absPosX;
    float absPosY;
};

#endif