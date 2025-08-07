#ifndef ARM_H
#define ARM_H

class ArmStepper{
    const int DirPin;
    const int StepPin;
    const int SleepPin;
};

class Gripper {
    const int pwmPin;
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