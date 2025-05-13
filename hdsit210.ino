#include <WiFiNINA.h>
#include <WiFiSSLClient.h>
#include <ArduinoHttpClient.h>

// WiFi 
char ssid[] = "VX220-83A5";
char pass[] = "3242C3D2040A0";

// Firebase 
char server[] = "trafficlight-control-default-rtdb.firebaseio.com";
int port = 443;
String commandPath = "/command.json";

// LED pins (blue, green, red)
int bluePin = 2;
int greenPin = 3;
int redPin = 4;

WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, server, port);

// Callback 
typedef void (*CommandCallback)(String);

// Callback function
void handleCommand(String command) {
  if (command == "blue") {
    digitalWrite(bluePin, !digitalRead(bluePin));
    Serial.println("Toggled BLUE");
  } else if (command == "green") {
    digitalWrite(greenPin, !digitalRead(greenPin));
    Serial.println("Toggled GREEN");
  } else if (command == "red") {
    digitalWrite(redPin, !digitalRead(redPin));
    Serial.println("Toggled RED");
  } else {
    Serial.println("Unknown command");
  }

  // Reset Firebase command to "none"
  client.beginRequest();
  client.put(commandPath);
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", 6); // "none"
  client.beginBody();
  client.print("\"none\"");
  client.endRequest();
}

// Function to check Firebase and callback
void checkFirebase(CommandCallback callback) {
  client.get(commandPath);
  int statusCode = client.responseStatusCode();
  String command = client.responseBody();
  command.trim();
  command.replace("\"", "");

  if (statusCode == 200 && command != "none") {
    Serial.print("Command received: ");
    Serial.println(command);
    callback(command);  // <<== CALLBACK IS USED HERE
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }

  Serial.println("Connected to WiFi!");
}

void loop() {
  checkFirebase(handleCommand); // This will be used to Pass the callback function
  delay(1000);
}
