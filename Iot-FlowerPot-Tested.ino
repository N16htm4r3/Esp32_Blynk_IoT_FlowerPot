#define BLYNK_AUTH_TOKEN            "AUTHTOKEN"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>


WidgetLCD lcd(V0);
WidgetLCD lcd1(V5);

int pin = 13;

int ldrvalue=0;
int ldroutput=0;

int sensorpin=35;
int sensorvalue=0;
int outputvalue=0;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SSID";
char pass[] = "PASSWORD";

#define DHTPIN 4     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V3, h);
  Blynk.virtualWrite(V4, t);
  lcd.print(0,0,"Sic:");
  lcd.print(4,0,t);
  lcd.print(0,1,"Nem:");
  lcd.print(4,1,h);
}

void setup()
{
  pinMode(pin, OUTPUT);
  pinMode(pin, HIGH);
  
  Serial.begin(460800);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  sensorvalue=analogRead(sensorpin);
  outputvalue=map(sensorvalue,800,4095,100,0); //100,0 pottupaaru
//  delay(1000);
  Blynk.virtualWrite(V1,outputvalue);
  
  lcd1.print(9,0,"Toprak:");
  lcd1.print(12,1,outputvalue);
  delay(2000);
  
  ldrvalue=analogRead(34);
  ldroutput=map(ldrvalue,1000,4095,100,0);
  delay(200);
  Blynk.virtualWrite(V2,ldroutput);
  
  if((ldrvalue)<3500)
    {
     lcd1.clear();
     lcd1.print(0,0,"Gunesli");
     lcd1.print(0,1,"Deger:");
     lcd1.print(6,1,ldroutput);

    }
  if((ldrvalue)>3500)
    {
     lcd1.clear();
     lcd1.print(0,0,"Karanlik");
     lcd1.print(0,1,"Deger:");
     lcd1.print(6,1,ldroutput);
    }
      
  Blynk.run();
  timer.run();
}
