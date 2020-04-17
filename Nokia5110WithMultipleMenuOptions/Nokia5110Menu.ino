#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <DS3232RTC.h>

//https://www.youtube.com/watch?v=VtZvf5T98FI
//http://educ8s.tv/arduino-nokia-5110-menu/

boolean backlight = true;
int contrast=20;

int menuitem = 1;
int subMenuItem =1;
int page = 1;
int timerValue = 0;
boolean timerFlag = false;

volatile boolean up = false;
volatile boolean down = false;
volatile boolean middle = false;

int downButtonState = 0;
int upButtonState = 0;  
int selectButtonState = 0;          
int lastDownButtonState = 0;
int lastSelectButtonState = 0;
int lastUpButtonState = 0;

DS3232RTC myRTC;  
time_t t;

Adafruit_PCD8544 display = Adafruit_PCD8544( 5, 4, 3);

void setup() {

  pinMode(2, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(7,OUTPUT);

  digitalWrite(7,LOW); //Turn Backlight ON
  myRTC.begin();  
  setClock();
  Serial.begin(9600);
  
  display.begin();      
  display.setContrast(contrast); //Set contrast to 20
  display.clearDisplay(); 
  display.display();   
}

void loop(){
  delay(100);
  
  drawMenu();

  downButtonState = digitalRead(2);
  selectButtonState = digitalRead(1);
  upButtonState =   digitalRead(0);
 
  checkIfDownButtonIsPressed();
  checkIfUpButtonIsPressed();
  checkIfSelectButtonIsPressed();


  if (up && page == 1 ) {
    up = false;
    menuitem--;
    if (menuitem==0){menuitem=3;}      
  }else if (up && page == 4 ) {
    up = false;
    contrast--; //adjust constrast down
    setContrast();
  }
  if (up && page == 3){
    up = false; 
    subMenuItem--;
    if (subMenuItem==0){subMenuItem = 3;}      
    }
  
  if(up && page ==2){
    up = false;
    startTimer();
    }

   if(down && page ==2){
    up = false;
    stopTimer();
    }
  
  if (down && page == 1) {
    down = false;
    menuitem++;
    if (menuitem==4){menuitem=1;}      
  }else if (down && page == 4 ) {
    down = false;
    contrast++; //adjust constrast up
    setContrast();
  }
  
  if(down && page == 3){
    down = false;
    subMenuItem++;
    if(subMenuItem == 4){subMenuItem = 1;}
    }

  if(middle && (page == 1 && menuitem == 3)){ 
      middle = false;
      page=3;
      }else if(middle && page == 3 && subMenuItem == 1){
        middle = false;
        if (backlight){
        backlight = false;
        turnBacklightOff();
        }else {
        backlight = true; 
        turnBacklightOn();
       }
      }else if(middle && (page == 3 && subMenuItem == 2)){
        middle = false;
        page = 4;
       }else if(middle && (page == 3 && subMenuItem == 3)){
        middle = false;
        page = 1;
        }

      if(middle && page==4){
        middle = false;
        page = 3; 
        }

      if(middle &&(page == 1 && menuitem == 1)){
        middle = false;
        page = 2;
        }else if(middle && page == 2){
        middle = false;
        page =1;
        }

        if(middle &&(page == 1 && menuitem == 2)){
        middle = false;
        page = 5;
        }else if(middle && page == 5){
        middle = false;
        page = 1;
        }
        
  }

  void checkIfDownButtonIsPressed(){
    if (downButtonState != lastDownButtonState) {
       if (downButtonState == 0) {
      down=true;
    }
    delay(50);
  }
   lastDownButtonState = downButtonState;
  }

void checkIfUpButtonIsPressed(){
  if (upButtonState != lastUpButtonState){
    if (upButtonState == 0) {
      up=true;
    }
    delay(50);
  }
   lastUpButtonState = upButtonState;
}

void checkIfSelectButtonIsPressed(){
   if (selectButtonState != lastSelectButtonState){
    if (selectButtonState == 0) {
      middle=true;
    }
    delay(50);
  }
   lastSelectButtonState = selectButtonState;
}

  
  void drawMenu(){
    
  if (page==1){    
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("MAIN MENU");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(0, 15);
   
    if (menuitem==1){ 
      display.setTextColor(WHITE, BLACK);
    }else{
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Timer");
    display.setCursor(0, 22);
   
    if (menuitem==2) {
      display.setTextColor(WHITE, BLACK);
    }else{
      display.setTextColor(BLACK, WHITE);
    }    
    display.print(">Clock");
    display.display();
    
    if (menuitem==3){ 
      display.setTextColor(WHITE, BLACK);
    }else{
      display.setTextColor(BLACK, WHITE);
    }  
    display.setCursor(0, 32);
    display.print(">Settings");
    display.display();

    
  }else if(page == 2){
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("Timer");
    display.drawFastHLine(0,10,83,BLACK);
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.print(timerValue);
    display.display();
  }else if (page==4){ 
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("CONTRAST");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 15);
    display.print("Value");
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.print(contrast);
 
    display.setTextSize(2);
    display.display();
    
  }else if(page==3){
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("SETTINGS");
    display.drawFastHLine(0,10,83,BLACK);
    
    if (subMenuItem==1){ 
      display.setTextColor(WHITE, BLACK);
    }else{
      display.setTextColor(BLACK, WHITE);
    }  
    display.setCursor(5, 15);
    display.print("Light:");
     if (backlight){
      display.print("ON");
    }else{
      display.print("OFF");
    }
    display.display();

     if (subMenuItem==2){ 
      display.setTextColor(WHITE, BLACK);
    }else{
      display.setTextColor(BLACK, WHITE);
    }  
    display.setCursor(5, 25);
    display.print("Contrast");
    display.display();

    if (subMenuItem==3){ 
      display.setTextColor(WHITE, BLACK);
    }else{
      display.setTextColor(BLACK, WHITE);
    }  
    display.setCursor(5, 35);
    display.print("Back");
    display.display();
    }else if(page == 5){
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("CLOCK");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 25);
   
    
    display.display();
      
      }
  
  }

/*
  void resetDefaults(){
    contrast = 20;
    setContrast();
    backlight = true;
    turnBacklightOn();
  }
  */

  void setContrast(){
    display.setContrast(contrast);
    display.display();
  }

  void stopTimer(){
    while(timerFlag){
    timerValue++;
    delay(1000);
    }
    }

  void startTimer(){
    timerFlag = false;
    }

  void turnBacklightOn(){digitalWrite(7,LOW);}
  void turnBacklightOff(){digitalWrite(7,HIGH);}


//real time clock init

void setClock(){
tmElements_t tm;
tm.Hour = 14;             
tm.Minute = 24;
tm.Second = 00;
tm.Day = 19;
tm.Month = 3;
tm.Year = 2020 - 1970;    
RTC.write(tm);        
}

void clockDisplay(){
    char buf[40];
    t = myRTC.get();
    float celsius = myRTC.temperature() / 4.0;
    float fahrenheit = celsius * 9.0 / 5.0 + 32.0;
    sprintf(buf, "%.2d:%.2d:%.2d %.2d%s%d ",
      hour(t), minute(t), second(t), day(t), monthShortStr(month(t)), year(t));
 
    Serial.print(buf);
    Serial.print(" ");
    Serial.print(celsius);
    Serial.println("C ");
    Serial.print(fahrenheit);
    Serial.println("F");
}
  
