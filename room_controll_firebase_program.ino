


#include "FirebaseESP8266.h"	
#include <ESP8266WiFi.h>
#include "DHT.h"


#define FIREBASE_HOST "https://room-control-with-monitoring-default-rtdb.firebaseio.com/" 
#define FIREBASE_AUTH "cj21zMOwMYGsu9dk1SmYbmPiPIE508BzqFQEf3mm"
#define WIFI_SSID "Wifi_S"
#define WIFI_PASSWORD "Souraf@123"

#define DHTPIN D2		
int led_1 = D5;			
int led_2 = D7;			
int wifi_led = D4;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;
//FirebaseData ledData2;


FirebaseJson json;


void setup()
{

  Serial.begin(9600);

  dht.begin();
  pinMode(led_1,OUTPUT);
  pinMode(led_2,OUTPUT);
  pinMode(wifi_led,OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    //delay(300);
    digitalWrite(wifi_led, HIGH);
    delay(250);
    digitalWrite(wifi_led, LOW);   
    delay(250);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  digitalWrite(wifi_led, LOW);
  

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void sensorUpdate(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.print("°C  ,");


  Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", t);
  Firebase.setFloat(firebaseData, "/FirebaseIOT/humidity", h);
}
void loop() {
  sensorUpdate();
  
  if (Firebase.getString(ledData, "/FirebaseIOT/led1")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(led_1, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(led_1, LOW);
    }
  }
  if (Firebase.getString(ledData, "/FirebaseIOT/led2")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(led_2, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(led_2, LOW);
    }
  }
  delay(100);
}