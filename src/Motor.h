#include <Servo.h>

#define servoPin  3

Servo myServo;

void Set_Servo() {
  myServo.attach(servoPin);
  myServo.write(0);
}

void Unlock_Servo() {                                               //서보모터 열림
  for(int angle = 0; angle < 91; angle++){
    myServo.write(angle);
    delay(15);
  }
}

void Lock_Servo(){                                                 //서보모터 닫힘
  for(int angle = 91; angle > 0;angle--){
    myServo.write(angle);
    delay(15);
  }
}
