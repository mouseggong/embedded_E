/*
 * Arduino Wi-Fi Shield Source Code (및 Comment)
 * 작성일 : 2016년 11월 25일
 * 기타 : Connetivity 모듈 아님
 */

#include <EEPROM.h>
#include <MsTimer2.h> // 타이머 관련

// Communication Data
#define Node_ID 83
#define Server_ID 187
#define Port_Num 7777

#if !defined(SERIAL_PORT_MONITOR)
  #error "Arduino version not supported. Please update your IDE to the latest version."
#endif

#if defined(SERIAL_PORT_USBVIRTUAL)
  // Shield Jumper on HW (for Leonardo and Due)
  #define port SERIAL_PORT_HARDWARE
  #define pcSerial SERIAL_PORT_USBVIRTUAL
#else
  // Shield Jumper on SW (using pins 12/13 or 8/9 as RX/TX)
  #include "SoftwareSerial.h"
  SoftwareSerial port(12, 13);
  #define pcSerial SERIAL_PORT_MONITOR
#endif

#include "EasyVR.h"

#define HBDEBUG(i)

EasyVR easyvr(port);

bool heartbeatDetected(int IRSensorPin, int delay);

//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
  GROUP_1  = 1,
};

enum Group0 
{
  G0_GO = 0,
};

enum Group1 
{
  G1_HELP = 0,
  G1_RESTROOM = 1,
  G1_THIRSTY = 2,
  G1_HUNGRY = 3,
  G1_LIGHTON = 4,
  G1_LIGHTOFF = 5,
};


int8_t group, idx;


// Sensor pin
int pin_cds = A4, pin_TEMP = A2, pin_HeartBeat = A3;
/*  위 코드로 알 수 있듯이, 본 코드에서 전송하는 센서는
 *  광센서 (CDS),
 *  온도센서 (TEMP),
 *  Open Detector (REED Switch),
 *  Motion Detector (PIR)
 */

// Node ID
uint8_t ID = 0;

// 정확히 모르겠음
uint32_t timer_check = 0;
// Arduino 가 시작한 시간으로 부터 PinMode 와 Serial 포트 초기화 완료 시간까지 측정하기 위한 용도로 추측

uint8_t RX_flag = 0, TX_flag = 0, Timer_flag = 0, Motion_flag = 0;
// RX_flag : 0 = 데이터를 수신할 수 없음 (Can't) : 1 = 데이터를 수신할 수 있음 (Can) : 2 = 오류 (Error)
// TX_flag : 데이터를 송신한 ID 저장 (0x00 = 서버 : 그 외 = 클라이언트)
// Timer_flag : 타이머가 발생되었다면 1, 아니면 0
// Motion_flag : Motion Detect 에 무언가 감지 되었다면 1, 아니면 0

uint8_t EEPROM_buf[2] = {0xAA, 0};
// EEPROM 에는 항상 0xAA 와 ID 값을 저장해야 함

char RX_buf[17];
// Wi-Fi Shield Pakcet Info 에 정의된 패킷의 길이는 항상 17Byte

uint8_t TX_buf[17] = {0xA0, 0x0A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0A, 0xA0};
// Wi-Fi Shield Pakcet Info 에서 Header 는 항상 0xA0, 0x0A 이며, Tail 은 항상 0x0A, 0xA0

int RX_count = 0;
// RX_count 가 낮을 수록 Wi-Fi 모듈을 설정 중이며, 높을 수록 데이터를 보낼 수 있는 상태.

void setup() {
  static int RX_count = 0;
  static char Check_buf[3] = {0, 0, 0};
  
  pinMode(pin_cds,INPUT);
  pinMode(pin_TEMP,INPUT);
  
  Serial.begin(115200);
  Serial1.begin(9600);
//  Serial.begin(9600);
//  Serial.begin(115200);

  
  delay(1000);
  
  timer_check = millis(); // PinMode 와 Serial 초기화를 설정한 시간을 milliseconds 로 저장
  
  Serial.println("Start Wifi Setting");
  
  Serial1.print("AT\r\n");
  delay(100);
  
  Serial1.print("AT+WAUTO=0,anygate\r\n"); 
  delay(10);
  
  Serial1.print("AT+NDHCP=0\r\n");
  delay(10);

  Serial1.print("AT+NAUTO=0,1,192.168.10.");
  Serial1.print(Server_ID);
  Serial1.print(",");
  Serial1.print(Port_Num);
  Serial1.print("\r\n");
  delay(10);
  
  Serial1.print("AT+NSET=192.168.10.");
  Serial1.print(Node_ID);
  Serial1.print(",255.255.255.0,192.168.10.1\r\n");
  delay(10);
  
  Serial1.print("AT&W0\r\n");
  delay(10);
  
  Serial1.print("ATC0\r\n");
  delay(10);
  
  Serial1.print("ATA\r\n");
  delay(10);
  
  Serial.println("Wifi Setting Finish");

  
  // setup PC serial port
  pcSerial.begin(115200);

  // bridge mode?
  int mode = easyvr.bridgeRequested(pcSerial);
  switch (mode)
  {
  case EasyVR::BRIDGE_NONE:
    // setup EasyVR serial port
    port.begin(9600);
    // run normally
    pcSerial.println(F("---"));
    pcSerial.println(F("Bridge not started!"));
    break;
    
  case EasyVR::BRIDGE_NORMAL:
    // setup EasyVR serial port (low speed)
    port.begin(9600);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("---"));
    pcSerial.println(F("Bridge connection aborted!"));
    break;
    
  case EasyVR::BRIDGE_BOOT:
    // setup EasyVR serial port (high speed)
    port.begin(115200);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("---"));
    pcSerial.println(F("Bridge connection aborted!"));
    break;
  }

  while (!easyvr.detect())
  {
    Serial.println("EasyVR not detected!");
    delay(1000);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  Serial.println("EasyVR detected!");
  easyvr.setTimeout(5);
  easyvr.setLanguage(0);

  group = EasyVR::TRIGGER; //<-- start group (customize)
    
  // timer2 period : 200ms, ISR: TIMER_ISR 설정
  MsTimer2::set(200, TIMER_ISR);
  // timer2 start
  MsTimer2::start();

}

void action();

const int delayMsec = 60; // 100msec per sample

void loop() {
  uint16_t i, cds = 0;
  
  if(((timer_check+4000) < millis()) && (RX_flag == 0)) {
    // PinMode 와 Serial 초기화를 설정한 시간에서 4초 (= 4000ms) 이하이면서, RX_flag 가 0일 경우
    // (= 결국에는 이 if 문이 작동될 확률은 약 1번, 많아야 3번)
    if(RX_count < 1) {  // RX_count 가 0 이라면, (= 초기 상태)
      if(EEPROM.read(0) == 0xAA) {
        // EEPROM 의 첫 번째 번지가 0xAA 인 것은 사용자가 EEPROM 을 한 번도 수정한 적 없다는 약속
        ID = EEPROM.read(1);  // EEPROM 으로부터, ID를 읽어옴
        RX_flag = 1;    // 데이터 수신 가능, loop() 함수가 이번만 실행하고 더 이상 반복되지 않음
        
        Serial.print("\n\rID : ");
        Serial.println(ID);
        
        TX_buf[3] = ID; // TX_buf 배열 3에 장치 ID 저장 (Wi-Fi Shield Pakcet Info 참조)
      } else {
        RX_flag = 2;    // 오류 있음, loop() 함수가 이번만 실행하고 반복되지 않음
        
        Serial.println("\n\rWifi Connected Error");
        Serial.print("\n\rPlease reset the ADK-2560"); 
      }
    }
  }

  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  Serial.print("Say a command in Group ");
  Serial.println(group);
  easyvr.recognizeCommand(group);

  do
  {
    // can do some processing while waiting for a spoken command
  }
  while (!easyvr.hasFinished());
  
  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  idx = easyvr.getWord();
  if (idx >= 0)
  {
    // built-in trigger (ROBOT)
    // group = GROUP_X; <-- jump to another group X
    return;
  }
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // print debug message
    uint8_t train = 0;
    char name[32];
    Serial.print("Command: ");
    Serial.print(idx);
    if (easyvr.dumpCommand(group, idx, name, train))
    {
      Serial.print(" = ");
      Serial.println(name);
    }
    else
      Serial.println();
  // beep
    easyvr.playSound(0, EasyVR::VOL_FULL);
    // perform some action
    action();
  }
  else // errors or timeout
  {
    if (easyvr.isTimeout())
      Serial.println("Timed out, try again...");
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
      Serial.print("Error ");
      Serial.println(err, HEX);
    }
  }

  static int beatMsec = 0;
  int heartRateBPM = 0;
  
  if (heartbeatDetected(pin_HeartBeat, delayMsec)) {
    heartRateBPM = 60000 / beatMsec;
    //digitalWrite(ledPin,1);

    // Print msec/beat and instantaneous heart rate in BPM
    Serial.print(beatMsec);
    Serial.print(", ");
    Serial.println(heartRateBPM);
    
    beatMsec = 0;
  } else {
    //digitalWrite(ledPin,0);
  }
  // Note: I assume the sleep delay is way longer than the
  // number of cycles used to run the code hence the error
  // is negligible for math.
  delay(delayMsec);
  beatMsec += delayMsec;
    

  
  if(Timer_flag) { // Timer_flag 가 발생하였고, 내 ID가 서버가 아니라면(서버라면 0x00),
  //if(Timer_flag && TX_flag) { // Timer_flag 가 발생하였고, 내 ID가 서버가 아니라면(서버라면 0x00),
    cds = analogRead(pin_cds);  // 광 센서로부터 조도값을 가져옴
    
    TX_buf[4] = cds>>8;
    TX_buf[5] = cds & 0xFF; // 온도 센서로 가져온 데이터는 uint16_t 형이므로, char 형에 맞춰서 Casting
    TX_buf[6] = TEMP_read();
    TX_buf[7] = heartRateBPM;
    
    TX_buf[14] = TX_buf[2]; // Check Sum 을 계산하기 위해 초기화
    
    for(i=3; i<14; i++) {
      TX_buf[14] += TX_buf[i];
      // Check Sum 데이터로 활용하기 위해, TX_buf 2에서 13까지의 값을 더함 (2는 초기값)
    }
    
    Serial.println("\n\r TX Packet data");
    
    for(i=0; i<17; i++) {
      Serial.write(' ');
      Serial.print(TX_buf[i],HEX);
      Serial1.write(TX_buf[i]);
      // Check Sum 데이터를 포함하여 Serial (Serial 모니터) 와 Serial1 (Wi-Fi 모듈)에 출력
    }

    TX_buf[8] = 0;
    Timer_flag = 0; // Timer_flag 초기화
  }
}

void serialEvent1(void) {
  Serial.print("event on\n");         // Serial1 에서 읽어온 데이터를 Serial 로 보냄
  
  // Serial 에서 읽어올 데이터가 존재할 때 자동으로 실행되는 함수. '1' 이 붙었기 때문에, Serial1 장치와 연관있다.
  static char Check_buf[4] = {0, 0, 0, };
  uint8_t i,check_sum = 0, RX_cnt = 0;
  
  Serial.print("event on2\n");
  if(RX_flag == 0) {  // 데이터를 수신할 수 없을 때 (= setup () 함수 작동 중)
    char da = Serial1.read(); // Serial1 에서 데이터를 읽어 옴
    Serial.write(da);         // Serial1 에서 읽어온 데이터를 Serial 로 보냄
    
    Check_buf[0] = Check_buf[1];
    Check_buf[1] = Check_buf[2];
    Check_buf[2] = da;
    
    /* 
     *  만약 "[OK]" 데이터가 들어온다면,
     *  1 : Check_buf[] = {0, 0, 0, '['}
     *  2 : Check_buf[] = {0, 0, '[', 'O'}
     *  3 : Check_buf[] = {0, '[', 'O', 'K'}
     *  4 : Check_buf[] = {'O', 'K', ']', '\r'}
     *  5 : Check_buf[] = {'K', ']', '\r', '\r'}
     *  6 : Check_buf[] = {']', '\r', '\r', '\r'}
     *  "\n" 은 LF 와 CR 을 동시에 행동하므로, "\r" 이 두 번 발생
     */
    
    if((Check_buf[0] == 'A') && (Check_buf[1] == 'T') && (Check_buf[2] == 'A') && (RX_count == 0)) {
      // Serial1 에서 읽어온 데이터가 'ATA' 이며, RX 버퍼에서 읽어온 횟수가 0회 일 때
      // 'ATA' 명령어는 Wi-Fi 모듈의 IP, SubNet, Gateway 정보를 출력하며, ':'문자로 구분
      RX_count = 1; // RX_count 는 Wi-fi Shiled 에 ATA 라는 명령어가 처음 발생하면 0 에서 1로 변경
                    // 만약 ATA 명령어가 실행되었는데 RX_count 가 0 이 아니라면, 이전에 이미 ATA 명령어가 실행된 적 있음
    } else if(RX_count == 4) {
      if(Check_buf[2] != ':') { // Serial1 에서 읽어온 데이터에 ':'가 있다는 것은, ERROR 상태 중 구체적인 오류가 있다는 의미 (ex. [ERROR: INVALID INPUT] 등)
        ID = ID*10 + (Check_buf[2]-'0');
      } else {
        RX_count++; // 아래 RX_count == 5 일 때로 넘어감 (= 아무런 행동을 취하지 않음)
      }
    } else if(RX_count == 5) {
      if(Check_buf[2] == ']') {
        if((Check_buf[0] == 'O') && (Check_buf[1] == 'K')) {  // 즉, Check_buf 의 값이 'OK]' 일 때, (= 오케이 일 때, 위의 예제에서 4 번 경우)
          RX_flag = 1;  // RX 가져올 값이 있음
          delay(1000);
          
          RX_cnt = Serial1.available(); // Serial1 에서 읽어올 데이터 크기를 RX_cnt 에 저장
          Serial.println(RX_cnt);       // Serial1 에서 읽어올 데이터 크기를 사용자에게 알려 줌
          
          while(1) {
            Serial.write(Serial1.read()); // Serial1 데이터를 읽어와, Serial 에 씀
            RX_cnt--; // RX_cnt 값을 감소
            
            if(RX_cnt == 0) // RX_cnt 가 0이라면,
              break;  // 더 이상 읽어올 데이터 없음
          }
    
          Serial.print("\n\rID : ");
          Serial.print(ID);
          
          EEPROM_buf[1] = ID;
          
          if(EEPROM.read(1) != ID) {  // EEPROM 두 번째 번지의 값이 ID 와 다르다면 (= EEPROM 을 수정한 흔적이 있다면)
            for(i=0; i<2; i++) {
              EEPROM.write(i,EEPROM_buf[i]);  // EEPROM 첫 번째 번지는 0xAA, 두 번째 번지는 ID를 기록
            }
          }
          
          TX_buf[3] = ID; // TX_buf 배열 3에 장치 ID 저장 (Wi-Fi Shield Pakcet Info 참조)
        } else {  // Check_buf 의 값이 'OK]' 가 아니라면, (무언가 문제가 있는 상태라면)
          RX_flag = 2;  // 수신 상태에 오류 있음
          
          Serial.print("\n\rWifi Connected Error");
          Serial.print("\n\rPlease reset the ADK-2560"); 
        }
      } // if(Check_buf[2] == ']') {
    } else if((Check_buf[2] == '.') && ((RX_count == 1) || (RX_count == 2) || (RX_count == 3))) {
      // RX_count 가 1, 2, 3 일 경우에는, 이미 'ATA' 명령어가 한 번 이상 실행 된 이후
      // Check_buf[2] 가 '.' 일 경우에는, IP 혹은 SubNet 혹은 Gateway 주소를 출력 중이므로 RX_count 만 증가
      RX_count++; // RX_count 증가
    } else if(RX_count == 1) {
      if(Check_buf[2] == ']') {
        if((Check_buf[0] == 'O') && (Check_buf[1] == 'R')) {  // 즉, 'ATA' 명령어가 실행된 적 있으며, Check_buf 의 값이 'OR]' 일 때, (= ERROR 에러 일 때)
          RX_flag = 2;  // 수신 상태에 오류 있음
          
          Serial.print("\n\rWifi Connected fail");
          Serial.print("\n\rPlease reset the ADK-2560"); 
        }
      } // if(Check_buf[2] == ']') {
    } // else if(RX_count == 1) {
    
  } else if(RX_flag == 1) { // 데이터를 수신할 수 있을 때 (= loop () 함수 작동 중)
    if(Serial1.available() > 16) {  // Serial1 에서 읽어올 데이터 크기가 16Byte 초과라면,
      Serial1.readBytes(RX_buf, 17);  // 앞 17Byte 만 읽어와서 RX_buf 에 저장
      Serial.print(RX_buf[0]);
      Serial.print("       ");
      Serial.print(RX_buf[1]);
      Serial.print("\n");
      if(((uint8_t)RX_buf[0] == 0xA0) && ((uint8_t)RX_buf[1] == 0x0A)) {
        // 데이터가 0xA0 0x0A 로 시작한다면, (수신한 데이터의 맨 처음임, Wi-Fi Shield Pakcet Info 참조)
        for(i=2; i<14; i++) {
          check_sum += (uint8_t)RX_buf[i];
          // Check Sum 데이터로 활용하기 위해, Serial1 에서 수신한 데이터를 uint8_t 로 변환하여 더함
          // 수신한 데이터의 2 에서 13 까지는 ID 와 DATA 영역 (Wi-Fi Shield Pakcet Info 참조)
        }
        
        if(check_sum == (uint8_t)RX_buf[14]) {
          // Check Sum 데이터가 수신한 데이터의 배열 14와 일치한다면, (= Check Sum 결과가 일치한다면)
          Serial.println("\n\r RX Packet data");
          
          for(i=0; i<17; i++) {
            Serial.write(' ');
            Serial.print((uint8_t)RX_buf[i],HEX);
          } // 16진수로 수신한 데이터 출력
          
          TX_flag = RX_buf[4];
          // 데이터를 송신한 ID 를 TX_flag 에 저장
          
          if(!TX_flag) {  // TX_flag 값이 0x00 이라면 (= 데이터를 보낸 ID가 0x00 이라면 (0x00 : 서버) )
            for(i=4; i<14; i++) {
              TX_buf[i] = 0; // TX_buf 배열에서 DATA 영역 초기화 (Wi-Fi Shield Pakcet Info 참조)
            }
            
            TX_buf[14] = TX_buf[2]; // Check Sum 영역을 TX_buf[2] 로 초기화 (TX_buf[2] 는 항상 0x00)
            
            for(i=3; i<14; i++) { // Check Sum 데이터로 활용하기 위해,
              TX_buf[14] += TX_buf[i];
              // 배열 3 에서 13 까지는 ID 와 DATA 영역 (Wi-Fi Shield Pakcet Info 참조)
            }
            
            Serial.println("\n\r TX Packet data");
            
            for(i=0; i<17; i++) {
              Serial.write(' ');
              Serial.print(TX_buf[i],HEX);
              Serial1.write(TX_buf[i]);
              // Check Sum 데이터를 포함하여 Serial (Serial 모니터) 와 Serial1 (Wi-Fi 모듈)에 출력
            } // for(i=0; i<17; i++)
          } // if(!TX_flag)
        } // if(check_sum == (uint8_t)RX_buf[14])
      } // if(((uint8_t)RX_buf[0] == 0xA0) && ((uint8_t)RX_buf[1] == 0x0A))
    } // if(Serial1.available() > 16)
    // 여기에 else 가 없는 것으로 보아, 16Byte 초과되는 패킷만 받는 듯 (즉, 16Byte 이하 패킷은 쓰레기로 처리)
  } // else if(RX_flag == 1)
}

void serialEvent(void) {
  // Serial 에서 읽어올 데이터가 존재할 때 자동으로 실행되는 함수. 그냥 serial 이기 때문에, Serial0 장치와 연관있다.
  Serial1.write(Serial.read());
}

char TEMP_read(void) { // 온도 관련
  char da;
  int temp = 0;
  float value;
  
  temp = analogRead(pin_TEMP);  // 온도 센서로부터 값을 가져옴
  value = (float)(temp/2.048);  // 온도 계산 (1)
  da = value - 50;              // 온도 계산 (2)
  
  return da;
}

void TIMER_ISR(void) {  // 타이머 관련
  Timer_flag = 1; // 타이머가 발생됨
}

void action()
{
    switch (group)
    {
    case GROUP_0:
      switch (idx)
      {
      case G0_GO:
        // write your action code here
        group = GROUP_1;
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      }
      break;
    case GROUP_1:
      switch (idx)
      {
      case G1_HELP:
        TX_buf[8] = G1_HELP+1; 
        break;
      case G1_RESTROOM:
        TX_buf[8] = G1_RESTROOM+1;
        break;
      case G1_THIRSTY:
        TX_buf[8] = G1_THIRSTY+1;
        break;
      case G1_HUNGRY:
        TX_buf[8] = G1_HUNGRY+1;
        break;
      case G1_LIGHTON:
        TX_buf[8] = G1_LIGHTON+1;
        break;
      case G1_LIGHTOFF:
        TX_buf[8] = G1_LIGHTOFF+1;
        break;
      }
      break;
    }
}

bool heartbeatDetected(int IRSensorPin, int delay)
{
  static int maxValue = 0;
  static bool isPeak = false;
  int rawValue;
  bool result = false;
    
  rawValue = analogRead(IRSensorPin);
  // Separated because analogRead() may not return an int
  rawValue *= (1000/delay);
  HBDEBUG(Serial.print(isPeak); Serial.print("p, "));
  HBDEBUG(Serial.print(rawValue); Serial.print("r, "));
  HBDEBUG(Serial.print(maxValue); Serial.print("m, "));

  // If sensor shifts, then max is out of whack.
  // Just reset max to a new baseline.
  if (rawValue * 4L < maxValue) {
    maxValue = rawValue * 0.8;
    HBDEBUG(Serial.print("RESET, "));
  }
  
  // Detect new peak
  if (rawValue > maxValue - (1000/delay)) {
    // Only change peak if we find a higher one.
    if (rawValue > maxValue) {
      maxValue = rawValue;
    }
    // Only return true once per peak.
    if (isPeak == false) {
      result = true;
      Serial.print(result); Serial.print(",  *");
    }
    isPeak = true;
  } else if (rawValue < maxValue - (3000/delay)) {
    isPeak = false;
    // Decay max value to adjust to sensor shifting
    // Note that it may take a few seconds to re-detect
    // the signal when sensor is pushed on meatier part
    // of the finger. Another way would be to track how
    // long since last beat, and if over 1sec, reset
    // maxValue, or to use derivatives to remove DC bias.
    maxValue-=(1000/delay);
 }
  HBDEBUG(Serial.print("\n"));
  return result;
}

