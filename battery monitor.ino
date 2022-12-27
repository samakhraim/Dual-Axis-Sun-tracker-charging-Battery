#include <ESP8266WiFi.h>

String apiKey = "U742JKMOH3YSP113";
const char* ssid =  "Sama Khraim";     // Enter your WiFi Network's SSID
const char* pass =  "samakhraim"; // Enter your WiFi Network's Password
const char* server = "api.thingspeak.com";
 int analogInPin  = A0;    // Analog input pin
int sensorValue;          // Analog Output of Sensor
float calibration = 0.36; // Check Battery voltage using multimeter & add/subtract the value
int bat_percentage;
 
WiFiClient client;
 
void setup()
{
  Serial.begin(115200);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print("*");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
 
void loop()
{
  sensorValue = analogRead(analogInPin);
  float voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration); //multiply by two as voltage divider network is 100K & 100K Resistor
 
  bat_percentage = mapfloat(voltage, 2.8, 4.2, 0, 100); //2.8V as Battery Cut off Voltage & 4.2V as Maximum Voltage
 
  if (bat_percentage >= 100)
  {
    bat_percentage = 100;
  }
  if (bat_percentage <= 0)
  {
    bat_percentage = 1;
  }
 
  Serial.print("Analog Value = ");
  Serial.print(sensorValue);
  Serial.print("\t Output Voltage = ");
  Serial.print(voltage);
  Serial.print("\t Battery Percentage = ");
  Serial.println(bat_percentage);
  delay(1000);
 
  if (client.connect(server, 80))
  {
 
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(voltage);
    postStr += "&field2=";
    postStr += String(bat_percentage);
    postStr += "\r\n\r\n";
 
    client.print("POST /update HTTP/1.1\n");
    delay(100);
    client.print("Host: api.thingspeak.com\n");
    delay(100);
    client.print("Connection: close\n");
    delay(100);
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    delay(100);
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    delay(100);
    client.print("Content-Length: ");
    delay(100);
    client.print(postStr.length());
    delay(100);
    client.print("\n\n");
    delay(100);
    client.print(postStr);
    delay(100);
  }
  client.stop();
  Serial.println("Sending....");
  delay(15000);
}
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}