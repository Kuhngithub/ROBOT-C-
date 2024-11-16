#pragma config(StandardModel, "RVW SWERVEBOT")
#define threshold 512
#define maxSpeed 90
#define minSpeed 30
#define minDistanceToWall 80
void driveForward(int velocity) {
    motor[rightMotor] = velocity;
    motor[leftMotor] = velocity;
}

void ceaseMotors() {
    motor[rightMotor] = 0;
    motor[leftMotor] = 0;
}

void slowStop() {
    int wallDistance = SensorValue[sonarSensor];
    while (wallDistance > minDistanceToWall) {
        int velocity = wallDistance - 2;  // Gradually decrease speed as approaching wall
        if (velocity > maxSpeed) velocity = maxSpeed;  // Limit the speed to maximum
        if (velocity < minSpeed) velocity = minSpeed;  // Maintain minimum speed
        driveForward(velocity);
        wallDistance = SensorValue[sonarSensor];
    }
    ceaseMotors();
}

task main() {
    // Phase 1: Advance to detect the line
    driveForward(maxSpeed);
    wait1Msec(2000);  // Modify this duration to locate the line

    // Phase 2: Line tracking
    while (SensorValue[sonarSensor] > minDistanceToWall || SensorValue[sonarSensor] == -1) {
        // RIGHT sensor detects dark surface
        if (SensorValue[rightLineFollower] > threshold) {
            // Adjust to the right
            motor[leftMotor] = maxSpeed;
            motor[rightMotor] = 0;
        }
        // CENTER sensor detects dark surface
        else if (SensorValue[centerLineFollower] > threshold) {
            // Move straight
            motor[leftMotor] = maxSpeed;
            motor[rightMotor] = maxSpeed;
        }
        // LEFT sensor detects dark surface
        else if (SensorValue[leftLineFollower] > threshold) {
            // Adjust to the left
            motor[leftMotor] = 0;
            motor[rightMotor] = maxSpeed;
        }
        else if (SensorValue[sonarSensor] < minDistanceToWall) {
            slowStop();
        }
    }
}
