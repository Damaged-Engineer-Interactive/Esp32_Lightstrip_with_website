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
    server.on("/firmware", HTTP_GET, std::bind(&WebPage::handleFirmware, this));
    server.on("/update", HTTP_POST, std::bind(&WebPage::handleUpload, this), std::bind(&WebPage::handleUpload2, this));
    server.onNotFound(std::bind(&WebPage::handleNotFound, this));
    server.begin();
    Serial.println("HTTP server started");
}

String WebPage::GenHeader(int redirectTime)
{
    Serial.println(redirectTime);
    String message = "";
    message += "<html><head>";
    if (redirectTime > 0)
    {
        message += "<meta http-equiv=\"refresh\" content=\"" + String(redirectTime) + ";url=http://" + DynamicData::get().ipaddress + "/\" />";
    }
    message += "<style>";
    message += "body { background-color: black; color: white; font-family: Arial, sans-serif; margin: 0; padding: 0; }";
    message += "input, button { background-color: #444; color: white; border: 1px solid white; padding: 5px; margin: 5px 0; }";
    message += "input::placeholder { color: lightgray; }";
    message += "table { border-collapse: collapse; width: 100%; background-color: #222; color: white; }";
    message += "td { padding: 8px; border: 1px solid white; }";
    message += "h1, h2 { color: lightgray; }";
    message += "a { color: cyan; text-decoration: none; }";
    message += "a:hover { color: lightblue; }";
    message += "</style></head><body>";
    return message;
}

String WebPage::GenFooter()
{
    return "<p>---------------------------</p></body></html>";
}

String WebPage::GenTableStart()
{
    return "<table border=\"1\" style=\"width: 100%; border: 1px solid white; background-color: #222;\"><tr>";
}

String WebPage::GenTableNewColumn()
{
    return "</tr><tr>";
}

String WebPage::GenTableRows(String Content[], int Count)
{
    String message = "";
    for (int i = 0; i < Count; i++)
    {
        message += "<td>" + Content[i] + "</td>";
    }
    message += "\n";
    return message;
}

String WebPage::GenTableEnd()
{
    return "</tr></table>";
}

void WebPage::handleNotFound() {
    String message = GenHeader(3);
    message += "<p>404: I'm just as confused as you are. How did you get here?</p>";
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
    else if (server.argName(i) == "brightness"){
      DynamicData::get().brightness = server.arg(i).toInt();
    }
  }
  if ((netPasswordSet == true)&&(netNameSet == true))
  {
    newNetworkSet = true;
    NVMData::get().SetNetData(netName, netPassword);
    message = "You got it!";
  }
  handleRoot();
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
  message += "\t<input type=\"text\"value=\"" + String(DynamicData::get().red) + "\" name=\"red\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<div>\n";
  message += "\t<label for=\"green\">green</label>\n";
  message += "\t<input type=\"text\"value=\"" + String(DynamicData::get().green) + "\" name=\"green\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<div>\n";
  message += "\t<label for=\"blue\">blue</label>\n";
  message += "\t<input type=\"text\"value=\"" + String(DynamicData::get().blue) + "\" name=\"blue\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<label for=\"waittime\">waittime</label>\n";
  message += "\t<input type=\"waittime\"value=\"" + String(DynamicData::get().waittime) + "\" name=\"waittime\" maxlength=\"4\">\n";
  message += "\t</div>\n";
  message += "\t<label for=\"brightness\">brightness</label>\n";
  message += "\t<input type=\"brightness\"value=\"" + String(DynamicData::get().brightness) + "\" name=\"brightness\" maxlength=\"4\">\n";
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
  message += "\t<p>The values for <b> RGB </b> no longer reset and are actual values";
  message += "\t<p>As mandated I have say a fair <i> <b> epilepsie warning </b> </i> for the scene(s) '6, 9' ";
  message += "\t<p> For the scene <i> 11 </i> I recommend using a value over <i> 150 </i> as <b> waittime </b>";
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
