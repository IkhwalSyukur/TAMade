/*************************************************************

  Blynk can provide your device with time data, like an RTC.
  Please note that the accuracy of this method is up to several seconds.

  App project setup:
    RTC widget (no pin required)
    Value Display widget on V1
    Value Display widget on V2

  WARNING :
  For this example you'll need Time keeping library:
    https://github.com/PaulStoffregen/Time

  This code is based on an example from the Time library:
    https://github.com/PaulStoffregen/Time/blob/master/examples/TimeSerial/TimeSerial.ino
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPL1lHjCEw3"
#define BLYNK_DEVICE_NAME           "TMPL1lHjCEw3"
#define BLYNK_AUTH_TOKEN            "u_qwMZPaXt3pDS7xX6gvilAHBlVn2w_O"
#include <HTTPClient.h>
#include "time.h"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns
#define SS_PIN    5  // ESP32 pin GIOP5 
#define RST_PIN   4 // ESP32 pin GIOP27 
#define RELAY_PIN 22 // ESP32 pin GIOP22 connects to relay

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networky]s.
char ssid[] = "Variasi Aluminium";///Enter your wifi name
char pass[] = "hapisahsyukur1";// Enter wifi password

BlynkTimer timer;

WidgetRTC rtc;

String masuk = "";
String currentTime = "";
String currentDate = "";

// Digital clock display of the time
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V2, currentDate);
}

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}

void sendSensor()
{
 
  Blynk.virtualWrite(V0, masuk); // select your virtual pins accordingly
}

LiquidCrystal_I2C lcd(0x27, 16, 2);

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

int kondisi = 1;
MFRC522 rfid(SS_PIN, RST_PIN);

byte keyTagUID[7] = {0x04, 0x48, 0x1B, 0xF2, 0x9D, 0x53, 0x80};
byte keyTagUID1[7] = {0x04, 0x60, 0x7A, 0xFA, 0x4A, 0x57, 0x80};
byte keyTagUID2[7] = {0x04, 0x4D, 0x4C, 0xBA, 0x88, 0x48, 0x80};
byte keyTagUID3[7] = {0x04, 0x8A, 0x54, 0xD2, 0x12, 0x5E, 0x80};
byte keyTagUID4[7] = {0x04, 0x2F, 0x75, 0x6A, 0xFA, 0x6E, 0x80};
byte keyTagUID5[7] = {0x04, 0x97, 0x3F, 0x6A, 0x44, 0x5A, 0x80};
byte keyTagUID6[7] = {0x04, 0x23, 0x82, 0xFA, 0x48, 0x2A, 0x80};
byte keyTagUID7[7] = {0x04, 0x9C, 0x59, 0x6A, 0xAE, 0x52, 0x80};
byte keyTagUID8[7] = {0x05, 0x8F, 0x58, 0x64, 0xF6, 0x31, 0x00};
byte keyTagUID9[7] = {0x05, 0x87, 0x83, 0x0E, 0x24, 0xE1, 0x00};
byte pin_rows[ROW_NUM]      = {13, 12, 14, 27}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte pin_column[COLUMN_NUM] = {26, 25, 33, 32};   // GIOP16, GIOP4, GIOP0, GIOP2 connect to the column pins
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const String password = "1111"; // change your password here
String input_password;

const int RELAY = 17; 
const int Tombol = 16;
const int Buzzer = 15;
const int red = 1;
const int green =3;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbwOx5ooX3dVW-UjhrrkRVTFl3dvZfJjbRyY88iZRUANM1qI4kaiRyNabZutQ3_tSxOU";    // change Gscript ID
int count = 0;

void setup()
{

  // connect to WiFi
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
  input_password.reserve(32);
  pinMode(RELAY, OUTPUT);
  pinMode(Tombol, INPUT_PULLUP);
  pinMode(red, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(green, OUTPUT);
  lcd.begin();
  lcd.backlight();
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)

  // Display digital clock every 10 seconds
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(10000L, clockDisplay);

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
}



void loop()
{
  int Push_button_state = digitalRead(Tombol);
  if ( Push_button_state == LOW )
      { 
        digitalWrite(RELAY, HIGH); 
        Serial.println("push");
        delay(3000);
        digitalWrite(RELAY,LOW);
      }
  if (kondisi ==1){
    lcd.setCursor(0,0);
    lcd.print("Scan KTP Anda");
//    Serial.println(kondisi);
    if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      if (rfid.uid.uidByte[0] == keyTagUID[0] &&
          rfid.uid.uidByte[1] == keyTagUID[1] &&
          rfid.uid.uidByte[2] == keyTagUID[2] &&
          rfid.uid.uidByte[3] == keyTagUID[3] &&
          rfid.uid.uidByte[4] == keyTagUID[4] &&
          rfid.uid.uidByte[5] == keyTagUID[5] &&
          rfid.uid.uidByte[6] == keyTagUID[6] ) {
        Serial.println("Access is granted to Ikhwal");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("Ikhwal");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "ikhwal";
        kondisi = 0;
        Blynk.run();
        timer.run();
        

/////////////////////////////////////////////////////////////////////
if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
//    String currentTime = String(hour()) + ":" + minute() + ":" + second();
//    String currentDate = String(day()) + " " + month() + " " + year();
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"nama=" + "iwal" + "&waktu=" + asString + "&tanggal=" + currentDate;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
/////////////////////////////////////////////////////////////////////////
        
      }

 else if (rfid.uid.uidByte[0] == keyTagUID1[0] &&
          rfid.uid.uidByte[1] == keyTagUID1[1] &&
          rfid.uid.uidByte[2] == keyTagUID1[2] &&
          rfid.uid.uidByte[3] == keyTagUID1[3] &&
          rfid.uid.uidByte[4] == keyTagUID1[4] &&
          rfid.uid.uidByte[5] == keyTagUID1[5] &&
          rfid.uid.uidByte[6] == keyTagUID1[6] ) {
        Serial.println("Access is granted to Ali");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("Ali");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "Ali";
        kondisi = 0;
        Blynk.run();
        timer.run();
      }
/////////////////////////////////////////////////
else if (rfid.uid.uidByte[0] == keyTagUID2[0] &&
          rfid.uid.uidByte[1] == keyTagUID2[1] &&
          rfid.uid.uidByte[2] == keyTagUID2[2] &&
          rfid.uid.uidByte[3] == keyTagUID2[3] &&
          rfid.uid.uidByte[4] == keyTagUID2[4] &&
          rfid.uid.uidByte[5] == keyTagUID2[5] &&
          rfid.uid.uidByte[6] == keyTagUID2[6] ) {
        Serial.println("Access is granted to made widi");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("made widi");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "made widi";
        kondisi = 0;
        Blynk.run();
        timer.run();

/////////////////////////////////////////////////////////////////////
if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
//    String currentTime = String(hour()) + ":" + minute() + ":" + second();
//    String currentDate = String(day()) + " " + month() + " " + year();
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"nama=" + "Made Widi" + "&waktu=" + asString + "&tanggal=" + currentDate;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
/////////////////////////////////////////////////////////////////////////
        
      }
/////////////////////////////////////////////////////
else if (rfid.uid.uidByte[0] == keyTagUID3[0] &&
          rfid.uid.uidByte[1] == keyTagUID3[1] &&
          rfid.uid.uidByte[2] == keyTagUID3[2] &&
          rfid.uid.uidByte[3] == keyTagUID3[3] &&
          rfid.uid.uidByte[4] == keyTagUID3[4] &&
          rfid.uid.uidByte[5] == keyTagUID3[5] &&
          rfid.uid.uidByte[6] == keyTagUID3[6] ) {
        Serial.println("Access is granted to abdul maulana");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("abdul maulana");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "abdul maulana";
        kondisi = 0;
        Blynk.run();
        timer.run();

/////////////////////////////////////////////////////////////////////
if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
//    String currentTime = String(hour()) + ":" + minute() + ":" + second();
//    String currentDate = String(day()) + " " + month() + " " + year();
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"nama=" + "Abdul Maulana" + "&waktu=" + asString + "&tanggal=" + currentDate;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
/////////////////////////////////////////////////////////////////////////
        
      }
////////////////////////////////////////////
else if (rfid.uid.uidByte[0] == keyTagUID4[0] &&
          rfid.uid.uidByte[1] == keyTagUID4[1] &&
          rfid.uid.uidByte[2] == keyTagUID4[2] &&
          rfid.uid.uidByte[3] == keyTagUID4[3] &&
          rfid.uid.uidByte[4] == keyTagUID4[4] &&
          rfid.uid.uidByte[5] == keyTagUID4[5] &&
          rfid.uid.uidByte[6] == keyTagUID4[6] ) {
        Serial.println("Access is granted to adnan abdillah");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("adnan abdillah");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "adnan abdillah";
        kondisi = 0;
        Blynk.run();
        timer.run();

/////////////////////////////////////////////////////////////////////
if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
//    String currentTime = String(hour()) + ":" + minute() + ":" + second();
//    String currentDate = String(day()) + " " + month() + " " + year();
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"nama=" + "Adnan Abdillah" + "&waktu=" + asString + "&tanggal=" + currentDate;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
/////////////////////////////////////////////////////////////////////////
        
      }
////////////////////////////////////////
else if (rfid.uid.uidByte[0] == keyTagUID5[0] &&
          rfid.uid.uidByte[1] == keyTagUID5[1] &&
          rfid.uid.uidByte[2] == keyTagUID5[2] &&
          rfid.uid.uidByte[3] == keyTagUID5[3] &&
          rfid.uid.uidByte[4] == keyTagUID5[4] &&
          rfid.uid.uidByte[5] == keyTagUID5[5] &&
          rfid.uid.uidByte[6] == keyTagUID5[6] ) {
        Serial.println("Access is granted to lukman");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("lukman");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "lukman";
        kondisi = 0;
        Blynk.run();
        timer.run();

/////////////////////////////////////////////////////////////////////
if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
//    String currentTime = String(hour()) + ":" + minute() + ":" + second();
//    String currentDate = String(day()) + " " + month() + " " + year();
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"nama=" + "Lukman" + "&waktu=" + asString + "&tanggal=" + currentDate;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
/////////////////////////////////////////////////////////////////////////
        
      }
//////////////////////////////////////////////////
else if (rfid.uid.uidByte[0] == keyTagUID6[0] &&
          rfid.uid.uidByte[1] == keyTagUID6[1] &&
          rfid.uid.uidByte[2] == keyTagUID6[2] &&
          rfid.uid.uidByte[3] == keyTagUID6[3] &&
          rfid.uid.uidByte[4] == keyTagUID6[4] &&
          rfid.uid.uidByte[5] == keyTagUID6[5] &&
          rfid.uid.uidByte[6] == keyTagUID6[6] ) {
        Serial.println("Access is granted to firman");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("firman");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "firman";
        kondisi = 0;
        Blynk.run();
        timer.run();

/////////////////////////////////////////////////////////////////////
if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
//    String currentTime = String(hour()) + ":" + minute() + ":" + second();
//    String currentDate = String(day()) + " " + month() + " " + year();
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"nama=" + "Firman" + "&waktu=" + asString + "&tanggal=" + currentDate;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
/////////////////////////////////////////////////////////////////////////
        
      }
//////////////////////////////////////////////////////////
else if (rfid.uid.uidByte[0] == keyTagUID7[0] &&
          rfid.uid.uidByte[1] == keyTagUID7[1] &&
          rfid.uid.uidByte[2] == keyTagUID7[2] &&
          rfid.uid.uidByte[3] == keyTagUID7[3] &&
          rfid.uid.uidByte[4] == keyTagUID7[4] &&
          rfid.uid.uidByte[5] == keyTagUID7[5] &&
          rfid.uid.uidByte[6] == keyTagUID7[6] ) {
        Serial.println("Access is granted to i made rupawan");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("i made rupawan");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "i made rupawan";
        kondisi = 0;
        Blynk.run();
        timer.run();

/////////////////////////////////////////////////////////////////////
if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
//    String currentTime = String(hour()) + ":" + minute() + ":" + second();
//    String currentDate = String(day()) + " " + month() + " " + year();
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"nama=" + "I Made Rupawan" + "&waktu=" + asString + "&tanggal=" + currentDate;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
/////////////////////////////////////////////////////////////////////////
        
      }
/////////////////////////////////////////////////////
else if (rfid.uid.uidByte[0] == keyTagUID8[0] &&
          rfid.uid.uidByte[1] == keyTagUID8[1] &&
          rfid.uid.uidByte[2] == keyTagUID8[2] &&
          rfid.uid.uidByte[3] == keyTagUID8[3] &&
          rfid.uid.uidByte[4] == keyTagUID8[4] &&
          rfid.uid.uidByte[5] == keyTagUID8[5] &&
          rfid.uid.uidByte[6] == keyTagUID8[6] ) {
        Serial.println("Access is granted to jaka bagus a.");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("jaka bagus a.");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "jaka bagus a.";
        kondisi = 0;
        Blynk.run();
        timer.run();

/////////////////////////////////////////////////////////////////////
if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
//    String currentTime = String(hour()) + ":" + minute() + ":" + second();
//    String currentDate = String(day()) + " " + month() + " " + year();
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"nama=" + "Jaka Bagus" + "&waktu=" + asString + "&tanggal=" + currentDate;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
/////////////////////////////////////////////////////////////////////////
        
      }
//////////////////////////////////////////////////
else if (rfid.uid.uidByte[0] == keyTagUID9[0] &&
          rfid.uid.uidByte[1] == keyTagUID9[1] &&
          rfid.uid.uidByte[2] == keyTagUID9[2] &&
          rfid.uid.uidByte[3] == keyTagUID9[3] &&
          rfid.uid.uidByte[4] == keyTagUID9[4] &&
          rfid.uid.uidByte[5] == keyTagUID9[5] &&
          rfid.uid.uidByte[6] == keyTagUID9[6] ) {
        Serial.println("Access is granted to aprilia");
        lcd.clear();
        lcd.print("Akses diterima");
        lcd.setCursor(0,1);
        lcd.print("aprilia");
        delay(3000);
        lcd.clear();
        lcd.print("Tekan A");
        masuk = "aprilia";
        kondisi = 0;
        Blynk.run();
        timer.run();

/////////////////////////////////////////////////////////////////////
if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
//    String currentTime = String(hour()) + ":" + minute() + ":" + second();
//    String currentDate = String(day()) + " " + month() + " " + year();
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"nama=" + "Aprilia" + "&waktu=" + asString + "&tanggal=" + currentDate;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
/////////////////////////////////////////////////////////////////////////
        
      }
         
      else
      {
        Serial.print("Access denied, UID:");
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Tidak ada akses");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
      }

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
      
    }
  } 
  }
  else{
//    Serial.println(kondisi);
    char key = keypad.getKey();

  if (key) {
    Serial.println(key);
    if (key == 'A') {
      lcd.clear();
      lcd.print("Password :");
      input_password = ""; // clear input password
    } else if (key == 'B') {
      if (password == input_password) {
        Serial.println("The password is correct, ACCESS GRANTED!");
        lcd.clear();
        lcd.print("Silahkan Masuk");
        // DO YOUR WORK HERE
        digitalWrite(RELAY,HIGH);
        digitalWrite(green,HIGH);
        delay(3000);
        digitalWrite(RELAY,LOW);
        digitalWrite(green,LOW);
        kondisi=1;
        lcd.clear();
      } else {
        Serial.println("The password is incorrect, ACCESS DENIED!");
        lcd.clear();
        lcd.print("Pin Salah");
        digitalWrite(red,HIGH);
        digitalWrite(Buzzer,HIGH);
        delay(3000);
        digitalWrite(red,LOW);
        digitalWrite(Buzzer,LOW);
        kondisi=1;
      }

      input_password = ""; // clear input password
    } else {
      if (input_password.length() == 0) {
//        lcd.clear();
      }
      input_password += key; // append new character to input password string
      lcd.setCursor(input_password.length(), 1); // move cursor to new position
      lcd.print("*");
    }
  }
}
}
