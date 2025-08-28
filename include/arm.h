#ifndef ARM_H
#define ARM_H

class ArmStepper{
    int DirPin;
    int StepPin;
    int SleepPin;
};

class Gripper {
    int pwmPin;
    void grip();
    void release();
};

class Arm {
    public:
        void setup();
        void pause(); //activate sleep pin in all drivers;
        void unpause(); //deactivate sleep pin in all drivers;
        void update();

    private:
        ArmStepper primaryArmMot;
        ArmStepper seconArmdMot;
        ArmStepper turretMot;
        Gripper gripper;

        volatile int posPrimary;
        volatile int posSecondary; 
        volatile int posTurret;

        volatile int targetPosPrimary; 
        volatile int targetPosSecondary; 
        volatile int targetPosTurret;
};



#endif