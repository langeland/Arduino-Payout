/***********************************************************************************************
 **
 ***********************************************************************************************/
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266HTTPClient.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Servo.h>


/***********************************************************************************************
 **
 ***********************************************************************************************/
#define WIRE_SDA_PIN D4
#define WIRE_SCL_PIN D6

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


/***********************************************************************************************
 **
 ***********************************************************************************************/
Servo servo;
const int servoPin = D2;
const int servoMax = 55;
const int servoMin = 145;
const int servoDalay = 10;
int serverPos = 0;    // variable to store the servo position


/***********************************************************************************************
 **
 ***********************************************************************************************/
int payoutAmount;
int payoutCounter = 0;


/***********************************************************************************************
 **
 ***********************************************************************************************/
static const unsigned char PROGMEM logo16_glcd_bmp [] = {
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xe3,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xc1,0xff,0xff,0xff,0x3,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xc1,0xff,0xff,0xfe,0x1,0xff,0xff,0xff,0xff,0xff,
0xc0,0x7,0xfc,0x3,0x83,0xf8,0x40,0x30,0x0,0xfc,0x1,0xff,0xc0,0x1f,0x0,0x7f,
0x80,0x3,0xf0,0x1,0x83,0xf0,0x40,0x30,0x0,0xfc,0x3f,0xff,0x0,0x1e,0x0,0x3f,
0x80,0x1,0xe0,0x0,0xc3,0xf0,0xc0,0x30,0x0,0xfc,0x3f,0xfe,0x0,0x1c,0x0,0x1f,
0x80,0x0,0xe0,0x60,0xc1,0xf0,0xc1,0xff,0xe0,0xfc,0x3f,0xfe,0x3,0x18,0x1c,0xf,
0x83,0xe0,0xc1,0xf0,0x61,0xe0,0xc1,0xff,0xc1,0xfc,0x1f,0xfc,0x1f,0xf8,0x3e,0xf,
0x83,0xf0,0xc1,0xf0,0x61,0xe1,0xc1,0xff,0xc3,0xfe,0xf,0xfc,0x1f,0xf8,0x7f,0xf,
0x83,0xf0,0x40,0x0,0x60,0xe1,0xc1,0xff,0x83,0xfc,0xe,0x3c,0x3f,0xf0,0x7f,0x7,
0x83,0xf0,0x40,0x0,0x70,0xc3,0xc1,0xff,0x7,0xf8,0x6,0x3c,0x3f,0xf0,0x7f,0x7,
0x83,0xf0,0x40,0x0,0x70,0xc3,0xc1,0xfe,0xf,0xf0,0xc0,0x3c,0x3f,0xf0,0x7f,0x7,
0x83,0xf0,0x40,0x0,0xf0,0x43,0xc1,0xfc,0x1f,0xf0,0xe0,0x7c,0x1f,0xf0,0x7f,0x7,
0x83,0xf0,0xc1,0xff,0xf8,0x7,0xc1,0xf8,0x3f,0xf1,0xe0,0x7c,0x1f,0xf8,0x7f,0xf,
0x83,0xe0,0xe0,0xff,0xf8,0x7,0xc1,0xf0,0x7f,0xf0,0xf0,0x7e,0xf,0xf8,0x3e,0xf,
0x80,0x0,0xe0,0x0,0xfc,0xf,0xe0,0x30,0x1,0xf0,0x60,0x3e,0x0,0x18,0x0,0xf,
0x80,0x1,0xf0,0x0,0xfc,0xf,0xe0,0x30,0x0,0xf0,0x0,0x3f,0x0,0x1c,0x0,0x1f,
0x80,0x3,0xf8,0x0,0xfc,0xf,0xe0,0x30,0x0,0xf8,0x6,0x1f,0x80,0x1e,0x0,0x3f,
0x80,0xf,0xfe,0x0,0xfe,0x1f,0xf8,0x30,0x0,0xff,0x1f,0x1f,0xe0,0x1f,0x80,0xff,
0x83,0xff,0xff,0xff,0xfc,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x83,0xff,0xff,0xff,0xfc,0x3f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x83,0xff,0xff,0xff,0xf0,0x3f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x83,0xff,0xff,0xff,0xe0,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x83,0xff,0xff,0xff,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xc7,0xff,0xff,0xff,0xf1,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};


/***********************************************************************************************
 ** define your default values here, if there are different values in config.json, they are overwritten.
 ***********************************************************************************************/
char config_data_server[40] = "http://payout.discworld.dk";
char config_data_endpoint[150] = "/api/withdraw.json";

char config_data_port[6] = "80";
char config_data_fingerprint[60] = "55 F6 EA 11 04 D5 8B 09 CE 35 87 DA C2 39 EE B5 A0 9F 6B 11";


/***********************************************************************************************
 **
 ***********************************************************************************************/
//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void configModeCallback (WiFiManager *myWiFiManager) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Entered config mode:");

  display.setCursor(0,15);
  display.println("Connect to:");
  display.println(myWiFiManager->getConfigPortalSSID());

  display.display();

  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}


/***********************************************************************************************
 **
 ***********************************************************************************************/
void wifiConnect(){

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(config_data_server, json["config_data_server"]);
          strcpy(config_data_endpoint, json["config_data_endpoint"]);
          strcpy(config_data_port, json["config_data_port"]);
          strcpy(config_data_fingerprint, json["config_data_fingerprint"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_config_data_server("dataServer", "Data server", config_data_server, 40);
  WiFiManagerParameter custom_config_data_endpoint("dataEndpoint", "Data endpoint", config_data_endpoint, 150);
  WiFiManagerParameter custom_config_data_port("dataPort", "Data server port", config_data_port, 6);
  WiFiManagerParameter custom_config_data_fingerprint("dataFingerprint", "Data server fingerprint", config_data_fingerprint, 59);
  


  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setAPCallback(configModeCallback);

  //wifiManager.setCustomHeadElement("<style>html, body {height: 100%; background-color: #333; color: #FFF} html {-webkit-text-size-adjust: 100%; -ms-text-size-adjust: 100%;} body {margin: 0; display: table; width: 100%; height: 100%; min-height: 100%; font-family: \"Helvetica Neue\", Helvetica, Arial, sans-serif; -webkit-box-shadow: inset 0 0 100px rgba(0,0,0,.5); box-shadow: inset 0 0 100px rgba(0,0,0,.5);} button {background-color: #FFF; border-radius: 6px; color: #333}</style>");
  wifiManager.setCustomBodyElement("<h1>Payout</h1>");

  //set static ip
  //wifiManager.setSTAStaticIPConfig(IPAddress(10, 0, 1, 99), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));

  //add all your parameters here
  wifiManager.addParameter(&custom_config_data_server);
  wifiManager.addParameter(&custom_config_data_endpoint);
  wifiManager.addParameter(&custom_config_data_port);
  wifiManager.addParameter(&custom_config_data_fingerprint);

  //reset settings - for testing
  // wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("Data Display")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(config_data_server, custom_config_data_server.getValue());
  strcpy(config_data_endpoint, custom_config_data_endpoint.getValue());
  strcpy(config_data_port, custom_config_data_port.getValue());
  strcpy(config_data_fingerprint, custom_config_data_fingerprint.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["config_data_server"] = config_data_server;
    json["config_data_endpoint"] = config_data_endpoint;
    json["config_data_port"] = config_data_port;
    json["config_data_fingerprint"] = config_data_fingerprint;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());  
}





/***********************************************************************************************
 **
 ***********************************************************************************************/
void setup() {
  Serial.begin(115200);
  delay(1000);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.println("/***********************************************************************************************");
  Serial.println(" ** Payout v0.0.1");
  Serial.println(" ***********************************************************************************************/");

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  Serial.println("Starting display:");
  Serial.println("  I2C Wire begin");
  Wire.begin(WIRE_SDA_PIN, WIRE_SCL_PIN);
  Serial.println("  OK");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();

  // miniature bitmap display
  display.drawBitmap(0, 20,  logo16_glcd_bmp, 128, 28, 1);
  display.display();
  delay(2000);

  Serial.println("Starting servo:");
  servo.attach(servoPin);  // attaches the servo on GIO2 to the servo object
  servo.write(servoMax);
  Serial.println("  OK");

  Serial.println("Starting WIFI:");
  wifiConnect();
  Serial.println("  OK");

  Serial.println("##########################");
}



void loop() {
  if (timer.check() == 1) {
    updateDisplay();
    yield();
  }
  delay(10);
}


