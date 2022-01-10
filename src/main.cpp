#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "DHT.h"

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define DHTPIN 12
#define DHTTYPE DHT11

// Insert your network credentials
// #define WIFI_SSID "Thamali"
// #define WIFI_PASSWORD "Hellooulu987"

#define WIFI_SSID "panoulu"
#define WIFI_PASSWORD ""


// Insert Firebase project API Key
#define API_KEY "AIzaSyBoMlOyfkpSVCR2KZHWMtTuQa27enmtQcc"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://airquality-e7a4d-default-rtdb.europe-west1.firebasedatabase.app/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

DHT dht11(DHTPIN, DHTTYPE);

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

const int h2Pin = 34;
const int ch4Pin = 35;
const int coPin = 32;

int h2Value = 0;
int ch4Value = 0;
int coValue = 0;
float humidValue = 0.0;
float tempValue = 0.0;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  dht11.begin();
}

void loop()
{

  h2Value = analogRead(h2Pin);
  ch4Value = analogRead(ch4Pin);
  coValue = analogRead(coPin);
  humidValue = dht11.readHumidity();
  tempValue = dht11.readTemperature();

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    // write H2 value
    if (Firebase.RTDB.setInt(&fbdo, "AirQuality/Hydrogen", h2Value))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    // save H2 value
    if (Firebase.RTDB.pushInt(&fbdo, "AirQualityDB/HydrogenDB", h2Value))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }




    // write CH4 value
    if (Firebase.RTDB.setInt(&fbdo, "AirQuality/Methane", ch4Value))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    // save CH4 value
    if (Firebase.RTDB.pushInt(&fbdo, "AirQualityDB/MethaneDB", ch4Value))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }




    // write CO value
    if (Firebase.RTDB.setInt(&fbdo, "AirQuality/CarbonMonoxide", coValue))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    // save CO value
    if (Firebase.RTDB.pushInt(&fbdo, "AirQualityDB/CarbonMonoxideDB", coValue))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }




    // write humidity value
    if (Firebase.RTDB.setFloat(&fbdo, "AirQuality/Humidity", humidValue))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    // save humidity value
    if (Firebase.RTDB.pushFloat(&fbdo, "AirQualityDB/HumidityDB", humidValue))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }




    // write temperature value
    if (Firebase.RTDB.setFloat(&fbdo, "AirQuality/Temperature", tempValue))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    // save temperature value
    if (Firebase.RTDB.pushFloat(&fbdo, "AirQualityDB/TemperatureDB", tempValue))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}