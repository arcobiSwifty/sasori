#ifndef ARM_H
#define ARM_H

class ArmStepper{
    int DirPin;
    int StepPin;
    int SleepPin;
};

class Gripper {
    int pwmPin;
};

class Arm {
    public:
        void setup();
        void pause();
        void goToPos(float x, float y, float theta);
        void grip();
        void release();

    private:
        ArmStepper primaryArmMot;
        ArmStepper seconArmdMot;
        ArmStepper turretMot;
        Gripper gripper;
};



#endif