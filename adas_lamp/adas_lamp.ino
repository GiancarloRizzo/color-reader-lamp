
/*
 *             MFRC522      Arduino       WS2812B
 *             Reader/PCD   Uno/101       LED-pixels
 * Signal      Pin          Pin           Pin
 * ---------------------------------------
 * RST/Reset   RST          9             
 * SPI SS      SDA(SS)      7
 * SPI SS      SDA(SS)      8
 * SPI SS      SDA(SS)      10            
 * SPI MOSI    MOSI         11 / ICSP-4   
 * SPI MISO    MISO         12 / ICSP-1   
 * SPI SCK     SCK          13 / ICSP-3   
 * SPI SSLED                              6
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define RST_PIN 9     
#define SS_PIN_1 8   
#define SS_PIN_2 10
#define SS_PIN_3 7

#define STRIP_DATA_PIN 6
#define NUM_LEDS 33

#define DEBUG false
#define Serial if(DEBUG)Serial


/*********************************************************************************************************************************************************************************/


String yellow_card = "2442414342";
String blue_card = "14421919850";
String red_card = "21525221425";

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN);   
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN);   
MFRC522 mfrc522_3(SS_PIN_3, RST_PIN);   

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, STRIP_DATA_PIN, NEO_RGB + NEO_KHZ800); 
int pixels_brightness = 15;
int grb[3];

String uid1 = "";
String uid2 = "";
String uid3 = "";

int pixels_delay = 0;
int card_delay = 200;


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
    Serial.println("no Card detected");
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
    Serial.println("no Card detected");
  }
  delay(card_delay);

  //reader 3
  if (mfrc522_3.PICC_IsNewCardPresent() && mfrc522_3.PICC_ReadCardSerial() ) {
    for (byte i = 0; i < mfrc522_3.uid.size; i++) {
      uid3.concat(mfrc522_3.uid.uidByte[i]);
    } 
    mfrc522_3.PICC_HaltA();
  } else {
    uid3="";
    Serial.println("no Card detected");
  }
  
  calc_color(uid1, uid2, uid3); 
  reset_uids();
}



/********************************************************************************************************************************************************************************/

void calc_color(String uid1, String uid2, String uid3){
  Serial.println(uid1);
  Serial.println(uid2);
  Serial.println(uid3);
  
  if (uid1==yellow_card){
    if(uid2==blue_card){
      Serial.println("yellow:green");
      green(grb);
    } else if(uid3==red_card){
      Serial.println("yellow:orange");
      orange(grb);
    } else {
      Serial.println("yellow");
      yellow(grb);
    }
  } else if (uid2==blue_card){
    if(uid1==yellow_card){
      Serial.println("blue:green");
      green(grb);
    } else if(uid3==red_card){
      Serial.println("blue:violet");
      violet(grb);
    } else {
      Serial.println("blue");
      blue(grb);
    }
  } else if (uid3==red_card){
    if(uid1==yellow_card){
      Serial.println("red:orange");
      orange(grb);
    } else if(uid2==blue_card){
      Serial.println("red:violet");
      violet(grb);
    } else {
      Serial.println("red");
      red(grb);
    }
  } else if (uid1==yellow_card && uid2==blue_card && uid3==red_card){
    Serial.println("rainbow");
    rainbow_surprise(grb);
  } else {
    Serial.println("black");
    black(grb);
  } 

  control_pixels(grb[0], grb[1], grb[2]);
}

void reset_uids(){
  uid1="";
  uid2="";
  uid3="";
}

void control_pixels(int g, int r , int b){
  Serial.println(g);
  Serial.println(r);
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


/****************************************************************************************************************************************************************************************/

void yellow(int grb[]){
  grb[0] = 255;
  grb[1] = 255;
  grb[2] = 0;
}
void orange(int grb[]){
  grb[0] = 120;
  grb[1] = 255;
  grb[2] = 0;
}
void red(int grb[]){
  grb[0] = 0;
  grb[1] = 255;
  grb[2] = 0;
}
void violet(int grb[]){
  grb[0] = 0;
  grb[1] = 255;
  grb[2] = 255;
}
void blue(int grb[]){
  grb[0] = 0;
  grb[1] = 0;
  grb[2] = 255;
}
void green(int grb[]){
  grb[0] = 255;
  grb[1] = 0;
  grb[2] = 0;
}
void black(int grb[]){
  grb[0] = 0;
  grb[1] = 0;
  grb[2] = 0;
}
void white(int grb[]){
  grb[0] = 255;
  grb[1] = 255;
  grb[2] = 255;
}

void rainbow_surprise(int grb[]){   
  grb = yellow();
  control_pixels(grb[0],grb[1],grb[2]);
  pixels.show();
  delay(rainbow_delay);
  
  grb = orange();
  control_pixels(grb[0],grb[1],grb[2]);
  pixels.show();
  delay(rainbow_delay);
  
  grb = red();
  control_pixels(grb[0],grb[1],grb[2]);
  pixels.show();
  delay(rainbow_delay);
  
  grb = violet();
  control_pixels(grb[0],grb[1],grb[2]);
  pixels.show();
  delay(rainbow_delay);
  
  grb = blue();
  control_pixels(grb[0],grb[1],grb[2]);
  pixels.show();
  delay(rainbow_delay);
  
  grb = green();
  control_pixels(grb[0],grb[1],grb[2]);
  pixels.show();
  delay(rainbow_delay);
  
  grb = white();
  control_pixels(grb[0],grb[1],grb[2]);
  pixels.show();
  delay(rainbow_delay);
}
