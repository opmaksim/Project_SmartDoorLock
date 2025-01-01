## 🛠 **작동 흐름**

### 1️⃣ **시스템 초기화**
- **`Set_Lcd`, `Set_Matrix`, `Set_Segment`, `Set_Servo`, `Set_RFRC`**:
  - LCD, 키패드, 7-Segment, 서보 모터, RFID 리더 초기화.
- **초기 상태**:
  - 도어락은 잠금 상태.
  - LCD: "Smart door Lock -Door locked-" 메시지 표시.

---

### 2️⃣ **RFID 태그 등록**
- **키패드의 `*` 버튼 2회 클릭**:
  - `Set_RFRC_Pwd` 함수가 `regi_cnt`를 1로 설정.
- **RFID 태그 등록**:
  - **LCD 메시지**: "TAG Register STEP.1:tagging"
  - 새로운 RFID 태그를 스캔하면 EEPROM에 저장.
  - **등록 성공 시**:
    - **LCD 메시지**: "TAG Register STEP.2:SAVE OK"
    - 부저: 성공 소리 (`Success_Regi` 호출).

---

### 3️⃣ **도어락 잠금 해제**
- **RFID 태그 인증**:
  - `Open_Door` 함수가 등록된 RFID 태그와 비교.
  - **성공 시**:
    - **LCD 메시지**: "Smart door Lock -Door unlocked-"
    - 부저: 잠금 해제 소리 (`Sound_Unlock` 호출).
    - 서보 모터: 잠금 해제 (`Unlock_Servo` 호출).
    - 3초 후 도어락이 자동으로 다시 잠김.

---

### 4️⃣ **비밀번호 인증**
- **키패드의 `#` 버튼 1회 클릭**:
  - `Set_RFRC_Pwd` 함수가 `regi_cnt`를 5로 설정.
- **비밀번호 입력**:
  - 4자리 비밀번호를 입력 후 EEPROM에 저장된 비밀번호와 비교.
  - **성공 시**:
    - **LCD 메시지**: "Smart door Lock -Door unlocked-"
    - 부저: 잠금 해제 소리 (`Sound_Unlock` 호출).
    - 서보 모터: 잠금 해제 (`Unlock_Servo` 호출).
  - **실패 시**:
    - 실패 횟수 증가 (`fail_cnt`).
    - **LCD 메시지**: "Wrong Password".
    - 부저: 실패 소리 (`Sound_Fail` 호출).

---

### 5️⃣ **비밀번호 변경**
- **키패드의 `#` 버튼 2회 클릭**:
  - `Set_RFRC_Pwd` 함수가 `regi_cnt`를 3으로 설정.
- **기존 비밀번호 확인**:
  - 입력한 비밀번호가 EEPROM에 저장된 비밀번호와 일치하는지 확인.
  - **일치 시**:
    - 새로운 비밀번호를 입력받아 EEPROM에 저장.
    - **LCD 메시지**: "Success Change!"
    - 부저: 성공 소리 (`Success_Regi` 호출).
  - **불일치 시**:
    - 실패 횟수 증가 (`fail_cnt`).
    - **LCD 메시지**: "Not match".

---

### 6️⃣ **보호 모드: 5회 인증 실패**
- 실패 횟수 (`fail_cnt`)가 5회에 도달하면:
  - **LCD 메시지**: "5 Failed Unlock"
  - 10초 동안 도어락 잠금 유지 (`Delay_10sec` 호출).
  - LED와 부저로 경고 신호 출력.

---

## 📋 **하드웨어 및 소프트웨어 구성**

### 1. **LCD 디스플레이**
- 상태 및 피드백을 시각적으로 제공.
- 예: "Door locked", "Unauthorized tag".

### 2. **7-Segment 디스플레이**
- 현재 인증 실패 횟수를 표시.

### 3. **서보 모터**
- 도어락 잠금/해제 동작을 제어.

### 4. **부저**
- 성공 및 실패 여부를 청각적으로 알림.

### 5. **EEPROM**
- RFID 태그와 비밀번호 데이터를 영구적으로 저장.

---

## 📂 **디렉토리 구조**

```plaintext
📁 프로젝트 디렉토리
├── smart-doorlock.ino      # 메인 코드
├── 7Segment.h              # 7-Segment 디스플레이 제어
├── Control_Pwd.h           # RFID 및 비밀번호 관리
├── KeyMatrix.h             # 키패드 입력 관리
├── LCD.h                   # LCD 디스플레이 제어
├── Motor.h                 # 서보 모터 제어
├── Sound.h                 # 부저 제어
```
