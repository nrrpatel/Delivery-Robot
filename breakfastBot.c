/*
GROUP 48
Srijay Mundlur, Shangheethan Prabaharan, Nikunj Patel, Om Patel

~~ Breakfast Bot ~~
*/

void calibrateSensors();
void driveDist (int pow,float dist);
void turnRobot(int angle, int pwr);
void shutdown();
void totalCost(float time);
void foodscan();
void getFood(float & encoderCount);
void grabObject();
int buttonControls();

const float RATE= 0.10;
const int MAX_RANGE = 100;
const int STOP_DIST=20;

task main()
{
	/*
	//CLAW RESET FOR TESTING ONLY
	motor[motorB]= 20;
	while (nMotorEncoder[motorB] < 360*4.5)
	{}
	motor[motorB]=0;
	*/

	calibrateSensors();

	//resetting timer
	time1[T1]=0;

	float encoderCount = 0;

	//getting input for number of cups to pick up
	displayString(5,"how many trips?");
	int trips = buttonControls();

	for (int tripnum = 1; tripnum <= trips; tripnum++)
	{
		//resetting distance variable
		encoderCount = 0;

		//pivoting until cup is detected
		foodscan();

		//turning an additional 10 degrees
		turnRobot(-10,5);

		//go to the cup
		nMotorEncoder[motorA]=0;
		getFood(encoderCount);

		//grab the cup
		grabObject();

		//spin 180 and move the same distance back
		turnRobot(-180,10);

		nMotorEncoder[motorA]=0;
		motor[motorA]= motor[motorD]=15;
		while (nMotorEncoder[motorA]<= encoderCount)
		{}
		motor[motorA]= motor[motorD]=0;

		//open the claw
		nMotorEncoder[motorB]=0;
		motor[motorB]= 20;
		while (nMotorEncoder[motorB] < 360*4.5)
		{}
		motor[motorB]=0;

		//wait 1 second before repeating (if needed)
		wait1Msec(1000);

	}
	//calculating cost based on timer and displaying for 10s
	totalCost(time1[T1]);
	wait1Msec(10000);
}

void calibrateSensors()
{
    SensorType[S1] = sensorEV3_Ultrasonic;
    wait1Msec(50);
    SensorType[S4] = sensorEV3_Gyro;
    wait1Msec(50);
    SensorMode[S4] = modeEV3Gyro_Calibration;
    wait1Msec(50);
    SensorMode[S4] = modeEV3Gyro_RateAndAngle;
    wait1Msec(50);

}

void driveDist (int pow,float dist)
{
	nMotorEncoder[motorA]=0;
	motor[motorA]=motor[motorD] = pow;
	while (nMotorEncoder[motorA] < dist*180/(PI*2.75))
	{}
	motor[motorA]=motor[motorD] = 0;
}

void turnRobot(int angle, int pwr)
{
	resetGyro(S4);
	if (angle < 0)
	{
		motor[motorA] = -pwr;
		motor[motorD] = pwr;
	}
	else
	{
		motor[motorA] = pwr;
		motor[motorD] = -pwr;
	}
	while(getGyroDegrees(S4) < abs(angle))
	{}
 	motor[motorA]=motor[motorD] = 0;
}

void shutdown()
{
	motor[motorA]=motor[motorD]=motor[motorB]=0;
  displayString(5, "Shutdown in progress");
  wait1Msec(10000);
}

void totalCost(float time)
{
  float cost= time*RATE;
  displayString(5,"cost: %f dollars",cost/100);
}

void foodscan()
{
	//slowly pivoting
	motor[motorA] = -3;
	motor[motorD] = 3;

	while(SensorValue[S1] >= MAX_RANGE)
	{}
	wait1Msec(200);

	motor[motorA] = motor[motorD] = 0;
}

void getFood(float & encoderCount)
{
  nMotorEncoder[motorA]=0;
	motor[motorA] = motor[motorD] = 25;

  while(SensorValue[S1] >= STOP_DIST)
    {}
  motor[motorA] = motor[motorD] = 0;
  encoderCount = nMotorEncoder[motorA];
}

void grabObject()
{
	driveDist(15,STOP_DIST-3);
	wait1Msec(500);
	//motor should be open when function is called
	nMotorEncoder[motorB]=0;
	//positive opens claw, negative closes it
	motor[motorB]= -20;
	while (nMotorEncoder[motorB] > -360*4.5)
	{}
	motor[motorB]=0;
	//close claw
}

int buttonControls()
{
	int input=0;
	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonUp) || getButtonPress(buttonDown)||
     getButtonPress(buttonLeft)||getButtonPress(buttonRight)||
     getButtonPress(buttonEnter))
	{
		if(getButtonPress(buttonUp))
		{
			input=1;
		}

		if(getButtonPress(buttonDown))
		{
			input=3;
		}

		if(getButtonPress(buttonLeft))
		{
			input=4;
		}
		if(getButtonPress(buttonRight))
		{
			input=2;
		}
		if(getButtonPress(buttonEnter))
		{
			shutdown();
		}
	}
	return input;
}
