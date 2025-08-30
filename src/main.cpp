#include <Arduino.h>
#include <FastLED.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>

#include "WebPage.h"

#include "NVMData.h"
#include "DynamicData.h"

WebPage webPage;

#define PIN_WS2812B 2
#define NUM_PIXELS 144

//too many variables but they are needed ig...
CRGB leds[NUM_PIXELS];
const int number = 143;
int counter = 20;
int scndcounter = 80;
int thrdcounter = 120;
int fourcounter = 40;
int sevencounter = 60;
int countereight = 100;
const int c11 = 20;
const int c12 = 80;
const int c13 = 120;
const int c14 = 40;
const int c17 = 60;
const int c18 = 100;
int randomNum = 0;
int dirtybit = 142;
int endcounter = 0;
int secondEndTimer = 0;
int oldScene = 0;
bool counterended = true;
bool counterhasended = false;
bool counterTwoEnd = false;
bool phase1 = false;
bool just_started = true;

//defined Time and Delays
static const int delayTime_ms = 5;
static const int SECONDS_1 = 200;
static const int SECONDS_10 = 2000;

void checkNetworkSet() {
  if(DynamicData::get().setNewNetwork == true)
  {
    if(webPage.newNetworkSet == true)
    {
      DynamicData::get().setNewNetwork = false;
      webPage.newNetworkSet = false;
      NVMData::get().StoreNetData();
    }
  }
  else
  {
    DynamicData::get().setNewNetwork = false;
  }
}

void setupWiFi() {
  String hostname = "LightStripAaron";
  if (NVMData::get().NetDataValid() == false) {
    DynamicData::get().setNewNetwork = true;
    Serial.println(WiFi.macAddress());
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hostname.c_str(), "123Power");
    delay(1000);
    Serial.println(WiFi.softAPIP());
    DynamicData::get().ipaddress = WiFi.softAPIP().toString();
  }
  else {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname.c_str());
    Serial.printf("SSID = %s\n", NVMData::get().GetNetName().c_str());
    WiFi.begin(NVMData::get().GetNetName().c_str(), NVMData::get().GetNetPassword().c_str());
    Serial.println(WiFi.macAddress());
    int maxWaitForNet = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(delayTime_ms);
      if (maxWaitForNet < SECONDS_10) {
        maxWaitForNet++;
      }
      else {
        Serial.print(".");
        WiFi.disconnect();
        WiFi.begin(NVMData::get().GetNetName().c_str(), NVMData::get().GetNetPassword().c_str());
        DynamicData::get().incErrorCounter("Wifi startup");
        maxWaitForNet = 0;
      }
    }
    Serial.println("");
    Serial.println("connected");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.dnsIP());
    DynamicData::get().ipaddress = WiFi.localIP().toString();
  }
}

void setup() {
  FastLED.addLeds<NEOPIXEL, PIN_WS2812B>(leds, NUM_PIXELS);
  Serial.begin(115200);

  //getting dynamic data from the Website (the setup)
  NVMData::get().Init();
  DynamicData::get().Init();
  Serial.println("NetName: "+  NVMData::get().GetNetName());
  setupWiFi();
  webPage.Init();
}

void twoPhaseLights() {
  if(phase1&&counterended){
    fill_solid(leds, NUM_PIXELS/2+NUM_PIXELS/2, CRGB::Red);
    fill_solid(leds, NUM_PIXELS/2, CRGB::Blue);
    phase1 = false;
    counterended = false;
    counterhasended = false;
  }
  
  else if(!phase1&&counterended){
    fill_solid(leds, NUM_PIXELS/2+NUM_PIXELS/2, CRGB::Blue);
    fill_solid(leds, NUM_PIXELS/2, CRGB::Red);
    phase1 = true;
    counterended = false;
    counterhasended = false;
  }

  if(counterhasended){
    fill_solid(leds, NUM_PIXELS, CRGB::Black);
    counterhasended = false;
  }
  FastLED.setBrightness(DynamicData::get().brightness);
  FastLED.show(); 
  endcounter++;

  if(endcounter == 249) {
    counterhasended = true;
  }

  if(endcounter >=250) {
    counterended = true;
    endcounter = 0;
  }
}

void lightStripTrafficAdvisor() {

  if(endcounter == 249) {
    counterhasended = true;
  }

  if(endcounter >=250) {
    counterended = true;
    endcounter = 0;
  }
  if(counter < 2) {
    leds[number] = CRGB::Black;
  }

  else{
    leds[counter-2] = CRGB::Black;
  }

  if(sevencounter < 2) {
    leds[number] = CRGB::Black;
  }

  else{
    leds[sevencounter-2] = CRGB::Black;
  }

  if(scndcounter < 2){
    leds[number] = CRGB::Black;
  }

  else{
    leds[scndcounter-2] = CRGB::Black;
  }

  if(thrdcounter < 2){
    leds[number] = CRGB::Black;
  }

  else{
    leds[thrdcounter-2] = CRGB::Black;
  }

  if(fourcounter < 2){
    leds[number] = CRGB::Black;
  }

  else{
    leds[fourcounter-2] = CRGB::Black;
  }

  if(counterended) {
    leds[counter] = CRGB::OrangeRed;
    leds[scndcounter] = CRGB::OrangeRed;
    leds[counter-1] = CRGB::OrangeRed;
    leds[scndcounter-1] = CRGB::OrangeRed;
    leds[thrdcounter] = CRGB::OrangeRed;
    leds[fourcounter] = CRGB::OrangeRed;
    leds[sevencounter] = CRGB::OrangeRed;
    leds[sevencounter-1] = CRGB::OrangeRed;
    FastLED.setBrightness(DynamicData::get().brightness);
    counterended= false;
  }


  FastLED.show();
  endcounter++;
  if(counterhasended) {
    counter++;
    scndcounter++;
    thrdcounter++;
    fourcounter++;
    sevencounter++;
    counterhasended = false;
	}

  if(fourcounter >= 143){
    leds[fourcounter-1] = CRGB::Black;
    fourcounter = 1;
    leds[dirtybit] = CRGB::Black;
  }

  if(sevencounter >= 143){
    leds[sevencounter-1] = CRGB::Black;
    sevencounter = 1;
    leds[dirtybit] = CRGB::Black;
  }

  if(scndcounter >= 143){
    leds[scndcounter-1] = CRGB::Black;
    scndcounter = 1;
    leds[dirtybit] = CRGB::Black;
  }

  if(thrdcounter >= 143){
    leds[thrdcounter-1] = CRGB::Black;
    thrdcounter = 1;
    leds[dirtybit] = CRGB::Black;
  }

  if(counter >= 143){
    leds[counter-1] = CRGB::Black;
    counter = 1;
    leds[dirtybit] = CRGB::Black;
  } 
}

void warpCoreMode() {
  leds[counter-1] = CRGB::Black;
  leds[counter] = CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue);
  leds[sevencounter-1] = CRGB::Black;
  leds[sevencounter] = CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue);
  leds[thrdcounter-1] = CRGB::Black;
  leds[thrdcounter] = CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue);
  leds[fourcounter-1] = CRGB::Black;
  leds[fourcounter] = CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue);

  FastLED.setBrightness(DynamicData::get().brightness);
  
  FastLED.show();
  if(counterended) {
    counter++;
    sevencounter++;
    thrdcounter++;
    fourcounter++;
    counterended = false;
  }

  endcounter++;
  if(endcounter >= DynamicData::get().waittime) {
    counterended = true;
    endcounter = 0;
  }

  if(counter > 143) {
    leds[counter-1] = CRGB::Black;
    counter = 1;
  }

  if(sevencounter > 143) {
    leds[sevencounter-1] = CRGB::Black;
    sevencounter = 1;
  }
  if(thrdcounter > 143) {
    leds[thrdcounter-1] = CRGB::Black;
    thrdcounter = 1;
  }

  if(fourcounter > 143) {
    leds[fourcounter-1] = CRGB::Black;
    fourcounter = 1;
  }
}

void twoPhaseLightCusomColor() {
  if(phase1&&counterended){
    leds[c12-5] = leds[c14-5] = leds[c18-5] = CRGB::Black;
    leds[c11-5] = leds[c13-5] = leds[c17-5] = CRGB(DynamicData::get().red,DynamicData::get().green,DynamicData::get().blue);
    FastLED.setBrightness(DynamicData::get().brightness);
    phase1 = false;
    counterended = false;
    counterhasended = false;
  }
  
  else if(!phase1&&counterended){
    leds[c11] =  leds[c13]  = leds[c17] =  CRGB(DynamicData::get().red,DynamicData::get().green,DynamicData::get().blue);
    FastLED.setBrightness(DynamicData::get().brightness);
    leds[c12] = leds[c14] = leds[c18] = CRGB::Black;
    phase1 = true;
    counterended = false;
    counterhasended = false;
  }

  if(counterhasended){
    leds[c11] = leds[c12] = leds[c13] = leds[c14] = leds[c17] = leds[c18]  = CRGB::Black;
    leds[c11-5] = leds[c12-5] = leds[c13-5] = leds[c14-5] = leds[c17-5] = leds[c18-5]  = CRGB::Black;
  }

  FastLED.show(); 
  endcounter++;

  if(endcounter == 249) {
    counterhasended = true;
  }

  if(endcounter >=250) {
    counterended = true;
    endcounter = 0;
  }
}

void offline() {
  fill_solid(leds, NUM_PIXELS, CRGB::Black);
  FastLED.show();
}

void RandomFlashingLights() {
  if (counterhasended && !counterended) {
    randomNum = rand() % NUM_PIXELS;
    leds[randomNum] = CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue);
    counterhasended = false;
    counterended = true;
    FastLED.setBrightness(DynamicData::get().brightness);
    FastLED.show();
  } 
  else if (counterended && counterhasended) {
    leds[randomNum] = CRGB::Black;
    counterended = false;
    counterhasended = false;
    FastLED.show();
  }

  secondEndTimer++;

  if (secondEndTimer >= DynamicData::get().waittime) {
    counterhasended = true;
    secondEndTimer = 0;
  }
  
}

void allOn() {
  fill_solid(leds, NUM_PIXELS, CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue));
  FastLED.setBrightness(DynamicData::get().brightness);
  FastLED.show();
}

void FullOnBlueBlackFlasher() {
  if (counterhasended && !counterended) {
    fill_solid(leds, NUM_PIXELS/2, CRGB::Black);
    fill_solid(leds+NUM_PIXELS/2, NUM_PIXELS/2, CRGB::Blue);
    FastLED.setBrightness(DynamicData::get().brightness);
    FastLED.show();
    counterended = true;
    counterhasended = false;
  } 
  else if (counterended && counterhasended) {
    fill_solid(leds, NUM_PIXELS/2, CRGB::Blue);
    fill_solid(leds+NUM_PIXELS/2, NUM_PIXELS/2, CRGB::Black);
    FastLED.setBrightness(DynamicData::get().brightness);
    counterended = false;
    counterhasended = false;
    FastLED.show();
  }

  secondEndTimer++;

  if (secondEndTimer >= DynamicData::get().waittime) {
    counterhasended = true;
    secondEndTimer = 0;
  }
}

void FullOnCustomBlackFlasher() {
  if (counterhasended && !counterended) {
    fill_solid(leds, NUM_PIXELS/2, CRGB::Black);
    fill_solid(leds+NUM_PIXELS/2, NUM_PIXELS/2, CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue));
    FastLED.setBrightness(DynamicData::get().brightness);
    FastLED.show();
    counterended = true;
    counterhasended = false;
  } 
  else if (counterended && counterhasended) {
    fill_solid(leds, NUM_PIXELS/2, CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue));
    fill_solid(leds+NUM_PIXELS/2, NUM_PIXELS/2, CRGB::Black);
    FastLED.setBrightness(DynamicData::get().brightness);
    counterended = false;
    counterhasended = false;
    FastLED.show();
  }

  secondEndTimer++;

  if (secondEndTimer >= DynamicData::get().waittime) {
    counterhasended = true;
    secondEndTimer = 0;
  }
}

void strobo(){
  if (counterhasended && !counterended) {
    fill_solid(leds, NUM_PIXELS, CRGB::White);
    FastLED.setBrightness(DynamicData::get().brightness);
    counterhasended = false;
    counterended = true;
    FastLED.show();
  } 
  else if (counterended && counterhasended) {
    fill_solid(leds, NUM_PIXELS, CRGB::Black);
    counterended = false;
    counterhasended = false;
    FastLED.show();
  }
  endcounter++;
  if (secondEndTimer >= 2) {
    counterhasended = true;
    secondEndTimer = 0;
  }
  if (endcounter >=DynamicData::get().waittime){
    secondEndTimer++;
    endcounter=0;
  }
}

void IndicatorRight(){
    if(just_started){
    phase1 = true;
  }
  if(phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::OrangeRed);
  fill_solid(leds, NUM_PIXELS-NUM_PIXELS/6, CRGB::Black);
    phase1 = false;
    counterended = false;
  }
  
  else if(!phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::Black);
    phase1 = true;
    counterended = false;
  }

  FastLED.setBrightness(150);
  FastLED.show(); 
  endcounter++;

  if(endcounter >= 175) {
    counterended = true;
    endcounter = 0;
  }
}

void IndicatorLeft(){
  if(just_started){
    phase1 = true;
  }
  if(phase1&&counterended){
    fill_solid(leds, NUM_PIXELS/6, CRGB::OrangeRed);
    phase1 = false;
    counterended = false;
  }
  
  else if(!phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::Black);
    phase1 = true;
    counterended = false;
  }

  FastLED.setBrightness(150);
  FastLED.show(); 
  endcounter++;

  if(endcounter >= 175) {
    counterended = true;
    endcounter = 0;
  }
}

void IndicatorRightHeadlight(){
    if(just_started){
    phase1 = true;
  }
  if(phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::OrangeRed);
  fill_solid(leds, NUM_PIXELS-NUM_PIXELS/6, CRGB::White);
    phase1 = false;
    counterended = false;
  }
  
  else if(!phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::White);
    phase1 = true;
    counterended = false;
  }

  FastLED.setBrightness(150);
  FastLED.show(); 
  endcounter++;

  if(endcounter >= 175) {
    counterended = true;
    endcounter = 0;
  }
}

void IndicatorLeftHeadlight(){
    if(just_started){
    phase1 = true;
  }
  if(phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::White);
    fill_solid(leds, NUM_PIXELS/6, CRGB::OrangeRed);
    phase1 = false;
    counterended = false;
  }
  
  else if(!phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::White);
    phase1 = true;
    counterended = false;
  }

  FastLED.setBrightness(150);
  FastLED.show(); 
  endcounter++;

  if(endcounter >= 175) {
    counterended = true;
    endcounter = 0;
  }
}

void HazardHeadLight(){
    if(just_started){
    phase1 = true;
  }
  if(phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::OrangeRed);
    fill_solid(leds, NUM_PIXELS-NUM_PIXELS/6, CRGB::White);
    fill_solid(leds, NUM_PIXELS/6, CRGB::OrangeRed);
    phase1 = false;
    counterended = false;
  }
  
  else if(!phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::White);
    phase1 = true;
    counterended = false;
  }

  FastLED.setBrightness(150);
  FastLED.show(); 
  endcounter++;

  if(endcounter >= 175) {
    counterended = true;
    endcounter = 0;
  }
}

void HazardLight(){
    if(just_started){
    phase1 = true;
  }
  if(phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::OrangeRed);
    fill_solid(leds, NUM_PIXELS-NUM_PIXELS/6, CRGB::Black);
    fill_solid(leds, NUM_PIXELS/6, CRGB::OrangeRed);
    phase1 = false;
    counterended = false;
  }
  
  else if(!phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::Black);
    phase1 = true;
    counterended = false;
  }

  FastLED.setBrightness(150);
  FastLED.show(); 
  endcounter++;

  if(endcounter >= 175) {
    counterended = true;
    endcounter = 0;
  }
}

void HazardLightFast(){
    if(just_started){
    phase1 = true;
  }
  if(phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::OrangeRed);
    fill_solid(leds, NUM_PIXELS-NUM_PIXELS/6, CRGB::Black);
    fill_solid(leds, NUM_PIXELS/6, CRGB::OrangeRed);
    phase1 = false;
    counterended = false;
  }
  
  else if(!phase1&&counterended){
    fill_solid(leds, NUM_PIXELS, CRGB::Black);
    phase1 = true;
    counterended = false;
  }

  FastLED.setBrightness(150);
  FastLED.show(); 
  endcounter++;

  if(endcounter >= 30) {
    counterended = true;
    endcounter = 0;
  }
}

void HeadlightNormal(){
  fill_solid(leds, NUM_PIXELS, CRGB::White);
  FastLED.setBrightness(150);
  FastLED.show();
}

void FarlightsHeadLight(){
  fill_solid(leds, NUM_PIXELS, CRGB::White);
  FastLED.setBrightness(255);
  FastLED.show();
}

/* http://192.168.54.147/change?scene=0&red=0&green=0&blue=0&waittime=2&brightness=150 Ref. in README.md && scenes esp32.txt */
void loop() {
  checkNetworkSet();
  webPage.loop();
  if(oldScene != DynamicData::get().scene) {
    offline();
    just_started = true;
    
  }
  oldScene = DynamicData::get().scene;
  switch (DynamicData::get().scene)
  {


  case 0:
    lightStripTrafficAdvisor();
    break;

  case 1:
    twoPhaseLights();
    break;

  case 2:
    warpCoreMode();
    break;

  case 3:
    twoPhaseLightCusomColor();
    break;

  case 4: 
    RandomFlashingLights();
    break;
  
  case 5:
    allOn();
    break;
  
  case 6:
    FullOnBlueBlackFlasher();
    break;

  case 7:
    FullOnCustomBlackFlasher();
    break;
  
  case 8:
    strobo();
    break;

  case 9:
    offline();
    break;


  case 10:
    IndicatorRight();
    break;
  
  case 11:
    IndicatorLeft();
    break;

  case 12:
    HazardLight();
    break;

  case 13:
    IndicatorRightHeadlight();
    break;

  case 14:
    IndicatorLeftHeadlight();
    break;

  case 15:
    HazardHeadLight();
    break;

  case 16:
    HeadlightNormal();
    break;

  case 17:
    FarlightsHeadLight();
    break;
  
  case 18:
    HazardLightFast();
    break;

  default:
    offline();
    break;
  
  }

  delay(1);
    
}
