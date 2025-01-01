#define	dataP	  0
#define	latchP	1
#define	clockP	A2 

byte digit[11] = {                                          //세그먼트 0~.까지 바이트를 나타냄
  B00010001,
  B11010111,
  B00110010,
  B10010010,
  B11010100,
  B10011000,
  B00011000,
  B11010001,
  B00010000,
  B10010000,
  B11101111
};


void Set_Segment(){                                             //74HC595 set 함수
  pinMode(latchP, OUTPUT);
  pinMode(clockP, OUTPUT);
  pinMode(dataP, OUTPUT);
  digitalWrite(latchP, LOW);
}

void Show_Segment(int i){                                     //7segment 출력 함수
  digitalWrite(latchP, LOW);
  shiftOut(dataP, clockP, LSBFIRST, digit[i]);
  digitalWrite(latchP, HIGH);
}
