#define piezo A3
int Success_tones[] = {659, 784, 880, 784, 880, 988, 880, 988, 1047, 784, 659, 784, 659, 587, 523};
int Unlock_tones[] = {731, 747, 850};
int Lock_tones[] = {850, 747, 431};
int Fail_tones[] = {800, 500, 800, 500};
void Set_piezo(){
  pinMode(piezo, OUTPUT);
}

void Tag_RFRC(){                              //RFRC 인식되면 소리나게 하는 함수
  tone(piezo, 1000);
  delay(1000);
  noTone(piezo);
}

void Success_Regi(){                         //인식 성공후 태그 저장 시 소리나게 하는 함수
  for(int i = 0; i < 15; i++){
    tone(piezo, Success_tones[i]);
    delay(200);
  }
  noTone(piezo);
}

void Sound_Unlock(){                        //잠금해제 되었을 때 소리나게 하는 함수
  for(int i = 0; i < 3; i++){
    tone(piezo, Unlock_tones[i]);
    delay(200);
  }
  noTone(piezo);
}

void Sound_Lock(){                         //잠겼을 때 소리나게 하는 함수
  for(int i = 0; i < 3; i++){
    tone(piezo, Lock_tones[i]);
    delay(200);
  }
  noTone(piezo);
}

void Sound_Fail(){                        //비밀번호가 다르거나 태그의 값이 다르면 소리나게 하는 함수
  for(int i = 0; i < 4; i++){
    tone(piezo, Fail_tones[i]);
    delay(100);
  }
  noTone(piezo);  
}
