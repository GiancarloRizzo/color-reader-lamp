
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
 * SPI SSLED                              5
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define RST_PIN 9     
#define SS_PIN_1 8   
#define SS_PIN_2 10
#define SS_PIN_3 7
#define STRIP_DATA_PIN 5
#define NUM_LEDS 30

String yellow_card = "2442414342";
String blue_card = "14421919850";
String red_card = "21525221425";

MFRC522 mfrc522_1(SS_PIN_1, RST_PIN);   
MFRC522 mfrc522_2(SS_PIN_2, RST_PIN);   
MFRC522 mfrc522_3(SS_PIN_3, RST_PIN);   

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, STRIP_DATA_PIN, NEO_RGB + NEO_KHZ800); 
int pixels_brightness = 15;

String uid1 = "";
String uid2 = "";
String uid3 = "";

int pixels_delay = 0;
int card_delay = 650;



/*********************************************************************************************************************************************************************************/

void init_readers(void);
void choose_color(String, String);
void control_pixels(int, int, int);
int *calc_rgb(String);
void reset_uids(void);

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

  //reader 3
  if (mfrc522_3.PICC_IsNewCardPresent() && mfrc522_3.PICC_ReadCardSerial() ) {
    for (byte i = 0; i < mfrc522_3.uid.size; i++) {
      uid3.concat(mfrc522_3.uid.uidByte[i]);
    } 
    mfrc522_3.PICC_HaltA();
  } else {
    uid3="";
  }
  delay(card_delay);
  
  choose_color(uid1, uid2, uid3); 

  reset_uids();
}



/*********************************************************************************************************************************************************************************/

void reset_uids(){
  uid1="";
  uid2="";
  uid3="";
}


void init_readers(){
  mfrc522_1.PCD_Init();
  mfrc522_2.PCD_Init();
  mfrc522_3.PCD_Init();
}


void choose_color(String uid1, String uid2, String uid3){
  int* grb;
  
  if (uid1==yellow_card || uid2==yellow_card || uid3==yellow_card){
    if(uid1==blue_card || uid2==blue_card){
      grb = calc_rgb("green");
    } else if(uid1==red_card || uid2==red_card || uid3==red_card){
      grb = calc_rgb("orange");
    } else {
      grb = calc_rgb("yellow");
    }
  } else if (uid1==blue_card || uid2==blue_card || uid3==blue_card){
    if(uid1==yellow_card || uid2==yellow_card){
      grb = calc_rgb("green");
    } else if(uid1==red_card || uid2==red_card || uid3==red_card){
      grb = calc_rgb("violet");
    } else {
      grb = calc_rgb("blue");
    }
  } else if (uid1==red_card || uid2==red_card || uid3==red_card){
    if(uid1==yellow_card || uid2==yellow_card || uid3==yellow_card){
      grb = calc_rgb("orange");
    } else if(uid1==blue_card || uid2==blue_card || uid3==blue_card){
      grb = calc_rgb("violet");
    } else {
      grb = calc_rgb("red");
    }
  } else if (uid1=="" || uid2=="" || uid3=="") {
    grb = calc_rgb("black");
  } else {
    grb = calc_rgb("white");
  }

  control_pixels(grb[0],grb[1],grb[2]);
  reset_uids();
}


void control_pixels(int g, int r , int b){
  pixels.setBrightness(pixels_brightness);
  for(int i=0; i < NUM_LEDS; i++){
    pixels.setPixelColor(i, pixels.Color(g,r,b));
  }
  pixels.show();
  delay(pixels_delay);
}


int *calc_rgb(String colorname){
  if(colorname=="yellow"){
    int grb[] = {255,255,0};
    return *grb;
  } else if (colorname=="orange") {
    int grb[] = {120,255,0};
    return *grb;
  } else if (colorname=="red") {
    int grb[] = {0,255,0};
    return *grb;
  } else if (colorname=="violet") {
    int grb[] = {0,255,255};
    return *grb;
  } else if (colorname=="blue") {
    int grb[] = {0,0,255};
    return *grb;
  } else if (colorname=="green") {
    int grb[] = {255,0,0};
    return *grb;
  } else if (colorname=="white") {
    int grb[] = {255,255,255};
    return *grb;
  } else {//black
    int grb[] = {0,0,0};
    return *grb;
  } 
}
