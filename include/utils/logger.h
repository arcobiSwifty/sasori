//logger functionality 

//display led (green or red) based on component status. 
//components 

// swerve module x3
//camera x1
//arm x3 (everytime one goes to sleep set to yellow)
//blueetooth connection


enum Status {
    ON,
    OFF,
    IDLE
};

class LedLoggger {

    public:
        void updateCanLed(int can_id, Status status);
        void updateBlueetooth(Status status);
        void updateArmLed(int id, Status status);

    private:
        int swerveLed[3];
        int cameraLed;
        int mainLed;
        int armLed[3];
        int bluetoothLed;
        
};