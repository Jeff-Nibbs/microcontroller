//www.elegoo.com
//2018.12.19
#include <Servo.h>
Servo myservo;


int positionTracker;
int direction = 2;

void setup(){
  myservo.attach(9);
  myservo.write(0);// move servos to center position -> 0°
  pinMode(LED_BUILTIN, OUTPUT);
} 
void loop(){

  if (direction == 0) {
    for (int i = 360; i > 0; i--)
    {
      myservo.write(positionTracker);
      delay(20);
    }
    direction = 1;
  }
  else if (direction == 1) {
    for (int i = 360; i > 0; i--)
    {
      myservo.write(positionTracker);
      delay(20);
    }
    direction = 0;
  }
  else {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second
  }

}
