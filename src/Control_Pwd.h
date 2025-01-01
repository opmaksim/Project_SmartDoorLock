#include "Sound.h"
#include "KeyMatrix.h"
#include "LCD.h"
#include "Motor.h"

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfidReader(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte readId[4];
byte regiId[4];

void Open_Door();
void Sel_Regi();
void Save_EEPROM();

void Set_RFRC(){
  SPI.begin();
  rfidReader.PCD_Init();
}

int x = 1;
int y = 2;
void Open_Door(){                                                             //RFID 인식 후 등록된 태그 몇 잠금해제 아니면 오류 출력 함수
  Show_Segment(fail_cnt);                                                     //7세그먼트에 실패 횟수를 띄움(초깃값 0)
  if(!rfidReader.PICC_IsNewCardPresent()) return;
  if(!rfidReader.PICC_ReadCardSerial()) return;
  for(int i = 0; i < 4; i++){                                                 //EEPROM에 저장된 RAID Tag값을 regiId배열에 저장
    byte hiByte = EEPROM.read(x);
    byte loByte = EEPROM.read(y);
    regiId[i] = word(hiByte, loByte);
    x+=2;
    y+=2;
  }
  if(rfidReader.uid.uidByte[0] == regiId[0]                                  //인식된 Tag 값과 regiId값이 같으면
      && rfidReader.uid.uidByte[1] == regiId[1]
      && rfidReader.uid.uidByte[2] == regiId[2]
      && rfidReader.uid.uidByte[3] == regiId[3]){
    Show_Open_Door();                                                       //열린다는 문구 Lcd에 출력
    digitalWrite(A1, HIGH);                                                 //LED ON
    Sound_Unlock();                                                         //열린다는 소리 부저 울림
    Unlock_Servo();                                                         //서보 모터 열림
    fail_cnt = 0;                                                           //실패 횟수 초기화
    delay(3000);                                                            //3초동안 유지
    Sound_Lock();                                                           //닫힌다는 소리 부저 울림
    Lock_Servo();                                                           //서보 모터 닫힘
    digitalWrite(A1, LOW);                                                  //LED OFF
  }
  else{
    Show_Unau_Door();                                                       //태그가 서로 다르다면 오류 문구 Lcd 출력
    digitalWrite(A1, HIGH);                                                 //LED ON
    Sound_Fail();                                                           //실패 소리 부저 울림
    digitalWrite(A1, LOW);                                                  //LED OFF
    fail_cnt++;                                                             //실패횟수 1 증가
    Show_Segment(fail_cnt);                                                 //7-세그먼트에 실패횟수 출력
    if(fail_cnt == 5){                                                      //실패횟수가 5회가 된다면
      regi_cnt = 4;                                                         //regi_cnt = 4 로 변경 -> Delay_10sec()함수 (10초동안 잠금)
      fail_cnt = 0;                                                         //실패횟수 초기화
    }
  }
  x=1;
  y=2;
}

void Save_EEPROM(){
  if(!rfidReader.PICC_IsNewCardPresent()) return;
  if(!rfidReader.PICC_ReadCardSerial()) return;

  MFRC522::PICC_Type picc_type = rfidReader.PICC_GetType(rfidReader.uid.sak);

  if(picc_type != MFRC522::PICC_TYPE_MIFARE_MINI
              && picc_type != MFRC522::PICC_TYPE_MIFARE_1K
              && picc_type != MFRC522::PICC_TYPE_MIFARE_4K
              && picc_type != MFRC522::PICC_TYPE_ISO_14443_4){
    return;
  }
  if(rfidReader.uid.uidByte[0] != readId[0]                                               //RFRC가 인식이 된다면
      || rfidReader.uid.uidByte[1] != readId[1]
      || rfidReader.uid.uidByte[2] != readId[2]
      || rfidReader.uid.uidByte[3] != readId[3]){
    Tag_RFRC();
    for(int i = 0;i < rfidReader.uid.size; i++){                                          //EEPROM에 해당 RFID Tag값 저장
      byte hiByte = highByte(rfidReader.uid.uidByte[i]);
      byte loByte = lowByte(rfidReader.uid.uidByte[i]);
      EEPROM.write(x, hiByte);
      EEPROM.write(y, loByte);
      x+=2;
      y+=2;
    }
  }
  rfidReader.PICC_HaltA();
  rfidReader.PCD_StopCrypto1();
  regi_cnt = 2;                                                                          //저장이 완료되면 regi_cnt = 2 -> 성공알림부저 및 Lcd 출력하는 곳으로 이동하게끔 함
  push_cnt = 0;                                                                          //해당 함수로 들어오려면 *클릭횟수(push_cnt)가 2가 되야하므로 등록이 완료되면 초기화
  push_cnt2 = 0;
  fail_cnt = 0;
  x = 1;                                                                                 //EEPROM 주소 초기화
  y = 2;
}

int pwd[4];                                                                              
int check_pwd[4];
int match_pwd[4];
int new_pwd[4];
int status_cnt = 0;
int pwd_cnt = 0;
int lcd_cnt = 11;
int new_cnt = 0;
int a = 9;
int b = 10;

void Change_pwd(){                                                                       //비밀번호 변경 함수
  switch(status_cnt){                                                                    //status_cnt에 따라 달라짐
  case 0:                                                                                //기본값 0
    Lcd.clear();                                                                         //Lcd 출력하자마자
    Lcd.setCursor(0,0);                                                                  //status_cnt = 1, case 1로 넘어감
    Lcd.print("Change Password");
    Lcd.setCursor(0,1);
    Lcd.print("Enter PWD: ");
    status_cnt = 1;
    break;
  case 1:
    Lcd.setCursor(lcd_cnt,1);                                                           //비밀번호 입력하는 위치
    for(int j = 0; j < numCols; j++){
      digitalWrite(pinCols[j], LOW);
      for(int i = 0; i < numRows; i++){
        if(digitalRead(pinRows[i]) == LOW){
          check_pwd[pwd_cnt] = numpad[i][j].toInt();                                   //numpad는 String배열이므로 toInt()로 int형으로 전환
          Lcd.print(check_pwd[pwd_cnt]);                                               //입력된 숫자를 check_pwd배열에 넣은 후 Lcd에 출력
          lcd_cnt++;                                                                   //lcd 커서 위치 1 증가
          pwd_cnt++;                                                                   //배열 위치 1 증가
        }
      }
      if(pwd_cnt == 4){                                                                //배열 위치가 4가 된다면
        pwd_cnt = 0;                                                                   //0으로 초기화
        status_cnt = 2;                                                                //status_cnt = 2 로 변경, case 2로 이동하게끔 함
      }
      digitalWrite(pinCols[j], HIGH);
    }
    delay(250);
    break;
  case 2:
    for(int i = 0; i < 4; i++){                                                        //EEPROM에 저장되어 있는 비밀번호 값을
      byte hibyte = EEPROM.read(a);                                                    //match_pwd배열에 저장
      byte lobyte = EEPROM.read(b);
      match_pwd[i] = word(hibyte, lobyte);
      a+=2;
      b+=2;
    }
    a = 9;
    b = 10;
    if(match_pwd[0] != check_pwd[0]                                                   //입력된 비밀번호와 EEPROM에 저장된 값과 다르면
      || match_pwd[1] != check_pwd[1]
      || match_pwd[2] != check_pwd[2]
      || match_pwd[3] != check_pwd[3]){
      Lcd.clear();
      Lcd.setCursor(0,0);
      Lcd.print("Not match");                                                        //매치가 안된다는 문구 lcd 출력
      digitalWrite(A1, HIGH);                                                        //LED ON
      Sound_Fail();                                                                  //실패부저울림
      digitalWrite(A1, LOW);                                                         //LED OFF
      fail_cnt++;                                                                    //실패횟수 1 증가
      regi_cnt = 0;                                                                  //초기 화면으로 가기 위해 regi_cnt 초기화
      status_cnt = 0;                                                                //status_cnt도 초기화
      pwd_cnt = 0;                                                                   //입력된 비밀번호 배열의 위치도 초기화
      lcd_cnt = 11;                                                                  //lcd 커서 설정
      push_cnt2 = 0;                                                                 //비밀번호를 변경하려면 #클릭횟수(push_cnt2)가 2가 되야했으므로, push_cnt2 초기화
      x = 1;
      y = 2;
      if(fail_cnt == 5){                                                             //실패횟수가 5회가 되면
        regi_cnt = 4;                                                                //regi_cnt = 4 로 해당하는 case문으로 이동하게끔 함
        fail_cnt = 0;                                                                //실패횟수 초기화
        push_cnt2 = 0;                                                               //눌림횟수 초기화
        x = 1;
        y = 2;
      }
    }
    else if(match_pwd[0] == check_pwd[0]                                             //입력한 값과 저장된 값이 같으면
      && match_pwd[1] == check_pwd[1]
      && match_pwd[2] == check_pwd[2]
      && match_pwd[3] == check_pwd[3]){
      lcd_cnt = 9;                                                                   //lcd 커서 설정
      status_cnt = 3;                                                                //case 3으로 이동하게끔 함
    }
    break;
  case 3:
    Lcd.clear();
    Lcd.setCursor(0,0);
    Lcd.print("Change Password");   
    Lcd.setCursor(0,1);
    Lcd.print("New PWD: ");
    Lcd.setCursor(lcd_cnt,1);
    status_cnt = 4;                                                                 //Lcd 출력하자마자 case 4문으로 이동하게끔 함
    break;
  case 4:
    for(int j = 0; j < numCols; j++){
      digitalWrite(pinCols[j], LOW);
      for(int i = 0; i < numRows; i++){
        if(digitalRead(pinRows[i]) == LOW){
          new_pwd[new_cnt] = numpad[i][j].toInt();                                 //입력받은 값은 new_pwd배열에 저장
          Lcd.print(new_pwd[new_cnt]);
          byte hibyte = highByte(new_pwd[new_cnt]);                                //new_pwd에 있는 값을 EEPROM에 저장
          byte lobyte = lowByte(new_pwd[new_cnt]);
          EEPROM.write(a, hibyte);
          EEPROM.write(b, lobyte);
          new_cnt++;                                                               //new_pwd 배열 위치 1 증가
          lcd_cnt++;                                                               //lcd 커서 위치 1 증가
          a+=2;                                                                    //EEPROM 주소 증가
          b+=2;
        }
      }
      if(new_cnt == 4){
        a = 9;
        b = 10;                                                            //배열 끝에 도달하면
        new_cnt = 0;                                                               //초기화
        status_cnt = 5;                                                            //case 5문으로 이동
      }
      digitalWrite(pinCols[j], HIGH);
    }
    delay(250);
    break;
  case 5:
    Lcd.clear();
    Lcd.setCursor(0,0);
    Lcd.print("Success Change!");                                                  //성공 문구 Lcd 출력
    Success_Regi();                                                                //성공부저울림
    lcd_cnt = 11;                                                                  //각종 변수들 초깃값으로 초기화
    regi_cnt = 0;
    status_cnt = 0;
    new_cnt = 0;
    push_cnt2 = 0;
    fail_cnt = 0;
    x = 1;
    y = 2;
    a = 9;
    b = 10;
  }
}

int enter_pwd[4];
int save_pwd[4];
int enter_cnt = 0;

void Enter_pwd(){                                                                  //비밀번호로 잠금해제하는 함수
  for(int i = 0; i < 4; i++){                                                      //EEPROM에 저장된 비밀번호 값을 불러와 save_pwd배열에 저장
    byte hibyte = EEPROM.read(x);
    byte lobyte = EEPROM.read(y);
    save_pwd[i] = word(hibyte, lobyte);
    x+=2;
    y+=2;
  }
  x = 9;                                                                          //EEPROM 주소 초기화
  y = 10;
  Lcd.setCursor(0, 1);
  Lcd.print("Enter PWD: ");
  for(int j = 0; j < numCols; j++){
    digitalWrite(pinCols[j], LOW);
    for(int i = 0; i < numRows; i++){
      if(digitalRead(pinRows[i]) == LOW){
        if(numpad[i][j] == "#"){                                                 //만약 #이 한번 더 눌린다면 push_cnt2 1 증가 -> push_cnt2가 2가 되므로
          push_cnt2++;                                                           //비밀번호 변경 함수로 넘어감
          regi_cnt = 3;                                                          //loop문 case 3으로 넘어가게끔 함
        }
        else{
          enter_pwd[enter_cnt] = numpad[i][j].toInt();                           //#이 아닌 다른 버튼이 눌렸다면 enter_pwd에 입력받은 값 저장
          Lcd.setCursor(enter_cnt + 11, 1);
          Lcd.print(enter_pwd[enter_cnt]);
          enter_cnt++;
        }
      }
    }
    digitalWrite(pinCols[j], HIGH);
    if(enter_cnt == 4){                                                         //입력이 끝나면
      enter_cnt = 0;
      if(enter_pwd[0] != save_pwd[0]                                            //만약 입력한 값과 저장된 값이 다르다면
          || enter_pwd[1] != save_pwd[1]
          || enter_pwd[2] != save_pwd[2]
          || enter_pwd[3] != save_pwd[3]){
        Lcd.clear();
        Lcd.setCursor(0, 0);
        Lcd.print("Smart door Lock");                                           //오류 문구 Lcd 출력
        Lcd.setCursor(0, 1);
        Lcd.print("Wrong Password");
        digitalWrite(A1, HIGH);                                                 //LED ON
        Sound_Fail();                                                           //실패부저울림
        digitalWrite(A1, LOW);                                                  //LED OFF
        fail_cnt++;                                                             //실패횟수 1 증가
        regi_cnt = 0;                                                           //초기화면으로 넘어가기 위해 초기화
        status_cnt = 0;
        push_cnt2 = 0;                                                          //#클릭횟수 변수 초기화
        x = 1;
        y = 2;
        if(fail_cnt == 5){                                                      //실패횟수가 5회가 된다면
          regi_cnt = 4;                                                         //loop문 case 4로 넘어가게끔 함
          fail_cnt = 0;                                                         //실패횟수 초기화
          push_cnt2 = 0;                                                        //#클릭횟수 변수 초기화
          x = 1;
          y = 2;
        }
      }
      else if(enter_pwd[0] == save_pwd[0]                                       //만약 입력값과 저장된 값이 같으면                 
          && enter_pwd[1] == save_pwd[1]
          && enter_pwd[2] == save_pwd[2]
          && enter_pwd[3] == save_pwd[3]){
        Show_Open_Door();                                                       //열렸다는 문구 Lcd 출력
        digitalWrite(A1, HIGH);                                                 //LED ON
        Sound_Unlock();                                                         //열림부저울림
        Unlock_Servo();                                                         //서보모터 열림
        fail_cnt = 0;                                                           //실패횟수 초기화
        delay(3000);                                                            //3초 유지
        Sound_Lock();                                                           //닫힘부저울림
        Lock_Servo();                                                           //서보모터 닫힘
        digitalWrite(A1, LOW);                                                  //LED OFF
        push_cnt2 = 0;                                                          //#클릭횟수 변수 초기화
        regi_cnt = 0;                                                           //초기화면으로 넘어가기 위해 초기화
        x = 1;
        y = 2;
      }
    }
  }
  delay(250);
}