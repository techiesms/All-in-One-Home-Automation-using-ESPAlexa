/*
 * 
 * This is the code for the project, 
 * Controlling UNLIMITED Appliances using ALEXA for FREE 
 * YouTube Video Link - https://youtu.be/sB8JhWnzkTU
 * 
 * 
 * This code is tried and tested with 
 * - Arduino IDE version 1.8.19
 * - ESP32 board packages version 2.0.1, 
 * - ESPAlexa Library version 2.7.0,
 * - AceButton Library version 1.9.1
 * 
 * In case the code doesn't work, try upgrading/downgrading the verison of above mentioned Libraries.
 * 
 */
 
#define DEBUG_SW 1

// libraries
#include <WiFi.h>
#include <Espalexa.h> // https://github.com/Aircoookie/Espalexa
#include <AceButton.h> // https://github.com/bxparks/AceButton

using namespace ace_button;

Espalexa espalexa;

//  Pins of Relay (Appliance Control)
#define R1 15
#define R2 2
#define R3 4
#define R4 22

// Pins of Relay (Fan Speed Control)
#define Speed1 21
#define Speed2 19
#define Speed4 18

// Pins of Switches
#define S1 32
#define S2 35
#define S3 34
#define S4 39

// Pins of Fan Regulator Knob
#define F1  27
#define F2  14
#define F3  12
#define F4  13

// Flags for Fan Regulator Knob
bool speed0_flag = 1;
bool speed1_flag = 1;
bool speed2_flag = 1;
bool speed3_flag = 1;
bool speed4_flag = 1;

// WiFi Credentials
const char* ssid = "SmS_jiofi";
const char* password = "sms123458956";

// device names
String Appliance1 = "Light 1";
String Appliance2 = "Light 2";
String Appliance3 = "Light 3";
String Appliance4 = "Light 4";
String Fan = "Fan";


boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);
void fanChanged(uint8_t brightness);


ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);

boolean wifiConnected = false;

//our callback functions
void firstLightChanged(uint8_t brightness)
{

  if (brightness == 255)
  {
    digitalWrite(R1, HIGH);
    Serial.println("Device1 ON");
  }
  else
  {
    digitalWrite(R1, LOW);
    Serial.println("Device1 OFF");
  }
}

void secondLightChanged(uint8_t brightness)
{

  if (brightness == 255)
  {
    digitalWrite(R2, HIGH);
    Serial.println("Device2 ON");
  }
  else
  {
    digitalWrite(R2, LOW);
    Serial.println("Device2 OFF");
  }
}

void thirdLightChanged(uint8_t brightness)
{

  if (brightness == 255)
  {
    digitalWrite(R3, HIGH);
    Serial.println("Device3 ON");
  }
  else
  {
    digitalWrite(R3, LOW);
    Serial.println("Device3 OFF");
  }
}

void fourthLightChanged(uint8_t brightness)
{
  if (brightness == 255)
  {
    digitalWrite(R4, HIGH);
    Serial.println("Device4 ON");
  }
  else
  {
    digitalWrite(R4, LOW);
    Serial.println("Device4 OFF");
  }
}

void fanChanged(uint8_t brightness)
{
  int percent = map(brightness, 0, 255, 0, 4);

  if (percent == 0)
    speed0();
  if (percent == 1)
    speed1();
  if (percent == 2)
    speed2();
  if (percent == 3)
    speed3();
  if (percent == 4)
    speed4();

}

// connect to wifi – returns true if successful or false if not
boolean connectWifi()
{
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}

void ESPAlexa_Devices()
{
  // Define your devices here.
  espalexa.addDevice(Appliance1, firstLightChanged);
  espalexa.addDevice(Appliance2, secondLightChanged);
  espalexa.addDevice(Appliance3, thirdLightChanged);
  espalexa.addDevice(Appliance4, fourthLightChanged);
  espalexa.addDevice(Fan, fanChanged);

  espalexa.begin();
}

void setup()
{
  Serial.begin(115200);

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW);

  pinMode(Speed1, OUTPUT);
  pinMode(Speed2, OUTPUT);
  pinMode(Speed4, OUTPUT);

  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S4, INPUT_PULLUP);

  pinMode(F1, INPUT_PULLUP);
  pinMode(F2, INPUT_PULLUP);
  pinMode(F3, INPUT_PULLUP);
  pinMode(F4, INPUT_PULLUP);


  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);


  button1.init(S1);
  button2.init(S2);
  button3.init(S3);
  button4.init(S4);



  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected)
  {
    ESPAlexa_Devices();
  }
  else
  {
    Serial.println("wifi not connected!!!");
    delay(1000);
  }
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    if (DEBUG_SW)Serial.print("Not Connected ");
  }
  else
  {
    if (DEBUG_SW)Serial.print("Connected  ");
    if (wifiConnected)
    {
      espalexa.loop();
      delay(1);
    }
    else
    {
      wifiConnected = connectWifi();
      if (wifiConnected)
      {
        ESPAlexa_Devices();
      }
    }
  }

  button1.check();
  button2.check();
  button3.check();
  button4.check();
  fan_switch_check();

}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  EspalexaDevice* d1 = espalexa.getDevice(0);
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      d1->setPercent(100); //This is used for feedback
      digitalWrite(R1, HIGH);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      d1->setPercent(0);
      digitalWrite(R1, LOW);
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  EspalexaDevice* d2 = espalexa.getDevice(1);
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      d2->setPercent(100);
      digitalWrite(R2, HIGH);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      d2->setPercent(0);
      digitalWrite(R2, LOW);
      break;
  }
}

void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  EspalexaDevice* d3 = espalexa.getDevice(2);
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      d3->setPercent(100);
      digitalWrite(R3, HIGH);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      d3->setPercent(0);
      digitalWrite(R3, LOW);
      break;
  }
}

void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  EspalexaDevice* d4 = espalexa.getDevice(3);
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      d4->setPercent(100);
      digitalWrite(R4, HIGH);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      d4->setPercent(0);
      digitalWrite(R4, LOW);
      break;
  }
}

void fan_switch_check()
{
  if (digitalRead(F1) == LOW && speed1_flag == 1)
  {
    EspalexaDevice* d5 = espalexa.getDevice(4);
    d5->setPercent(25);
    speed1();
    speed1_flag = 0;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;


  }
  if (digitalRead(F2) == LOW && digitalRead(F3) == HIGH && speed2_flag == 1)
  {
    EspalexaDevice* d5 = espalexa.getDevice(4);
    d5->setPercent(50);
    speed2();
    speed1_flag = 1;
    speed2_flag = 0;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;

  }
  if (digitalRead(F2) == LOW && digitalRead(F3) == LOW && speed3_flag == 1)
  {
    EspalexaDevice* d5 = espalexa.getDevice(4);
    d5->setPercent(75);
    speed3();
    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 0;
    speed4_flag = 1;
    speed0_flag = 1;
  }
  if (digitalRead(F4) == LOW  && speed4_flag == 1)
  {
    EspalexaDevice* d5 = espalexa.getDevice(4);
    d5->setPercent(100);
    speed4();
    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 0;
    speed0_flag = 1;
  }
  if (digitalRead(F1) == HIGH && digitalRead(F2) == HIGH && digitalRead(F3) == HIGH && digitalRead(F4) == HIGH  && speed0_flag == 1)
  {
    EspalexaDevice* d5 = espalexa.getDevice(4);
    d5->setPercent(0);
    speed0();
    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 0;
  }
}

// Fan Speed Control

void speed0()
{
  //All Relays Off - Fan at speed 0
  if (DEBUG_SW)Serial.println("SPEED 0");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);

}

void speed1()
{
  //Speed1 Relay On - Fan at speed 1
  if (DEBUG_SW)Serial.println("SPEED 1");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed1, HIGH);
}

void speed2()
{
  //Speed2 Relay On - Fan at speed 2
  if (DEBUG_SW)Serial.println("SPEED 2");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed2, HIGH);
}

void speed3()
{
  //Speed1 & Speed2 Relays On - Fan at speed 3
  if (DEBUG_SW)Serial.println("SPEED 3");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed1, HIGH);
  digitalWrite(Speed2, HIGH);

}

void speed4()
{
  //Speed4 Relay On - Fan at speed 4
  if (DEBUG_SW)Serial.println("SPEED 4");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed4, HIGH);
}
