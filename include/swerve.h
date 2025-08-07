#ifndef SWERVE_H
#define SWERVE_H



class SwerveModule {
    public:
        const int canID;
        float relPosX;
        float relPosY;
        float relAngle; //updated through can bus
};

class SwerveDrive {
    SwerveModule modules[3];
    const float wheel_to_center = 80; //update

    void setSpeed(float Vxf, float Vyf, float omega);

    void stop();
    
};

#endif