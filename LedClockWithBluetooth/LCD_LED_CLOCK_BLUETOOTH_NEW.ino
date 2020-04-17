#include "bday_config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3232RTC.h>
#include<SoftwareSerial.h>
#define DS3231_I2C_ADDRESS 104

//backlight
const int backLightOFFHour = 22;
const int backLightONHour = 6;

//bluetooth
const int bttx = 9;  //tx of bluetooth module is connected to pin 9 of arduino
const int btrx = 10;  //rx of bluetooth module is connected to pin 10 of arduino
SoftwareSerial bluetooth(bttx, btrx);
String Data = ""; //initialise a string to hold my message

byte tMSB, tLSB;
float temp3231;

boolean birbiBdayFlag;

boolean isBacklightOn;

float celsius;
char buf1[40];
char buf2[40];

//global time variable
time_t t;

//RTC instance
DS3232RTC myRTC(false);

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin();
  bluetooth.begin(9600);
  Serial.begin(9600);
  myRTC.begin();
  lcd.begin();
  lcd.createChar(0, heart);
  lcd.createChar(1, celsDegrees);

  isBacklightOn = true;

  t = myRTC.get();

  birbiBdayFlag = isBirbiBdayYet();

  //setRTC(); //setting the time, use only for that
}

void loop() {


  Serial.println("isBacklightOn: ");
  Serial.println(isBacklightOn);

  t = myRTC.get();
  setLCDDisplay();
  setBacklightBoolOnAndOff();
  turnOnAndOffBackLigth();
  setBackLigthBoolWithBluetooth();

  birbiBdayFlag = isBirbiBdayYet();

}


void setRTC() {
  tmElements_t tm;
  tm.Hour = 9;             //set the tm structure to 23h31m30s on 13Feb2009
  tm.Minute = 39;
  tm.Second = 10;
  tm.Day = 30;
  tm.Month = 3;
  tm.Year = 2020 - 1970;    //tmElements_t.Year is the offset from 1970
  RTC.write(tm);            //set the RTC from the tm structure
}


void setLCDDisplay() {
  getData();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(buf1);
  lcd.setCursor(0, 1);
  lcd.print("Celsius: ");
  lcd.print(celsius); //get3231Temp() backup solution
  lcd.write(byte(1));

  delay(7000);

  if (birbiBdayFlag) {
    lcd.setCursor(0, 0);
    lcd.print("Happy");
    lcd.write(byte(0));
    lcd.print("Bday");
    lcd.write(byte(0));
    lcd.print("Birbi!");
    lcd.setCursor(0, 1);
    lcd.print("Celsius: ");
    lcd.print(celsius);
    lcd.write(byte(1));
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Date:");
    lcd.print(buf2);
    lcd.setCursor(0, 1);
    lcd.print("Celsius: ");
    lcd.print(celsius);
    lcd.write(byte(1));
  }

  delay(3000);

}

void getData() {
  //for debuging
  char buf[40];
  sprintf(buf, "%.2d:%.2d:%.2d %.2d%s%d ",
          hour(t), minute(t), second(t), day(t), monthShortStr(month(t)), year(t));
  Serial.println(buf);
  Serial.println(" ");

  //the values
  celsius = myRTC.temperature() / 4.0;

  sprintf(buf1, "%.2d:%.2d",
          hour(t), minute(t));

  sprintf(buf2, "%s.%.2d.%d ",
          monthShortStr(month(t)), day(t), year(t));


}

//checks if today is Birbi's Bday
boolean isBirbiBdayYet() {
  String monthStr;
  String dayStr;
  int dayInt;

  int isBirbiBdayDay = 26; //26
  String isBirbiBdayMonth = "Mar";

  char bufMonth[10];
  char bufDay[10];

  sprintf(bufMonth, "%s",
          monthShortStr(month(t)));

  sprintf(bufDay, "%.2d",
          day(t));

  monthStr = String(bufMonth);
  dayStr = String(bufDay);
  dayInt = dayStr.toInt();

  if (isBirbiBdayDay == dayInt && isBirbiBdayMonth.equals(monthStr))
    return true;
  else
    return false;
}


void setBacklightBoolOnAndOff() {
  String strHour;
  char bufHour[10];

  sprintf(bufHour, "%.2d",
          hour(t));
  strHour = String(bufHour);
  int intHour = strHour.toInt();

  
  if (intHour == backLightONHour)
    isBacklightOn = true;
  else if (intHour == backLightOFFHour)
    isBacklightOn = false;
   

  }


void turnOnAndOffBackLigth() {
  if (isBacklightOn == true)
    lcd.backlight();
  else
    lcd.noBacklight();
}


//backup temp method
float get3231Temp() {

  //temp registers (11h-12h) get updated automatically every 64s
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 2);

  if (Wire.available()) {
    tMSB = Wire.read(); //2's complement int portion
    tLSB = Wire.read(); //fraction portion

    temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
    temp3231 += ( (tLSB >> 6) * 0.25 ); //only care about bits 7 & 8
  }
  else {
    //oh noes, no data!
  }

  return temp3231;
}

void setBackLigthBoolWithBluetooth() {
  if (bluetooth.available() > 0) {
    while (bluetooth.available()) {
      char character = bluetooth.read(); // Receive a single character from the software serial port
      Data.concat(character); // Add the received character to the receive buffer
      if (character == '#') { //If end of message character recieved, move on.

        Serial.print(Data); //Output the message

        if (Data.equals("on#")) {
          isBacklightOn = true;
        }

        if (Data.equals("off#")) {
          isBacklightOn = false;
        }
        Data = ""; //clear the buffer/message
        Serial.println();
        delay(1000);
      }
    }
  } else {
    Serial.println("no signal");

  }

}
