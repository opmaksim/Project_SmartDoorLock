const int numRows = 4;
const int numCols = 3;

int pinRows[numRows] = {5, 6, 7, 8};
int pinCols[numCols] = {4, 2, A0};
int Key_result;
int regi_cnt = 0;
int push_cnt = 0;
int push_cnt2 = 0;
int fail_cnt = 0;

String numpad[4][3] = {                                                   //키패드
  {"1", "2", "3"},
  {"4", "5", "6"},
  {"7", "8", "9"},
  {"*", "0", "#"}
};

void Set_Matrix(){                                                        //매트릭스 set함수
  for(int i = 0; i < numRows; i++){
    pinMode(pinRows[i], INPUT_PULLUP);
  }
  for(int j = 0; j < numCols; j++){
    pinMode(pinCols[j], OUTPUT);
    digitalWrite(pinCols[j], HIGH);
  }
}

void Set_RFRC_Pwd(){                                                     //RFRC 등록하기 위해 *클릭 두번, 비밀번호 입력 #클릭 한번, 비밀번호 변경 #클릭 두번
  for(int j = 0; j < numCols; j++){
    digitalWrite(pinCols[j], LOW);

    for(int i = 0; i < numRows; i++){
      if(digitalRead(pinRows[i]) == LOW){
        if(numpad[i][j] == "*") push_cnt++;                              //*이 눌리면 push_cnt 증가
        else if(numpad[i][j] == "#") push_cnt2++;                        //#이 눌리면 push_cnt2 증가
      }
    }
    digitalWrite(pinCols[j], HIGH);
  }
    if(push_cnt == 2){                                                  //*이 2번 입력되면 RFRC등록으로 넘어가기
      regi_cnt = 1;
      push_cnt = 0;
      push_cnt2 = 0;
    }
    if(push_cnt2 == 1){                                                //#이 1번 입력되면 비밀번호 입력으로 넘어가기
      regi_cnt = 5;
      push_cnt = 0;
    }
    if(push_cnt2 == 2){                                                //#이 2번 입력되면 비밀번호 변경으로 넘어가기
      regi_cnt = 3;
      push_cnt = 0;
      push_cnt2 = 0;
    }
  delay(250);
}
