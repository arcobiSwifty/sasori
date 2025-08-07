struct BlockPos {
    float angle; 
    float offsetX;
    float distanceY;
    bool error;
};
struct LinePos {
    float angle; 
    float offsetX;
    bool error;
};



class Camera {

    void initialize(); //activate. get field orientation from first image processing.
    
    BlockPos getBlockPosition();

    LinePos getLinePosition();

};