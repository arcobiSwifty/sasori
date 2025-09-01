#ifndef CANDATA 
#define CANDATA

struct __attribute__((packed)) SwerveUpdateData { //using ints instead of double to save on space
    int rx;
    int ry;
    int lx;
    int ly;
};
struct __attribute__((packed))SwervePosData {
    float absPosX;
    float absPosY;
};

#endif