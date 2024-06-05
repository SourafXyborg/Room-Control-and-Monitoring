
#include "FirebaseESP8266.h"	
#include <ESP8266WiFi.h>
#include "DHT.h"


#define FIREBASE_HOST "----" 
#define FIREBASE_AUTH "----"
#define WIFI_SSID "----"
#define WIFI_PASSWORD "----"

#define DHTPIN D2		
int relay_Light = D5;			
int relay_Fan = D6;	
int relay_Cooler = D7;	
int relay_Humidifier = D8;				
int wifi_led = D4;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;
//FirebaseData ledData2;


FirebaseJson json;

char On[] = "1";
char Off[] = "0";

void setup()
{

  Serial.begin(9600);

  dht.begin();
  pinMode(relay_Light, OUTPUT);
  pinMode(relay_Fan, OUTPUT);
  pinMode(relay_Cooler, OUTPUT);
  pinMode(relay_Humidifier, OUTPUT);
  pinMode(wifi_led, OUTPUT);
  void sensorUpdate();
  void hum();
  void fan();
  void cooler();
  void light();
  void deact();
  void deactivate_all();
  void temp_hum_check();
    
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);


  Firebase.setString(firebaseData, "/FirebaseIOT/deact", "0");
  

}


void loop() {
  wifi_status();
  sensorUpdate();
  hum();
  fan();
  cooler();
  deact();
  light();
  temp_hum_check();


  
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

void wifi_status() {

  if(WiFi.status() != WL_CONNECTED) {
    while((WiFi.status() != WL_CONNECTED)) {
      digitalWrite(wifi_led,LOW);
      delay(2000);
      digitalWrite(wifi_led,HIGH);
      delay(2000);

    }
  }
  else {
    digitalWrite(wifi_led,HIGH);{

  }
}
}

void temp_hum_check(){

    if(dht.readHumidity()<30.00){
     Firebase.setString(firebaseData, "/FirebaseIOT/hum", "On");  
     Firebase.setString(firebaseData, "/ValueChange/hum", On);
  }
  if(dht.readHumidity()>80.00){
     Firebase.setString(firebaseData, "/FirebaseIOT/fan", "On");  
     Firebase.setString(firebaseData, "/ValueChange/fan", On); 
  }
  if(dht.readTemperature()>40.00){
     Firebase.setString(firebaseData, "/FirebaseIOT/cooler", "On");  
     Firebase.setString(firebaseData, "/ValueChange/cooler", On);
  }
  if(dht.readTemperature()<14.00){
     Firebase.setString(firebaseData, "/FirebaseIOT/cooler", "Off");  
     Firebase.setString(firebaseData, "/ValueChange/cooler", Off);
  }

}



void light(){

 Firebase.getString(firebaseData, "/ValueChange/light");
  

  if (firebaseData.dataType() == "string") {
    String stringValue = firebaseData.stringData();

    if (stringValue == Off) {

      digitalWrite(relay_Light,HIGH);
      delay(100);

    } else if(stringValue == On){

      digitalWrite(relay_Light,LOW);
      delay(100);

    }
  }
}


void fan(){

 Firebase.getString(firebaseData, "/ValueChange/fan");
  

  if (firebaseData.dataType() == "string") {
    String stringValue = firebaseData.stringData();

    if (stringValue == Off) {

      digitalWrite(relay_Fan,HIGH);
      delay(100);

    } else if(stringValue == On){

      digitalWrite(relay_Fan,LOW);
      delay(100);

    }
  }
}



void cooler(){

 Firebase.getString(firebaseData, "/ValueChange/cooler");
  

  if (firebaseData.dataType() == "string") {
    String stringValue = firebaseData.stringData();

    if (stringValue == On) {

      digitalWrite(relay_Cooler,LOW);
      delay(100);

    } else if(stringValue == Off){

      digitalWrite(relay_Cooler,HIGH);
      delay(100);

    }
  }
}



void hum(){

 Firebase.getString(firebaseData, "/ValueChange/hum");
  

  if (firebaseData.dataType() == "string") {
    String stringValue = firebaseData.stringData();

    if (stringValue == On) {

      digitalWrite(relay_Humidifier,LOW);
      delay(100);

    } else if(stringValue == Off){

      digitalWrite(relay_Humidifier,HIGH);
      delay(100);

    }
  }
}
 


void deact(){

 Firebase.getString(firebaseData, "/FirebaseIOT/deact");
  

  if (firebaseData.dataType() == "string") {
    String stringValue = firebaseData.stringData();

    if (stringValue == "1") {

      deactivate_all();

      delay(100);
      Firebase.setString(firebaseData, "/FirebaseIOT/deact", "0");

    } 
        
    }
  }


void deactivate_all(){ 

  Firebase.setString(firebaseData, "/ValueChange/light", Off);
  Firebase.setString(firebaseData, "/ValueChange/fan", Off);
  Firebase.setString(firebaseData, "/ValueChange/cooler", Off);
  Firebase.setString(firebaseData, "/ValueChange/hum", Off);

  Firebase.setString(firebaseData, "/FirebaseIOT/light", "Off");
  Firebase.setString(firebaseData, "/FirebaseIOT/fan", "Off");
  Firebase.setString(firebaseData, "/FirebaseIOT/cooler", "Off");
  Firebase.setString(firebaseData, "/FirebaseIOT/hum", "Off");
  
  delay(200);
}



