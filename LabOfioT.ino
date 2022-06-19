3:18 PM 6/19/20223:18 PM 6/19/2022#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h" // Install Firebase ESP8266 library
#include <DHT.h> // Install DHT11 Library and Adafruit Unified Sensor Library#define FIREBASE_HOST "fir-iottemp-3dd53-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "vm5tn5cRLmv7x1fuOrkNwm56In64AXkAinPHSvxk"
//#define WIFI_SSID "KaranchiNoOne"
//#define WIFI_PASSWORD "fahad123"
#define WIFI_SSID "Vodafone-A81615835"
#define WIFI_PASSWORD "KHcdTGcRAJEqxsyP"
#define DHTPIN 2 // Connect Data pin of DHT to D2
int led = D5; // Connect LED to D5#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;FirebaseJson json;// Update these with values suitable for your network.
const char* ssid = "KaranchiNoOne";
//const char* password = "1234";const char* ssid = "Vodafone-A81615835";
const char* password = "1234";
const char* mqtt_server = "192.168.1.12";
const char* mqtt_user = "guest";
const char* mqtt_password = "guest";
WiFiClient espClient;
PubSubClient client(espClient);long lastMsg = 0;
char msg[550];
int value = 0;void sensorUpdate(){
// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
// Read temperature as Fahrenheit (isFahrenheit = true)
float f = dht.readTemperature(true); // Check if any reads failed and exit early (to try again)
if (isnan(h) || isnan(t) || isnan(f)) {
Serial.println(F("Failed to read from DHT sensor!"));
return;
} Serial.print(F("Humidity: "));
Serial.print(h);
Serial.print(F("% Temperature: "));
Serial.print(t);
Serial.print(F("°C ,"));
Serial.print(f);
Serial.println(F("°F ")); if (Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", t))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
} if (Firebase.setFloat(firebaseData, "/FirebaseIOT/humidity", h))
{
Serial.println("PASSED");
Serial.println("PATH: " + firebaseData.dataPath());
Serial.println("TYPE: " + firebaseData.dataType());
Serial.println("ETag: " + firebaseData.ETag());
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + firebaseData.errorReason());
Serial.println("------------------------------------");
Serial.println();
}
}void setup_wifi() {
delay(100);
// We start by connecting to a WiFi network
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
randomSeed(micros());
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}void reconnect() {
// Loop until we're reconnected
while (!client.connected())
{
Serial.print("Attempting MQTT connection...");
// Create a random client ID
String clientId = "ESP8266Client-";
clientId += String(random(0xffff), HEX);
if (client.connect(mqtt_password))
{
Serial.println("connected");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");
// Wait 6 seconds before retrying
delay(6000);
}
}
} //end reconnect()
void setup()
{
Serial.begin(115200);
setup_wifi();
client.setServer(mqtt_server,1883);
dht.begin();
pinMode(led,OUTPUT);
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.reconnectWiFi(true);
}
void loop()
{
if (!client.connected()) {
reconnect();
sensorUpdate();
if (Firebase.getString(ledData, "/FirebaseIOT/led")){
Serial.println(ledData.stringData());
if (ledData.stringData() == "1") {
digitalWrite(led, HIGH);
}
else if (ledData.stringData() == "0"){
digitalWrite(led, LOW);
}
}
delay(100);
}
}
