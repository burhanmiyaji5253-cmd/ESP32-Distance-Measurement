#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

// Ultrasonic sensor pins
const int trigPin = 5;  // GPIO5
const int echoPin = 18; // GPIO18

// Web server running on port 80
WebServer server(80);

// Function to measure distance
long readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

// Serve the main page
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>Distance Measurement</title>"
                "<style>"
                "body { background-color: #f2f2f2; font-family: Arial, sans-serif; text-align: center; padding: 40px 20px; }"
                "h1 { font-size: 38px; color: #2c3e50; margin-bottom: 20px; }"
                "p { font-size: 32px; color: #34495e; margin-bottom: 60px; }"
                ".watermark { position: fixed; bottom: 60px; width: 100%; text-align: center; font-size: 14px; color: #777; }"
                "@media (max-width: 600px) {"
                "h1 { font-size: 32px; }"
                "p { font-size: 26px; }"
                ".watermark { font-size: 12px; }"
                "}"
                "</style>"
                "<script>"
                "function fetchDistance() {"
                "  fetch('/distance')"
                "    .then(response => response.text())"
                "    .then(data => {"
                "      document.getElementById('distance').innerHTML = data + ' cm';"
                "    });"
                "}"
                "setInterval(fetchDistance, 1000);" // Fetch every 1 second
                "</script>"
                "</head><body>"
                "<h1>Distance Measurement Tool</h1>"
                "<p id='distance'>Loading...</p>"
                "<div class='watermark'>Created by Burhan Miyaji</div>"
                "</body></html>";
  server.send(200, "text/html", html);
}

// Serve just the distance
void handleDistance() {
  long distance = readDistanceCM();
  server.send(200, "text/plain", String(distance));
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start web server
  server.on("/", handleRoot);
  server.on("/distance", handleDistance);
  server.begin();
}

void loop() {
  server.handleClient();
}
