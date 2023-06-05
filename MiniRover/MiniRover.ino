// Code for 3 dof arm...Arduino Mega

#include "Motor.h"
#include "sbus.h"
#define SBUS_PIN A8
SBUS sbus;

int right_wheel_pin1 = 2;
int right_wheel_pin2 = 3;

int left_wheel_pin1 = 4;
int left_wheel_pin2 = 5;

int base_arm_pin1 = 6;
int base_arm_pin2 = 7;

int middle_arm_pin1 = 8;
int middle_arm_pin2 = 9;

int clip_arm_pin1 = 10;
int clip_arm_pin2 = 11;

int rEna = 12;
int lEna = 13;
float pwm = 255;

//These array will be used in filtering
int base[3] = {900, 900, 900};
int middle[3] = {900, 900, 900};
int clip[3] = {900, 900, 900};

//Initializing motor

Motor motor_Right(right_wheel_pin2, right_wheel_pin1);
Motor motor_Left(left_wheel_pin2, left_wheel_pin1);
Motor base_arm(base_arm_pin2, base_arm_pin1);
Motor middle_arm(middle_arm_pin2, middle_arm_pin1);
Motor clip_arm(clip_arm_pin2, clip_arm_pin1);



void setup()
{
    // These are wheel motors
    pinMode(rEna, OUTPUT);
    pinMode(lEna, OUTPUT);

    analogWrite(rEna, pwm);
    analogWrite(lEna, pwm);

    Serial.begin(9600);
    sbus.begin(SBUS_PIN, sbusNonBlocking);
}

// This is the filter function to filter noise
int filterarm(int x, int prev)
{
    if (x < 950 || x >= 1050 && x <= 1500 || x >= 1550 && x <= 1800 || x >= 1968 && x <= 3000)
        return prev;
    else
        return x;
}
// Experimental filter function to filter noise
int filterG(int x, int prev)
{
    if (x < 900 || x >= 1968 && x <= 3000)
        return prev;
    else
        return x;
}

// arm() controls arm
void arm()
{
    for (int i = 2; i >= 0; i--)
    {
    base[i] = filterG(sbus.getChannel(3), base[i]);
    middle[i] = filterG(sbus.getChannel(1), middle[i]);
    clip[i] = filterG(sbus.getChannel(8), clip[i]);
    }


    base[0] = (base[0] + base[1] + base[2]) / 3;
    middle[0] = (middle[0] + middle[1] + middle[2]) / 3;
    clip[0] = (clip[0] + clip[1] + clip[2]) / 3;

    Serial.print(base[0]);
    Serial.print(' ');
    Serial.print(middle[0]);
    Serial.print(' ');
    Serial.println(clip[0]);

    //control the arm pwm using map
    base[0] = map(base[0], 2000, 1000, -150, 150);
    middle[0] = map(middle[0], 2000, 1000, -150, 155);
    clip[0] = map(clip[0], 2000, 1000, -150, 150);
   
    base_arm.setSpeed(base[0]);
    middle_arm.setSpeed(middle[0]);
    clip_arm.setSpeed(clip[0]);
  

}
void loop()
{

    //  for (int i = 1; i <= 10; i ++) {
    //      Serial.print(sbus.getChannel(i));
    //      Serial.print(' ');
    //    }
    int xvel = sbus.getChannel(2) + 7;
    int zvel = sbus.getChannel(4) - 3;

    //  Serial.print(xvel);
    //  Serial.print(' ');
    //  Serial.println(zvel);

    if (xvel < 1000 || zvel < 1000)
    {
        xvel = 1500;
        zvel = 1500;
    }

    if (abs(xvel - 1500) < 100)
    {
        xvel = 1500;
    }

    if (abs(zvel - 1500) < 100)
    {
        zvel = 1500;
    }

    //  Serial.print(xvel);
    //  Serial.print(' ');
    //  Serial.println(zvel);

    xvel = map(xvel, 2000, 1000, -100, 100);
    zvel = map(zvel, 1000, 2000, -100, 100);

    //  Serial.print(xvel);
    //  Serial.print(' ');
    //  Serial.println(zvel);

    motor_Right.setSpeed(5 * (xvel - zvel));
    motor_Left.setSpeed(5 * (xvel + zvel));
    arm();

    delay(10);
}
