#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>

#define WIFI_SSID       "????????"
#define WIFI_PASSWORD   "????????"
#define FIREBASE_HOST "????????"
#define FIREBASE_AUTH "????????"

const int ledPin = 14; //D5 pin esp2866
int timezone = 7;
String inputString = "";         // a string to hold incoming data
char ntp_server1[20] = "ntp.ku.ac.th";
char ntp_server2[20] = "fw.eng.ku.ac.th";
char ntp_server3[20] = "time.uni.net.th";
int dst = 0;
String v,i;
boolean stringComplete = false;  // whether the string is complete
int Control;

//------------ define function --------------------
//void serialEvent();
String NowString() ;
//------------------------------------------------

void setup() 
{

  Serial.begin(19200);
  Serial.println(WiFi.localIP());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  pinMode(ledPin, OUTPUT);

  configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
  Serial.println("Waiting for time");
  while (!time(nullptr)) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Now: " + NowString());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop() {
 // Control=Firebase.getInt("Power/Control");

    digitalWrite(ledPin, Firebase.getInt("Power/Control"));

  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') 
    {
      stringComplete = true;
    }
  }
  v=inputString.substring(inputString.indexOf('V')+1, inputString.indexOf('I'));
  i=inputString.substring(inputString.indexOf('I')+1, inputString.indexOf('A'));
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["voltage"] = v;
    root["current"] = i;
    root["time"] = NowString();
   String name = Firebase.push("Power/data", root);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
    // clear the string:
    inputString = "";
    stringComplete = false;
   // inChar = '';
  delay(5000);
}

//void serialEvent() 
//{
//  while (inChar != 'A') 
//  {
//    inChar = (char)Serial.read();
//    inputString += inChar;    
//  }
//}

String NowString() {
  time_t now = time(nullptr);
  struct tm* newtime = localtime(&now);

  String tmpNow = "";
  tmpNow += String(newtime->tm_year + 1900);
  tmpNow += "-";
  tmpNow += String(newtime->tm_mon + 1);
  tmpNow += "-";
  tmpNow += String(newtime->tm_mday);
  tmpNow += " ";
  tmpNow += String(newtime->tm_hour);
  tmpNow += ":";
  tmpNow += String(newtime->tm_min);
  tmpNow += ":";
  tmpNow += String(newtime->tm_sec);
  return tmpNow;
}

