#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <String.h>
#define ACCEPT_TYPE "text/csv" 
WiFiClient client;

const char ssid[] = "GLA-CSED";
const char password[] = "GLACAMPUS";

const char host[] = "dcs.glaitm.org"; 
const int httpsPort = 7080;
const char appKey[]= "f6b94c8d-1c77-40c0-9be5-c8adb5bfbabd";

const char Thing[] = "Smart_Jacket";
const char Property1[] = "Alcohol";
const char Property2[] = "Air_Quality";
const char Property3[] = "Pulse_Rate";
const char Property4[] = "Sweat";
const char Property5[] = "Body_Temp";
const char Property6[] = "Room_Temp";
const char Property7[] = "Room_Hum";
const char Property8[] = "X_Value";
const char Property9[] = "Y_Value";
const char Property10[] = "Z_Value";



char e[9];

void setup() 
{
  Wire.begin(4,5);  
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
{
    delay(500);
    Serial.print(".");
 }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 

}


void loop() 
{
  Wire.requestFrom(10, 64);
  char array[64];
  int i=0;
  while (0 < Wire.available())
  {
    char c = Wire.read();
    array[i] = c;
    Serial.print(array[i]);
    i++;
    
  }
  Serial.println("");
  char *strings[10]; 
  char *ptr = NULL;   
  byte index = 0;
  ptr = strtok(array, ",");
  while (ptr != NULL)
  {
    strings[index] = ptr;
    index++;
    ptr = strtok(NULL, ",");
  }
  Serial.println(strings[0]);   //Alcohol
  Serial.println(strings[1]);   //Air Quality
  Serial.println(strings[2]);   //Pulse Rate
  Serial.println(strings[3]);   //Swaet
  Serial.println(strings[4]);   //Body temp 
  Serial.println(strings[5]);   //Room Temp 
  Serial.println(strings[6]);   //Room Hum
  Serial.println(strings[7]);   //X Value
  Serial.println(strings[8]);   //Y value
  Serial.println(strings[9]);   //X value
  



  
  Put(Thing,Property1,strings[0]);
  Put(Thing,Property2,strings[1]);
  Put(Thing,Property3,strings[2]);
  Put(Thing,Property4,strings[3]);
  Put(Thing,Property5,strings[4]);
  Put(Thing,Property6,strings[5]);
  Put(Thing,Property7,strings[6]);
  Put(Thing,Property8,strings[7]);
  Put(Thing,Property9,strings[8]);
  Put(Thing,Property10,strings[9]);

  delay(1000);
}

void Put(String ThingName, String ThingProperty, String Value)
  {

    Serial.println(host);
  if (!client.connect(host, httpsPort))
 {
    Serial.println("connection failed");
    return;
  } else

 {
    Serial.println("Connected to ThingWorx.");
  }
  String url = "/Thingworx/Things/" + ThingName + "/Properties/" + ThingProperty; 
  Serial.print("requesting URL: ");
  Serial.println(url);

  String strPUTReqVal = "{\"" + ThingProperty + "\":\"" + Value + "\"}";  
  Serial.print("PUT Value: ");
  Serial.println(strPUTReqVal);

  client.print(String("PUT ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "appKey:"+ appKey + "\r\n" +
               "x-thingworx-session: false" + "\r\n" +
               "Accept: application/json" + "\r\n" +
               "Connection: close" + "\r\n" +
               "Content-Type: application/json" + "\r\n" +
               "Content-Length: " + String(strPUTReqVal.length()) + "\r\n\r\n" +
               strPUTReqVal + "\r\n\r\n");   

  while (client.connected()) 
{
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  client.stop();
}
