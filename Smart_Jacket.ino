#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define PRpin A0
#define Airpin A1
#define Alcoholpin A2
#define GSRpin A3
#define CW 6

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_MPU6050 mpu;

#define DHTPIN 2     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE); 



unsigned int avgValueAlcohol; 
int bufAlcohol[10],tempAlcohol;
 
uint8_t period = 1000;
char c;

unsigned int avgValuePR; 
int bufPR[10],tempPR;

unsigned int avgValueAir; 
int bufAir[10],tempAir;

unsigned int avgValueGSR; 
int bufGSR[10],tempGSR;

String str;

void setup(){
  Wire.begin(10);
  mlx.begin();
  dht.begin();

  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendEvent);
  
  Serial.begin(9600);
  
  pinMode(CW, OUTPUT);
  digitalWrite(CW,HIGH);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  delay(100);

}


void loop(){

/////////////////////////////////////Alcohol///////////////////////////////////////////////////////

   for(int i=0;i<10;i++)      
  { 
    bufAlcohol[i]=analogRead(Alcoholpin);
    delay(100);
  }
  for(int i=0;i<9;i++)        
  {
    for(int j=i+1;j<10;j++)
    {
      if(bufAlcohol[i]>bufAlcohol[j])
      {
        tempAlcohol=bufAlcohol[i];
        bufAlcohol[i]=bufAlcohol[j];
        bufAlcohol[j]=tempAlcohol;
      }
    }
  }
  avgValueAlcohol=0;
  for(int i=2;i<8;i++)
  {
     avgValueAlcohol+=bufAlcohol[i];                     
  }
  
  float AlcoholValue = analogRead(Alcoholpin);
  Serial.println("Alcohol value"+ String(AlcoholValue));

///////////////////////////////////////////////PR/////////////////////////////////////////////////////////

   for(int i=0;i<10;i++)      
  { 
    bufPR[i]=analogRead(PRpin);
    delay(100);
  }
  for(int i=0;i<9;i++)        
  {
    for(int j=i+1;j<10;j++)
    {
      if(bufPR[i]>bufPR[j])
      {
        tempPR=bufPR[i];
        bufPR[i]=bufPR[j];
        bufPR[j]=tempPR;
      }
    }
  }
  avgValuePR=0;
  for(int i=2;i<8;i++)
  {
     avgValuePR+=bufPR[i];                     
  }
  float PRValue = analogRead(PRpin);
  Serial.println("PR value"+ String(PRValue));

//////////////////////////////////////////////////////Air Quality////////////////////////////////////////

  for(int i=0;i<10;i++)      
  { 
    bufAir[i]=analogRead(Airpin);
    delay(100);
  }
  for(int i=0;i<9;i++)        
  {
    for(int j=i+1;j<10;j++)
    {
      if(bufAir[i]>bufAir[j])
      {
        tempAir=bufAir[i];
        bufAir[i]=bufAir[j];
        bufAir[j]=tempAir;
      }
    }
  }
  avgValueAir=0;
  for(int i=2;i<8;i++)
  {
     avgValueAir+=bufAir[i];                     
  }
  
  float AirValue = analogRead(Airpin);
  Serial.println("Air value"+ String(AirValue));

  //////////////////////////////////////////////////////Sweat Sensor - GSR////////////////////////////////////////

  for(int i=0;i<10;i++)      
  { 
    bufGSR[i]=analogRead(GSRpin);
    delay(100);
  }
  for(int i=0;i<9;i++)        
  {
    for(int j=i+1;j<10;j++)
    {
      if(bufGSR[i]>bufGSR[j])
      {
        tempGSR=bufGSR[i];
        bufGSR[i]=bufGSR[j];
        bufGSR[j]=tempGSR;
      }
    }
  }
  avgValueGSR=0;
  for(int i=2;i<8;i++)
  {
     avgValueGSR+=bufGSR[i];                     
  }
  
  float GSRValue = analogRead(GSRpin);
  Serial.println("GSR value"+ String(GSRValue));

  //////////////////////////////////////////Body Temp - MLX90614 Sensor ///////////////////////////////////////////////////
  
  float MLXValue = mlx.readObjectTempC();
  Serial.println("MLX Value"+ String(MLXValue));

   //////////////////////////////////////////Room Temp/Humidity - DHT11 Sensor ///////////////////////////////////////////////////
  
  float HUMValue = dht.readHumidity();
  float TEMPValue = dht.readTemperature();
  Serial.println("Humidity Value"+ String(HUMValue));
  Serial.println("Temperature Value"+ String(TEMPValue));

  //////////////////////////////////////////Activity Tracking and Fall Detection - MPU6030 ///////////////////////////////////////////////////

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  float XValue = a.acceleration.x;
  float YValue = a.acceleration.y;
  float ZValue = a.acceleration.z;
  
  Serial.println("X Value"+ String(XValue));
  Serial.println("Y Value"+ String(YValue));
  Serial.println("Z Value"+ String(YValue));


  //////////////////////////////////final string/////////////////////////////////////////////////////

  str = String(AlcoholValue)+ ","+ String(AirValue)+ ","+ String(PRValue)+","+ String(GSRValue)+","+ String(MLXValue)+","+ String(HUMValue)+"," + String(TEMPValue)+","+ String(XValue)+","+ String(YValue)+","+ String(ZValue)+"," ;
  Serial.println(str);


  ////////////////////////////////////////////////////////////motor////////////////////////////////////////////
 
  if(c=='0')
  {
    digitalWrite(CW,HIGH);
    Serial.println(c); 
  }
  else if(c=='1')
  {
    digitalWrite(CW, LOW);
    Serial.println(c); 
  }
   
delay(1000);
}

void receiveEvent(int howMany){
  while (Wire.available()>0) {
     c = Wire.read();
    // Serial.print(c);
  }
}
void sendEvent(int howmany){
  for(int i=0;i<str.length();i++)
   {
     Wire.write(str[i]);

   }  
}
