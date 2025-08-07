// from joystick imputs, calculates each modules desidered movement. communicates with swerve modules and updates central position. 
//sends update to modules about desired speed / angle


class SwerveController {
    float robotAbsX;
    float robotAbsY;

    const float relX; 
    const float relY;

    float angle;
    float targetSpeed;

};