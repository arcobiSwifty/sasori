#ifndef CANDATA 
#define CANDATA

struct __attribute__((packed)) SwerveUpdateData { //using ints instead of double to save on space
    char rx;
    char ry;
    char lx;
    char ly;
};
struct __attribute__((packed))SwervePosData {
    float absPosX;
    float absPosY;
};

#endif