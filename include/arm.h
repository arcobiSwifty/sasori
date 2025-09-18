#ifndef ARM_H
#define ARM_H


class Arm {
    public:
        void setup();
        void pause(); //activate sleep pin in all drivers;
        void unpause(); //deactivate sleep pin in all drivers;
        void update();

        void goTo(const float z, const float phi, const float r);
        void relativeMove(const float z, const float phi, const float r);
        bool hasArrived();

        void grip();
        void release();


    private:

        volatile int posPrimary;
        volatile int posSecondary; 
        volatile int posTurret;

        volatile int targetPosPrimary; 
        volatile int targetPosSecondary; 
        volatile int targetPosTurret;


        volatile float targetRho;
        volatile float targetPhi;
        volatile float targetR;

        float currentArmSpeed;
        unsigned long lastPoll;

};

extern Arm arm;

#endif