#ifndef CAM
#define CAM

#include "hal/can_data.h"


class Camera {
    public:

    void setup(); //activate. get field orientation from first image processing.

    bool requestReceived = false;

    static void canReceive();

    void sendBlockPosition();
    
    BlockPos getBlockPosition(int blockType);

    LinePos getLinePosition();

};

extern Camera cam;

#endif