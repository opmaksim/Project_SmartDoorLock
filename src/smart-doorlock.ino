#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include "Control_Pwd.h"

void setup() {
    Set_Matrix();                          //매트릭스 set함수
    Set_RFRC();                            //RFRC set함수
    Set_Lcd();                             //I2C_Lcd set함수
    Set_piezo();                           //piezo set함수
    Set_Servo();                           //Servo set함수
    Set_Segment();                         //7-Segment set함수
    pinMode(A1, OUTPUT);                   //A1 연결된 LED pinMode 설정
}
void loop() {
    switch (regi_cnt) {                      //regi_cnt 변수의 값에 따라 실행되는 함수가 달라짐
    case 0:
        Show_Lock_Door();                    //Lcd 화면 출력
        Open_Door();                         //RFRC 인식이 되면 등록된 태그인지 검사 후 결과 출력
        Set_RFRC_Pwd();                      //* 또는 #버튼 인식 및 눌림 횟수 저장
        break;
    case 1:
        Sign_up();                           //Lcd 화면 출력
        Save_EEPROM();                       //태그된 RFID EEPROM에 저장 후 regi_cnt = 2로 변경
        break;
    case 2:
        Success_sign();                      //Lcd 화면 출력
        Success_Regi();                      //성공부저울림
        regi_cnt = 0;                        //regi_cnt 초기화하여 처음 화면으로 되돌아감
        break;
    case 3:
        Change_pwd();                        //#버튼이 2번 눌리면 regi_cnt가 3으로 변경됨, 해당 함수 실행
        break;
    case 4:
        Delay_10sec();                       //비밀번호 & RFID Tag 5회 틀리면 10초동안 잠금화면 출력하는 함수
        regi_cnt = 0;
        break;
    case 5:
        Show_Enter_pwd();                    //Lcd 화면 출력
        regi_cnt = 6;                        //출력하자마자 regi_cnt 6으로 변경
        break;
    case 6:
        Enter_pwd();                         //비밀번호 입력 값이 맞으면 잠금해제, 아니면 오류 뜨게 하는 함수
        break;
    }
}