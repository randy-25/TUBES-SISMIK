#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "AB KOST 4G";
const char* password = "AB454545";

// For Car Control Testing
const int ledPin1 = 21; // GPIO for LED
const int ledPin2 = 22; 
const int ledPin3 = 23; 
const int ledPin4 = 25;      

// For Speed Testing
const int trigPin = 5;    // GPIO for ultrasonic TRIG
const int echoPin = 18;   // GPIO for ultrasonic ECHO

AsyncWebServer server(80);

void connectWifi(){
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print IP address
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());
}

float distance;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Wi-Fi Connection
  connectWifi();

  // Define HTTP GET handler for LED
  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request){
    String ledState = request->getParam("state")->value();

    if(ledState == "maju"){
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
    }else if(ledState == "kiri"){
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
    }else if(ledState == "kanan"){
        digitalWrite(ledPin3, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin4, LOW);
    }else if(ledState == "mundur"){
        digitalWrite(ledPin4, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin1, LOW);
    }else{
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
    }
    request->send(200, "text/plain", "LED is " + ledState);
  });

  // Define HTTP GET handler for ultrasonic distance
  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request){
    // test using ultrasonic sensor
    distance = getDistance();
    String response = "{\"speed\": " + String(distance, 2) + "}";  // Format with 2 decimal places
    request->send(200, "application/json", response);
  });

  server.begin();
}

float getDistance() {
  // Send a 10-microsecond pulse to the TRIG pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the ECHO pin pulse length
  long duration = pulseIn(echoPin, HIGH, 25000);  // timeout after 25ms
  if (duration == 0) {
    return -1;  // Return -1 if no echo was received
  }

  // Calculate the distance in cm
  float distanceFromSensor = (duration * 0.0343) / 2.0;  // Speed of sound at 343m/s divided by 2
  return distanceFromSensor;
}

void loop() {
  Serial.println(distance);
  delay(500);
  // Nothing here
}
