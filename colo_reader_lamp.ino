
/*
 *             MFRC522      Arduino       WS2812B
 *             Reader/PCD   Uno/101       LED-pixels
 * Signal      Pin          Pin           Pin
 * ---------------------------------------
 * RST/Reset   RST          9             
 * SPI SS      SDA(SS)      8
 * SPI SS      SDA(SS)      10            
 * SPI MOSI    MOSI         11 / ICSP-4   
 * SPI MISO    MISO         12 / ICSP-1   
 * SPI SCK     SCK          13 / ICSP-3   
 * SPI SSLED                              5
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define RST_PIN 9     
#define SS_PIN_1 8   
#define SS_PIN_2 10
#define STRIP_DATA_PIN 5
#define NUM_LEDS 30

String yellow_card = "2442414342";
String blue_card = "14421919850";
String red_card = "21525221425";

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN);   
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN);   

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, STRIP_DATA_PIN, NEO_RGB + NEO_KHZ800); 
int pixels_brightness = 15;

String uid1 = "";
String uid2 = "";

int pixels_delay = 0;
int card_delay = 1000;



/*********************************************************************************************************************************************************************************/

void init_readers(void);
void calc_colors(String, String);
void control_pixels(int, int, int);

/*********************************************************************************************************************************************************************************/



void setup() {
  //readers
  Serial.begin(9600);

  //rfid  
  SPI.begin();         
  init_readers();
  
  //LED-pixels
  pixels.begin();
  control_pixels(0,0,0); //off
}



void loop() {
  init_readers();

  //reader 1
  if (mfrc522_1.PICC_IsNewCardPresent() && mfrc522_1.PICC_ReadCardSerial() ) {
    for (byte i = 0; i < mfrc522_1.uid.size; i++) {
      uid1.concat(mfrc522_1.uid.uidByte[i]);
    } 
    mfrc522_1.PICC_HaltA(); 
  } else {
    uid1="";
  }
  delay(card_delay);
  
  //reader 2
  if (mfrc522_2.PICC_IsNewCardPresent() && mfrc522_2.PICC_ReadCardSerial() ) {
    for (byte i = 0; i < mfrc522_2.uid.size; i++) {
      uid2.concat(mfrc522_2.uid.uidByte[i]);
    } 
    mfrc522_2.PICC_HaltA();
  } else {
    uid2="";
  }
  delay(card_delay);
  calc_color(uid1, uid2); 
  uid1="";
  uid2="";
}



/*********************************************************************************************************************************************************************************/



void calc_color(String uid1, String uid2){
  int r=0;
  int g=0;
  int b=0;
  Serial.println(uid1);
  Serial.println(uid2);
  if (uid1==yellow_card || uid2==yellow_card){
    if(uid1==blue_card || uid2==blue_card){
      Serial.println("yellow:green");
      r=0;g=255;b=0;  //green
    } else if(uid1==red_card || uid2==red_card){
      Serial.println("yellow:orange");
      r=255;g=120;b=0; //orange
    } else {
      Serial.println("yellow");
      r=255;g=255;b=0; //yellow
    }
  } else if (uid1==blue_card || uid2==blue_card){
    if(uid1==yellow_card || uid2==yellow_card){
      Serial.println("blue:green");
      r=0;g=255;b=0;  //green
    } else if(uid1==red_card || uid2==red_card){
      Serial.println("blue:violet");
      r=255;g=0;b=255; //violet 
    } else {
      Serial.println("blue");
      r=0;g=0;b=255; //blue
    }
  } else if (uid1==red_card || uid2==red_card){
    if(uid1==yellow_card || uid2==yellow_card){
      Serial.println("red:orange");
      r=255;g=120;b=0; //orange
    } else if(uid1==blue_card || uid2==blue_card){
      Serial.println("red:violet");
      r=255;g=0;b=255; //violet 
    } else {
      Serial.println("red");
      r=255;g=0;b=0; //red
    }
  } else {
    r=0;g=0;b=0; //off
  }

  control_pixels(r,g,b);
  uid1="";
  uid2="";
}



void control_pixels(int r, int g , int b){
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);

  pixels.setBrightness(pixels_brightness);
  for(int i=0; i < NUM_LEDS; i++){
    pixels.setPixelColor(i, pixels.Color(g,r,b));
  }
  pixels.show();
  delay(pixels_delay);
}



void init_readers(){
  mfrc522_1.PCD_Init();
  mfrc522_2.PCD_Init();
}
