#pragma config(StandardModel, "RVW SWERVEBOT")

#define move_speed 200
#define tile_dist_cm 900
#define turn_angle 185
#define wall_dist 20
#define delay_time 500
#define start_delay 2000
#define rotate_speed 65

// Function to create a delay
void delay(){
	wait1Msec(delay_time);
}

// Function to move the robot forward by one space
void advance(){
	// Reset encoder values
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;

	// Move forward until a wall is detected or the tile distance is covered
	while(SensorValue[sonarSensor] > wall_dist && SensorValue[leftEncoder] < tile_dist_cm){
		motor[leftMotor] = move_speed;
		motor[rightMotor] = move_speed;
	}

	// Stop the motors
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
}

// Function to rotate the robot in a specified direction
void rotate(char* direction){
	// Reset encoder values
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;

	// Rotate left
	if(strcmp(direction, "left") == 0){
		while(SensorValue[leftEncoder] > -turn_angle || SensorValue[rightEncoder] < turn_angle){
			motor[rightMotor] = rotate_speed;
			motor[leftMotor] = -rotate_speed;
		}
	}
	// Rotate right
	else if(strcmp(direction, "right") == 0){
		while(SensorValue[leftEncoder] < turn_angle || SensorValue[rightEncoder] > -turn_angle){
			motor[rightMotor] = -rotate_speed;
			motor[leftMotor] = rotate_speed;
		}
	}

	// Stop the motors
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;

	// Add a delay
	delay();
}
// Function to measure distance using the sonar sensor
int get_distance(){
	wait1Msec(100);
	return SensorValue[sonarSensor];
}

// Function to determine the next direction based on measured distances
char* next_direction(){
	int dist_forward, dist_left, dist_right;

	// Measure forward distance
	dist_forward = get_distance();

	// Measure left distance
	rotate("left");
	dist_left = get_distance();
	rotate("right"); // Back to original position

	// Measure right distance
	rotate("right");
	dist_right = get_distance();
	rotate("left"); // Back to original position

	// Determine the direction with the greatest distance
	if(dist_forward >= dist_left && dist_forward >= dist_right){
		return "forward";
		}else if (dist_left > dist_forward && dist_left > dist_right){
		return "left";
		}else{
		return "right";
	}
}
task main(){
	// Reset encoder values
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;


	// Main loop
	while(true){
		advance();
		char* direction = next_direction();
		if(strcmp(direction, "forward") == 0){
			advance();
			}else{
			rotate(direction);
			advance();
		}
	}
}
