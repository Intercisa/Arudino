
const int led = 13;
const int SHORT_BLINK = 300;
const int LONG_BLINK = 1000;
const int ARR_SIZE = 6;
String testWord = "SOS";
typedef struct keyAndValue_ {
  char keyLetter;
  int value[ARR_SIZE];
} keyAndValue_t;

  keyAndValue_t myMorseArray[] = {
   {'A',{SHORT_BLINK,LONG_BLINK}},
   {'B',{LONG_BLINK,SHORT_BLINK, SHORT_BLINK, SHORT_BLINK}},
   {'C',{LONG_BLINK,SHORT_BLINK,LONG_BLINK,SHORT_BLINK}},
   {'D',{LONG_BLINK,SHORT_BLINK,SHORT_BLINK}},
   {'E',{SHORT_BLINK}},
   {'F',{SHORT_BLINK,SHORT_BLINK,LONG_BLINK,SHORT_BLINK}},
   {'G',{LONG_BLINK,LONG_BLINK,SHORT_BLINK}},
   {'H',{SHORT_BLINK,SHORT_BLINK,SHORT_BLINK,SHORT_BLINK}},
   {'I',{SHORT_BLINK,SHORT_BLINK}},
   {'J',{SHORT_BLINK,LONG_BLINK,LONG_BLINK,LONG_BLINK }},
   {'K',{LONG_BLINK,SHORT_BLINK,LONG_BLINK}},
   {'L',{SHORT_BLINK,LONG_BLINK,SHORT_BLINK,SHORT_BLINK}},
   {'M',{LONG_BLINK,LONG_BLINK}},
   {'N',{LONG_BLINK,SHORT_BLINK}},
   {'O',{LONG_BLINK,LONG_BLINK,LONG_BLINK}},
   {'P',{SHORT_BLINK,LONG_BLINK,LONG_BLINK,SHORT_BLINK}},
   {'Q',{LONG_BLINK,LONG_BLINK,SHORT_BLINK,LONG_BLINK}},
   {'R',{SHORT_BLINK,LONG_BLINK,SHORT_BLINK}},
   {'S',{SHORT_BLINK,SHORT_BLINK,SHORT_BLINK}},
   {'T',{LONG_BLINK}},
   {'U',{SHORT_BLINK,SHORT_BLINK,LONG_BLINK}},
   {'V',{SHORT_BLINK,SHORT_BLINK,SHORT_BLINK,LONG_BLINK}},
   {'W',{SHORT_BLINK,LONG_BLINK,LONG_BLINK}},
   {'X',{LONG_BLINK,SHORT_BLINK,SHORT_BLINK,LONG_BLINK}},
   {'Y',{LONG_BLINK,SHORT_BLINK,LONG_BLINK,LONG_BLINK}},
   {'Z',{LONG_BLINK,LONG_BLINK,SHORT_BLINK,SHORT_BLINK}},
   {'1',{SHORT_BLINK,LONG_BLINK,LONG_BLINK,LONG_BLINK,LONG_BLINK}},
   {'2',{SHORT_BLINK,SHORT_BLINK,LONG_BLINK,LONG_BLINK,LONG_BLINK}},
   {'3',{SHORT_BLINK,SHORT_BLINK,SHORT_BLINK,LONG_BLINK,LONG_BLINK}},
   {'4',{SHORT_BLINK,SHORT_BLINK,SHORT_BLINK,SHORT_BLINK,LONG_BLINK}},
   {'5',{SHORT_BLINK,SHORT_BLINK,SHORT_BLINK,SHORT_BLINK,SHORT_BLINK}},
   {'6',{LONG_BLINK,SHORT_BLINK,SHORT_BLINK,SHORT_BLINK,SHORT_BLINK}},
   {'7',{LONG_BLINK,LONG_BLINK,SHORT_BLINK,SHORT_BLINK,SHORT_BLINK}},
   {'8',{LONG_BLINK,LONG_BLINK,LONG_BLINK,SHORT_BLINK,SHORT_BLINK}},
   {'9',{LONG_BLINK,LONG_BLINK,LONG_BLINK,LONG_BLINK,SHORT_BLINK}},
   {'0',{LONG_BLINK,LONG_BLINK,LONG_BLINK,LONG_BLINK,LONG_BLINK}}
};

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i< sizeof(testWord.length()); i++){
  for(int j=0; j <= sizeof(myMorseArray)/sizeof(*myMorseArray); j++){
    char writeKey = myMorseArray[j].keyLetter;
    if(writeKey == testWord[i]){
      for(int k = 0; k<ARR_SIZE; k++){
        if(myMorseArray[j].value[k]!=0){
          Serial.print(testWord[i]);
          Serial.print(" ");
          Serial.println(myMorseArray[j].value[k]);

          digitalWrite(led, HIGH);
          delay(myMorseArray[j].value[k]);
          digitalWrite(led, LOW);
          }
          delay(500);
        }
      
      }
      
    }
 
    }
 
}
