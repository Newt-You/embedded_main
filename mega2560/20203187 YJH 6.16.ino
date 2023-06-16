#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <String.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#define SLAVE 4 // 슬레이브 주소

#define BTN_UP 14
#define BTN_DOWN 15
#define BTN_LEFT 16
#define BTN_RIGHT 17
#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 
struct Product {
  int num = 0;
  String name = "non";
  int price = 10000;
  int count = 10;
};
byte nuidPICC[4];
Product product[10];
int line = 0;
int menu = 1;


// 기본 데이터 설정
void InitData() {
  for(int i = 0; i < 10; i ++) {
    product[i].num = i + 1;
    product[i].name = String(product[i].name + (i + 1));
  }
}


// GLCD 화면 출력
void InitGLCD() {
  Serial.begin(9600);
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  for(int i = 14; i < 18; i ++) {
    pinMode(i, INPUT);
  }
  
  GLCD.Init();
  GLCD.SelectFont(System5x7);
   
}

void ShowMenu() {
  // 첫 줄
  GLCD.CursorToXY(2, 0);
  GLCD.print("Help Me Pos");
  GLCD.DrawLine(0, 8, 127, 8);

  // 끝 줄
  GLCD.DrawLine(0, 53, 127, 53);
  GLCD.CursorToXY(0, 56);
  GLCD.print(" pay  lis  day  all");

  // 메뉴 선택
  GLCD.DrawRoundRect(6 + menu * 30, 55, 25, 10, 2);
}

void ShowList() {
  for(int i = 0; i < 4; i ++) {
    GLCD.CursorToXY(2, i * 11 + 11);
    GLCD.print(product[(i + line)% 10].num);

    GLCD.CursorToXY(20, i * 11 + 11);
    GLCD.print(product[(i + line)% 10].name);

    GLCD.CursorToXY(70, i * 11 + 11);
    GLCD.print(product[(i + line)% 10].price);

    GLCD.CursorToXY(110, i * 11 + 11);
    GLCD.print(product[(i + line)% 10].count);
  }
}



void setup() {
  
  InitGLCD();
  InitData();

}

void loop() {
 
  GLCD.Init();
  ShowMenu();
  ShowList();
 
 // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent()){
   
    return;
  }
  // Verify if the NUID has been readed
  if ( rfid.PICC_ReadCardSerial()){
      line++;
      line%=10;
      return;
  }
  

  // 버튼 처리
  if(digitalRead(BTN_UP)) {
    line ++;
    line %= 10;
  }
  else if(digitalRead(BTN_DOWN)) {
    line --;
    if(line < 0) line = 9;
  }
  else if(!digitalRead(BTN_LEFT)) {
    menu --;
    if(menu < 0) menu = 3;
  }
  else if(!digitalRead(BTN_RIGHT)) {
    menu ++;
    menu %= 4;
  }

  
  delay(50);
}
