#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht11(DHTPIN, DHTTYPE);

const int BUZ = 5;  //buzzer to nodemcu D1 (GPIO-5)
const int LED = 4;  //LED to nodemcu D2 (GPIO-4)
int mq135 = A0;     //smoke sensor is connected with the analog pin A0
int data = 0;

void sensorData()
{
  data = analogRead(mq135);           //read A0 pin of NodeMCU
  //Blynk.virtualWrite(V2,data);        //virtual pin V2
  
  if (data > 610)                     //trigger buzzer if if smoke sensor value is greate than 610
  {
      tone(BUZ,5000);                   // Send 1KHz sound signal…
      digitalWrite(LED,HIGH);
      delay(1000);                      // …for 1 sec
  }
  else
  {
    digitalWrite(LED,LOW);
    digitalWrite(BUZ,LOW);
  }
}

#define FIREBASE_HOST "helllllo-3e954-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "3e6n9vzLsuJdyNKFjH72eBZzTKNdbM6QRxycm2EE"

#define WIFI_SSID "NgaiVaCham"
#define WIFI_PASSWORD "0868122609"

FirebaseData firebaseData;

void setup(){
  Serial.begin(115200);

  dht11.begin();
  WiFi.begin( WIFI_SSID,  WIFI_PASSWORD);
  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(400);
     timer.setInterval(1000L, sensorData);
   pinMode(BUZ, OUTPUT); // Set BUZ 
  pinMode(LED, OUTPUT); // Set LED 
  }
  Serial.println();
  Serial.print("ket noi voi IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
 
  }
  void loop(){
  timer.run();
    float t = dht11.readTemperature();
    float h= dht11.readHumidity();

    if( isnan(t) || isnan(h)|| isnan(data)){
        Serial.println("dht11 dau nay");
        delay(1000);
        return;
    }
      
    Serial.print("nhietdo");
    Serial.print(t);
    Serial.println();
    Serial.print("doam");
    Serial.print(h);
    Serial.println();
     Serial.print("track_kk");
    Serial.print(data);
    Serial.println();

    if ( Firebase.setFloat(firebaseData,"/nhietdo" ,t))
    {   Serial.println("ok ");
    }else {
      Serial.println("fail");
      Serial.println("VL :"+firebaseData.errorReason());
      }
    if ( Firebase.setFloat(firebaseData,"/doam" ,h))
    {   Serial.println("ok ");
    }else {
      Serial.println("fail");
      Serial.println("VL :"+firebaseData.errorReason());
      }
      if ( Firebase.setFloat(firebaseData,"/track_kk" ,data))
    {   Serial.println("ok ");
    }else {
      Serial.println("fail");
      Serial.println("VL :"+firebaseData.errorReason());
      }
      delay(1000);
      
    }  
