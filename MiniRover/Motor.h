#ifndef _motor_h
#define _motor_h

class Motor {
public:
  Motor(byte fwd_pin, byte bkd_pin);
  void setSpeed(int vel);

private:
  byte fwd, bkd; // pins
  byte cur_speed; // current pwm
};

Motor::Motor(byte fwd_pin, byte bkd_pin)
{
  fwd = fwd_pin;
  bkd = bkd_pin;
  
  pinMode(fwd, OUTPUT);
  pinMode(bkd, OUTPUT);
}

void Motor::setSpeed(int vel)
{  
  if (vel > 0) {
    analogWrite(fwd, vel);
    analogWrite(bkd, 0);
  } else {
    analogWrite(fwd, 0);
    analogWrite(bkd, -vel);
  }
}

#endif
