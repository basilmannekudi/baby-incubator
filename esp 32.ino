#include <WiFiManager.h>
#include <FirebaseESP32.h>
String data;
String FIREBASE_HOST = "https://incubator-7b923-default-rtdb.firebaseio.com/";
String FIREBASE_AUTH = "8gLgxr6i470C8r4c84X1glq4WpPCPIsrivpwOtTf";

FirebaseData firebaseData;
FirebaseJson json;
#define RXp2 16
#define TXp2 17
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);
 WiFiManager wm;
bool res;
res = wm.autoConnect("Incubator");

// anonymous ap
if(!res) {
    Serial.println("Failed to connect");
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
          Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
    }
}

void loop() {
   

if (Serial2.available() > 0) { //Check if data is available to read
    data = Serial2.readStringUntil('\n'); //Read data from serial port
    //Serial.println(data);
    
    int commaIndex1 = data.indexOf(','); //Find first comma
    int commaIndex2 = data.indexOf(',', commaIndex1 + 1); 
    int commaIndex3 = data.indexOf(',', commaIndex2 + 1); 
    String T = data.substring(0, commaIndex1); //Extract first data
    String H = data.substring(commaIndex1 + 1, commaIndex2); 
    int B = data.substring(commaIndex2 + 1, commaIndex3).toInt();//Extract second data
    int O = data.substring(commaIndex3+1).toInt();


  json.set("/isPoweredOn", "true");
  Firebase.updateNode(firebaseData,"/data",json);
  json.set("/bp", B);
  Firebase.updateNode(firebaseData,"/data",json);
    json.set("/spo2", O);
    
  Firebase.updateNode(firebaseData,"/data",json);
    json.set("/temp", T);
  Firebase.updateNode(firebaseData,"/data",json);
    json.set("/humid", H);
  Firebase.updateNode(firebaseData,"/data",json);
   
   
  }
  

}
