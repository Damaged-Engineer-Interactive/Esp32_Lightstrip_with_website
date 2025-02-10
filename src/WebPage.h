#ifndef WebPage_h
#define WebPage_h

#include <Arduino.h>
#include <WebServer.h>
#include <Update.h>
#include "NVMData.h"
#include "DynamicData.h"

int redirectTime = 1;

class WebPage
{
private:
    WebServer server;
    String GenHeader(int redirectTime);
    String GenFooter();
    String GenTableStart();
    String GenTableNewColumn();
    String GenTableRows(String Content[], int Count);
    String GenTableEnd();
    String addKeyValuePair(String key, String value);
    void handleRoot();
    void handleChange();
    void handleNotFound();
    void handleFirmware();
    void handleUpload();
    void handleUpload2();
    void handlePowerSource();
    void handlePowerSource2();
    void handleJson();
    String setMessage(String arg, String value, String result);
public:
    WebPage();
    ~WebPage();
    void Init();
    void loop();
    bool newNetworkSet = false;
};
WebPage::WebPage()
{
}
WebPage::~WebPage()
{
}
WebServer server(80);
void WebPage::loop() {
    server.handleClient();
}
void WebPage::Init() {
    
    server.on("/", std::bind(&WebPage::handleRoot, this));
    server.on("/change", std::bind(&WebPage::handleChange, this));
    server.on("/json", std::bind(&WebPage::handleJson, this));
    // choose bin file
    server.on("/firmware", HTTP_GET, std::bind(&WebPage::handleFirmware, this));
    /*handling uploading firmware file */
    server.on("/update", HTTP_POST, std::bind(&WebPage::handleUpload, this), std::bind(&WebPage::handleUpload2, this));
    server.onNotFound(std::bind(&WebPage::handleNotFound, this));
    server.begin();
    Serial.println("HTTP server started");
}
String WebPage::GenHeader(int redirectTime)
{
  Serial.println(redirectTime);
  String message= "";
  message += "<html>";
  message += "<head>";
  if (redirectTime > 0)
  {
    String redirectTimeString = String(redirectTime);
    message += "<meta http-equiv=\"refresh\" content=\"" + redirectTimeString + ";url=http://" + DynamicData::get().ipaddress + "/\" />";
  }
  message += "</head>";
  message += "<body>\n";
  message += "\t<p>This will only work until it breaks</p>\n";
  message += "\t<p>------------V0.2.1---------------</p>\n";
  return message;
}
String WebPage::GenFooter()
{
  String message= "";
  message += "\t<p>---------------------------</p>\n";
  message += "</body>";
  message += "</html>\n";
  return message;
}
String WebPage::GenTableStart()
{
  String message= "";
  message += "\t<table border=\"4\">";
  message += "\t<tr>\n";
  return message;
}
String WebPage::GenTableNewColumn()
{
  String message= "";
  message += "\t</tr>";
  message += "\t<tr>\n";
  return message;
}
String WebPage::GenTableRows(String Content[], int Count)
{
  String message = "";
  for (int i = 0; i < Count; i++)
  {
    message += "<td>"+Content[i]+"</td>";
  }
  message +="\n";
  return message;
}
String WebPage::GenTableEnd()
{
  String message= "";
  message += "\t</tr>";
  message += "\t</table>\n";
  return message;
}
void WebPage::handleNotFound() {
  String message= "";
  message += GenHeader(3);
  message += "404: Listen, I'm just as confused as you are okay?\n\n";
  message += "How did you get here anyways? Just go back to the home page okay?\n\n";
  message += GenFooter();
  server.send(200, "text/html", message);
}
void WebPage::handleChange() {
  String message = "Ohh oh!\n\n";
  bool netNameSet = false;
  bool netPasswordSet = false;
  String netName = "";
  String netPassword = "";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    if (server.argName(i) == "netname")
    {
      netNameSet = true;
      netName = server.arg(i);
    }
    else if (server.argName(i) == "password")
    {
      netPasswordSet = true;
      netPassword = server.arg(i);
    }
    else if (server.argName(i) == "scene") {
      DynamicData::get().scene =  server.arg(i).toInt();
      message = "I dont exist";
    }
    else if (server.argName(i) == "red") {
      DynamicData::get().red =  server.arg(i).toInt();
    }
    else if (server.argName(i) == "green") {
      DynamicData::get().green =  server.arg(i).toInt();
    }
    else if (server.argName(i) == "blue") {
      DynamicData::get().blue =  server.arg(i).toInt();
    }
    else if (server.argName(i) == "waittime") {
      DynamicData::get().waittime =  server.arg(i).toInt();
    }
  }
  if ((netPasswordSet == true)&&(netNameSet == true))
  {
    newNetworkSet = true;
    NVMData::get().SetNetData(netName, netPassword);
    message = "You got it!";
  }
  String returnMessage= "";
  returnMessage += GenHeader(3);
  returnMessage += message;
  returnMessage += "</body>";
  returnMessage += "</html>";
  server.send(200, "text/html", returnMessage);
}
void WebPage::handleFirmware() {
  const char* serverIndex = 
    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
    "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
      "<input type='file' name='update'>"
            "<input type='submit' value='Update'>"
        "</form>"
    "<div id='prg'>progress: 0%</div>"
    "<script>"
      "$('form').submit(function(e){"
      "e.preventDefault();"
      "var form = $('#upload_form')[0];"
      "var data = new FormData(form);"
      " $.ajax({"
      "url: '/update',"
      "type: 'POST',"
      "data: data,"
      "contentType: false,"
      "processData:false,"
      "xhr: function() {"
      "var xhr = new window.XMLHttpRequest();"
      "xhr.upload.addEventListener('progress', function(evt) {"
      "if (evt.lengthComputable) {"
      "var per = evt.loaded / evt.total;"
      "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
      "}"
      "}, false);"
      "return xhr;"
      "},"
      "success:function(d, s) {"
      "console.log('success!')" 
    "},"
    "error: function (a, b, c) {"
    "}"
    "});"
    "});"
    "</script>";
  server.sendHeader("Connection", "close");
  server.send(200, "text/html", serverIndex);
}
void WebPage::handleUpload() {
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  ESP.restart();
}
void WebPage::handleUpload2() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("Update: %s\n", upload.filename.c_str());
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    /* flashing firmware to ESP*/
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) { //true to set the size to the current progress
      Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
    } else {
      Update.printError(Serial);
    }
  }
}
void WebPage::handleRoot() {
  String uri = DynamicData::get().ipaddress;
  String message = "";
  message += GenHeader(0);
  
  if( DynamicData::get().setNewNetwork == true )
  {
    message += "\t<form action=\"change\">\n";
    message += "\t<label class=\"h2\" form=\"networkdata\">Network name</label>\n";
    message += "\t<div>\n";
    message += "\t<label for=\"netname\">netname</label>\n";
    message += "\t<input type=\"text\" name=\"netname\" maxlength=\"30\">\n";
    message += "\t</div>\n";
    message += "\t<div>\n";
    message += "\t<label for=\"password\">password</label>\n";
    message += "\t<input type=\"text\" name=\"password\" maxlength=\"40\">\n";
    message += "\t</div>\n";
    message += "\t<div>\n";
    message += "\t<button type=\"reset\">reset</button>\n";
    message += "\t<button type=\"submit\">set</button>\n";
    message += "\t</div>\n";
    message += "\t</form>\n";
  }
  message += "\t<form action=\"change\">\n";
  message += "\t<label class=\"h2\" form=\"scene\">set the stage </label>\n";
  message += "\t<div>\n";
  message += "\t<label for=\"scene\">scene</label>\n";
  message += "\t<input type=\"text\"value=\"" + String(DynamicData::get().scene) + "\" name=\"scene\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<div>\n";
  message += "\t<label for=\"red\">red</label>\n";
  message += "\t<input type=\"text\"placeholder=\"" + String(DynamicData::get().red) + "\" name=\"red\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<div>\n";
  message += "\t<label for=\"green\">green</label>\n";
  message += "\t<input type=\"text\"placeholder=\"" + String(DynamicData::get().green) + "\" name=\"green\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<div>\n";
  message += "\t<label for=\"blue\">blue</label>\n";
  message += "\t<input type=\"text\"placeholder=\"" + String(DynamicData::get().blue) + "\" name=\"blue\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<label for=\"waittime\">waittime</label>\n";
  message += "\t<input type=\"waittime\"value=\"" + String(DynamicData::get().waittime) + "\" name=\"waittime\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<label for=\"brightness\">brightness</label>\n";
  message += "\t<input type=\"brightness\"placeholder=\"" + String(DynamicData::get().brightness) + "\" name=\"brightness\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<div>\n";
  message += "\t<button type=\"reset\">clear</button>\n";
  message += "\t<button type=\"submit\">set</button>\n";
  message += "\t</div>\n";
  message += "\t</form>\n";

  char timeh[5];
  char timem[5];
  char times[5];
  message += "\t<p>---------------------------</p>\n";
  message += "\t<p>ipaddress:  " + DynamicData::get().ipaddress + "</p>\n";
  message += "\t<p>Thanks for trying my site mate. Have a good day!";
  message += "\t<p>You have to <b> always </b> set the RGB value again (its only placeholders), otherwise it will reset the value to 0";
  message += "\t<p>As mandated I have say a fair <i> <b> epilepsie warning </b> </i> for the scene(s) '6, 9' ";
  message += GenFooter();
  server.send(200, "text/html", message);
}

String WebPage::addKeyValuePair(String key, String value)
{
  String retVal = "";
  retVal += "\"";
  retVal += key;
  retVal += "\" :\"";
  retVal += value;
  retVal += "\"";
  return retVal;
}
void WebPage::handleJson()
{
  String message = "";
  String key = "";
  message += "{ ";
  
  message += "}";
  server.send(200, "text/plain", message);
}
#endif