#ifndef B_CONTROLLER
#define B_CONTROLLER

//include controller library

//connectos to dual shock controller. handles input routing 

//drive mode: 
//  left is movement 
//  right is robot orientation

//press x, switch to arm mode 
//  press L2 - R2 for auto pick auto place 
//  left is main /secondary arm movement in the xz plane 
//  right is turrent orientation
//  l1 r2 manual pick and place
//  when exiting drive mode, put arm to sleep in reset position

// arrows: select place - pick up spot (object should be held in place)
// L2 + arrow -> auto pick up from spot 
// R2 + arrow -> auto place in spot
//forward arrow: do janken

//touch pad?

//y initiate automatic driving mode, navigate until checkpoint 

// 2 buttons yet to map. speed mode?

// option: reset swerve module


class BluetoothController {
    public: 
        BluetoothController();

        unsigned long last_poll;

        volatile bool armMode; 
        volatile bool commandSent;

        volatile int LastRJoyStickX; 
        volatile int LastLJoyStickX; 
        volatile int LastRJoyStickY; 
        volatile int LastLJoyStickY; 

        void connect();
        
        static void onConnect();

        static void onDisconnect();

        static void onEvent();

        void handleEvent();

        void setErrorState();
        void setNormalState();
        void setObjectDetectedState();
        
};

extern BluetoothController BC;

#endif