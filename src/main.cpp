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
int dirtybit = 142;
int color1R = 0;
int color1G = 0;
int color1B = 255;
int color2R = 255;
int color2G = 0;
int color2B = 0;
int endcounter = 0;
int secondEndTimer = 0;
int oldScene = 0;
bool counterended = true;
bool counterhasended = false;
bool counterTwoEnd = false;
bool phase1 = false;

//defined smth with these var.
static const int delayTime_ms = 5;
static const int SECONDS_1 = 200;

void checkNetworkSet()
{
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

void setupWiFi()
{
  String hostname = "LightStripAaron";
  if (NVMData::get().NetDataValid() == false)
  {
    DynamicData::get().setNewNetwork = true;
    Serial.println(WiFi.macAddress());
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hostname.c_str(), "123Power");
    delay(1000);
    Serial.println(WiFi.softAPIP());
    DynamicData::get().ipaddress = WiFi.softAPIP().toString();
  }
  else
  {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname.c_str());
    Serial.printf("SSID = %s\n", NVMData::get().GetNetName().c_str());
    WiFi.begin(NVMData::get().GetNetName().c_str(), NVMData::get().GetNetPassword().c_str());
    Serial.println(WiFi.macAddress());
    int maxWaitForNet = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(delayTime_ms);
      if (maxWaitForNet < SECONDS_1)
      {
        maxWaitForNet++;
      }
      else
      {
        Serial.print(".");
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

  //NVM data or like that as well as webpage smth
  NVMData::get().Init();
  DynamicData::get().Init();
  Serial.println("NetName: "+  NVMData::get().GetNetName());
  setupWiFi();
  webPage.Init();
}

void twoPahseLights() {
  if(phase1&&counterended){
    leds[c12-5] = leds[c14-5] = leds[c18-5] = CRGB(color1R, color1G, color1B);
    leds[c11-5] = leds[c13-5] = leds[c17-5] = CRGB(color2R,color2G,color2B);
    phase1 = false;
    counterended = false;
    counterhasended = false;
  }
  
  else if(!phase1&&counterended){
    leds[c11] =  leds[c13]  = leds[c17] =  CRGB(color1R, color1G, color1B);
    leds[c12] = leds[c14] = leds[c18] = CRGB(color2R,color2G,color2B);
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

void trafficAdvisorWithFLashers() {
  if(counter < 2) {
    leds[number] = CRGB::Black;
  }

  else {
    leds[counter-2] = CRGB::Black;
  }

  if(sevencounter < 2) {
    leds[number] = CRGB::Black;
  }

  else {
    leds[sevencounter-2] = CRGB::Black;
  }

  if(scndcounter < 2) {
    leds[number] = CRGB::Black;
  }

  else {
    leds[scndcounter-2] = CRGB::Black;
  }

  if(thrdcounter < 2) {
    leds[number] = CRGB::Black;
  }

  else {
    leds[thrdcounter-2] = CRGB::Black;
  }

  if(fourcounter < 2) {
    leds[number] = CRGB::Black;
  }

  else {
    leds[fourcounter-2] = CRGB::Black;
  }
  
  if(counterTwoEnd) {
    counter++;
    scndcounter++;
    thrdcounter++;
    fourcounter++;
    sevencounter++;
  }

  if(counterTwoEnd) {
    leds[counter] = CRGB::OrangeRed;
    leds[scndcounter] = CRGB::OrangeRed;
    leds[counter-1] = CRGB::OrangeRed;
    leds[scndcounter-1] = CRGB::OrangeRed;
    leds[thrdcounter] = CRGB::OrangeRed;
    leds[fourcounter] = CRGB::OrangeRed;
    leds[sevencounter] = CRGB::OrangeRed;
    leds[sevencounter-1] = CRGB::OrangeRed;
    counterTwoEnd = false;
  }

  if(phase1) {
    if(!counterended) {
      leds[c11-5] = leds[c12-5] = leds[c18-5] = CRGB(color1R,color1G,color1B);
      leds[c14-5] = leds[c13-5] = leds[c17-5] =  CRGB(color2R,color2G,color2B);
    }
    if(counterended) {
      phase1 = false;
      counterended = false;
    }
  }

  else if(!phase1) {
    if(!counterended) {
       leds[c13] =  leds[c17] = leds[c14] =  CRGB(color1R,color1G,color1B);
      leds[c11] = leds[c12] = leds[c18] = CRGB(color2R,color2G,color2B);
    }
    if(counterended) {
      phase1 = true;
      counterended = false;
    }
  }

  if(counterhasended){
    leds[c11] = leds[c12] = leds[c13] = leds[c14] = leds[c17] = leds[c18]  = CRGB::Black;
    leds[c11-5] = leds[c12-5] = leds[c13-5] = leds[c14-5] = leds[c17-5] = leds[c18-5]  = CRGB::Black;
    counterhasended = false;
  }

  FastLED.show();
  
  endcounter++;
  secondEndTimer++;

  if(secondEndTimer >= 6) {
    counterTwoEnd = true;
    secondEndTimer = 0;
  }

  if(endcounter == 249) {
    counterhasended = true;
  }

  if(endcounter >=250) {
    counterended = true;
    endcounter = 0;
  }

  if(fourcounter >= 143) {
    leds[fourcounter-1] = CRGB::Black;
    fourcounter = 1;
    leds[dirtybit-1] = CRGB::Black;
    leds[dirtybit] = CRGB::Black;
  }

  if(sevencounter >= 143) {
    leds[sevencounter-1] = CRGB::Black;
    sevencounter = 1;
    leds[dirtybit-1] = CRGB::Black;
    leds[dirtybit] = CRGB::Black;
  }

  if(scndcounter >= 143) {
    leds[scndcounter-1] = CRGB::Black;
    scndcounter = 1;
    leds[dirtybit-1] = CRGB::Black;
    leds[dirtybit] = CRGB::Black;
  }

  if(thrdcounter >= 143) {
    leds[thrdcounter-1] = CRGB::Black;
    thrdcounter = 1;
    leds[dirtybit-1] = CRGB::Black;
    leds[dirtybit] = CRGB::Black;
  }

  if(counter >= 143) {
    leds[counter-1] = CRGB::Black;
    counter = 1;
    leds[dirtybit-1] = CRGB::Black;
    leds[dirtybit] = CRGB::Black;
  }
}

void lightStripTrafficAdvisor() {
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

  
  leds[counter] = CRGB::OrangeRed;
  leds[scndcounter] = CRGB::OrangeRed;
  leds[counter-1] = CRGB::OrangeRed;
  leds[scndcounter-1] = CRGB::OrangeRed;
  leds[thrdcounter] = CRGB::OrangeRed;
  leds[fourcounter] = CRGB::OrangeRed;
  leds[sevencounter] = CRGB::OrangeRed;
  leds[sevencounter-1] = CRGB::OrangeRed;



  delay(8);
  FastLED.show();
  counter++;
  scndcounter++;
  thrdcounter++;
  fourcounter++;
  sevencounter++;

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

void debugMultipuleLights(){
  int colorR = 255; 
  int colorB = 120;
  int colorG = 100;
  leds[20] = leds[40] = leds[60] = leds[80] = leds[100] = leds[120] = CRGB(colorR, colorG, colorB);
  FastLED.show();
}

void warpCoreMode() {
  leds[counter-1] = CRGB::Black;
  leds[counter] = CRGB::White;
  leds[sevencounter-1] = CRGB::Black;
  leds[sevencounter] = CRGB::White;
  FastLED.show();
  if(counterended) {
    counter++;
    sevencounter++;
    counterended = false;
  }

  endcounter++;
  if(endcounter >= 3) {
    counterended = true;
    endcounter = 0;
  }

  if(counter > 143){
    leds[counter-1] = CRGB::Black;
    counter = 1;
  }

  if(sevencounter > 143){
    leds[sevencounter-1] = CRGB::Black;
    sevencounter = 1;
  }
}

void fullLighting() {
  fill_solid(leds, 100, CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue));
  FastLED.show();
}

void FullLightFlash() {
  if (counterhasended && !counterended) {
    fill_solid(leds, 70, CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue));
    counterhasended = false;
    counterended = true;
    FastLED.show();
  } 
  else if (counterended && counterhasended) {
    fill_solid(leds, 100, CRGB::Black);
    counterended = false;
    counterhasended = false;
    FastLED.show();
  }

  if (secondEndTimer >= 110) {
    counterhasended = true;
    secondEndTimer = 0;
  }

  secondEndTimer++;

}

void twoPhaseLightCusomColor() {
  if(phase1&&counterended){
    leds[c12-5] = leds[c14-5] = leds[c18-5] = CRGB::Black;
    leds[c11-5] = leds[c13-5] = leds[c17-5] = CRGB(DynamicData::get().red,DynamicData::get().green,DynamicData::get().blue);
    phase1 = false;
    counterended = false;
    counterhasended = false;
  }
  
  else if(!phase1&&counterended){
    leds[c11] =  leds[c13]  = leds[c17] =  CRGB(DynamicData::get().red,DynamicData::get().green,DynamicData::get().blue);
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

void fillSolid80() {
  fill_solid(leds, 80, CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue));
  FastLED.show();
}

void offline() {
  fill_solid(leds, 143, CRGB::Black);
  FastLED.show();
}

void lighting() {
  leds[20] = leds [60] = leds[100] = leds[140] = CRGB(DynamicData::get().red, DynamicData::get().green, DynamicData::get().blue);
  FastLED.setBrightness(150);
  FastLED.show();
}

// http://192.168.54.205/change?scene=4&red=0&green=0&blue=0 use that for changing scene and color (scene 4 (onyl soem spots) and 5 (full lighting) and case 6 (flashing in that color) only so far)
void loop() {
  checkNetworkSet();
  webPage.loop();
  if(oldScene != DynamicData::get().scene) {
    offline();
    
  }
  oldScene = DynamicData::get().scene;
  switch (DynamicData::get().scene)
  {
  case 0:
    trafficAdvisorWithFLashers();
    break;

  case 1:
    lightStripTrafficAdvisor();
    break;

  case 2:
    twoPahseLights();
    break;

  case 3:
    warpCoreMode();
    break;
  
  case 4:
    lighting();
    break;
  
  case 5:
    fullLighting();
    break;
  
  case 6:
    FullLightFlash();
    break;

  case 7:
    fillSolid80();
    break;
  case 8:
    twoPhaseLightCusomColor();
    break;

  default:
    offline();
    break;
  }

  delay(1);
  
}