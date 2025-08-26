#ifndef CANDATA 
#define CANDATA

struct __attribute__((packed)) SwerveUpdateData {
    float targetRelSpeed;
    float targetRelAngle;
    float robotPosX;
    float robotPosY;
    float robotAngle;
};
struct __attribute__((packed))SwervePosData {
    float absPosX;
    float absPosY;
    float relAngle;
};

#endif