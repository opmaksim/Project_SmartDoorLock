# 🌟 **스마트 도어락** (Smart Door Lock)  
**스마트 도어락 프로젝트**는 RFID 태그와 비밀번호 인증 방식을 활용하여 사용자의 안전과 편리함을 극대화한 IoT 기반 스마트 잠금 시스템입니다.

## 📌 **프로젝트 개요**  
- **📅 수행 기간**: 2023.06.15 ~ 2023.06.19  
- **🔧 사용 기술**:  
  - Arduino IDE  
  - C++  
  - EEPROM 데이터 저장  
  - SPI/I2C 통신  
- **🎯 주요 기능**:  
  - **RFID 태그와 비밀번호 인증**  
  - **5회 실패 시 10초간 잠금 유지**  
  - **7-Segment 디스플레이를 통한 시각적 피드백**  
  - **TFT LCD와 LED/부저를 활용한 사용자 피드백**



## 📚 **기술 스택**

| 기술           | 설명                                           |
|----------------|----------------------------------------------|
| **Arduino IDE** | 도어락 시스템 소프트웨어 작성 및 업로드           |
| **C++**         | 하드웨어 제어 및 기능 구현                     |
| **EEPROM**      | 데이터 영구 저장 (RFID 및 비밀번호)             |
| **SPI/I2C**     | 하드웨어 간의 데이터 전송                      |

## 🛠 **기능 설명**  

### 🎟 **RFID 태그 및 비밀번호 인증**
- RFID 태그 또는 비밀번호로 도어락 잠금 해제  
- 비밀번호 또는 태그 입력 실패 시 오류 횟수 기록  

### 🔒 **보호 기능**
- 인증 실패 5회 이상 시, 10초 동안 시스템 잠금  

### 📊 **실시간 피드백 제공**
- **TFT LCD**: 시스템 상태 표시 (잠금/해제/인증 실패 등)  
- **7-Segment 디스플레이**: 오류 횟수 표시  
- **LED와 부저**: 성공/실패 알림  

### 🗂 **데이터 보존**
- EEPROM을 활용해 전원이 꺼져도 RFID 태그와 비밀번호 데이터 유지  

### ⚙️ **하드웨어 연동**
- **Key Matrix**: 비밀번호 입력 및 변경  
- **74HC595**: 디지털 핀 절약을 위한 시프트 레지스터  
- **서보 모터**: 물리적 잠금 및 해제  


## 🖼 **시스템 구성도**  

![alt text](<img/구성1.jpg>)
> 시스템 구성도: Arduino Uno와 RFID Reader, Key Matrix, TFT LCD, 서보 모터, 부저, LED의 조합으로 구현.  


## 📋 **플로우 차트** 

![alt text](<img/아두이노 플로차트.png>)

```plaintext
1. 시스템 시작 → 잠금 여부 표시
2. RFID 태그 인증 → 등록된 카드라면 잠금 해제
3. 비밀번호 입력 → 등록된 비밀번호라면 잠금 해제
4. 오류 횟수 증가 → 5회 초과 시 10초간 잠금
5. 초기 화면으로 복귀 → 루프 반복
```

## **소스 코드**

### [소스코드 바로가기](/src/)

## 📽 **시연 영상**

###  [EEPROM 작동영상](https://drive.google.com/file/d/1MujaSoHp4UNyhDWvEoenL6BfYaLAoawG/view?usp=sharing)  
![alt text](gif/EEPROM작동영상.gif)

###  [RF태그 인증 성공](https://drive.google.com/file/d/1YYxM1XOTx2GOW8KGLdstxpaAq8Ou49_t/view?usp=sharing)
![alt text](gif/RF태그인증성공.gif)

### [비밀번호 인증 성공](https://drive.google.com/file/d/1anoRTXU_o0v-hY8vrUQUnI5wxC5C_Y1i/view?usp=sharing)
![alt text](gif/비밀번호인증성공.gif)

### [RF태그 5회 오류](https://drive.google.com/file/d/1NXzleVFyKxmz5J1ppQkno4h6jzdR3PGr/view?usp=sharing)
![alt text](gif/RF태그5회오류.gif)

### [비밀번호 5회 오류](https://drive.google.com/file/d/1Rxn2nM4UNtDHe29yYQBI_8duVJtfgDdl/view?usp=sharing)
![alt text](gif/비밀번호5회오류.gif)

### [복합적 인증 5회 오류](https://drive.google.com/file/d/125lMFFEactIWvCZ_-WxsLUZxeB01ogBq/view?usp=drive_link)
![alt text](gif/복합적인증5회오류.gif)

### [오류 중첩 후 인증 성공](https://drive.google.com/file/d/15-v4FAV2ENnxRNOuzUoVExP98IRCbwpY/view?usp=sharing)
![alt text](gif/오류중첩후인증성공.gif)
### [RF태그 변경](https://drive.google.com/file/d/18BHI_SWtR5N701nFjLiItIUpMxn-22A2/view?usp=sharing)
![alt text](gif/RF태그변경.gif)

### [비밀번호 변경](https://drive.google.com/file/d/1eqMVDRwIiuwbm2lJyh8Ob9F2aArfxDsH/view?usp=sharing)
![alt text](gif/비밀번호변경.gif)
