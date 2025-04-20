#include <WiFiNINA.h>
#include <WiFiSSLClient.h>
#include <ArduinoHttpClient.h>

// WiFi credentials
char ssid[] = "VX220-83A5";         
char pass[] = "3242C3D2040A0";     


char server[] = "trafficlight-control-default-rtdb.firebaseio.com";
int port = 443; 


String bluePath = "/leds/blue.json";
String greenPath = "/leds/green.json";
String redPath = "/leds/red.json";


int bluePin = 2;
int greenPin = 3;
int redPin = 4;


WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, server, port);

void setup() {
  Serial.begin(9600);

  // Set LED pins as OUTPUT
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }

  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP());
}

void loop() {
  checkAndSetLED(bluePath, bluePin);
  checkAndSetLED(greenPath, greenPin);
  checkAndSetLED(redPath, redPin);
  delay(1000);  
}

void checkAndSetLED(String path, int pin) {
  client.get(path);
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  response.trim();

  Serial.print("GET ");
  Serial.println(path);
  Serial.print("Status: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  if (statusCode == 200) {
    if (response == "true") {
      digitalWrite(pin, HIGH);
    } else {
      digitalWrite(pin, LOW);
    }
  } else {
    Serial.println("Failed to get data from Firebase");
  }
}
