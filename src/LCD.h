#include <LiquidCrystal_I2C.h>
#include "7Segment.h"
LiquidCrystal_I2C Lcd(0x27, 16, 2);

char buffer[3];

void Set_Lcd(){
  Lcd.init();
  Lcd.backlight();
}

void Show_Lock_Door(){                                          //초기 화면 출력
  Lcd.clear();
  Lcd.setCursor(0, 0);
  Lcd.print("Smart door Lock");
  Lcd.setCursor(0, 1);
  Lcd.print("-Door locked-");
}

void Show_Open_Door(){                                         //열렸을때 화면 출력
  Lcd.clear();
  Lcd.setCursor(0, 0);
  Lcd.print("Smart door Lock");
  Lcd.setCursor(0, 1);
  Lcd.print("-Door unlocked-");
}

void Show_Unau_Door(){                                        //태그값이 다를때 화면 출력
  Lcd.clear();
  Lcd.setCursor(0, 0);
  Lcd.print("Smart door Lock");
  Lcd.setCursor(0, 1);
  Lcd.print("Unauthorized tag");  
}

void Show_Enter_pwd(){                                        //비밀번호 값 입력할때 화면 출력
  Lcd.clear();
  Lcd.setCursor(0, 0);
  Lcd.print("Smart door Lock");
}

void Sign_up(){                                               //태그 인식 대기할때 화면 출력
  Lcd.clear();
  Lcd.setCursor(0, 0);
  Lcd.print("TAG Register");
  Lcd.setCursor(0, 1);
  Lcd.print("STEP.1:tagging");
  delay(250);
}

void Success_sign(){                                         //인식 성공 후 저장했을때 화면 출력
  Lcd.clear();
  Lcd.setCursor(0, 0);
  Lcd.print("TAG Register");
  Lcd.setCursor(0, 1);
  Lcd.print("STEP.2:SAVE OK");
}

void Delay_10sec(){                                          //10초 잠금할때 화면 출력
  Lcd.clear();
  Lcd.setCursor(0, 0);
  Lcd.print("5 Failed Unlock");
  for(int i = 10; i > 0; i--){
    Lcd.setCursor(0, 1);
    sprintf(buffer, "%02d", i);
    Lcd.print(buffer);
    Lcd.setCursor(2, 1);
    Lcd.print(" second Left");
    Show_Segment(i);
    digitalWrite(A1, HIGH);
    delay(500);
    digitalWrite(A1, LOW);
    delay(500);
  }
}