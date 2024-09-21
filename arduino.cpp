#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverName = "http://api.thingspeak.com/update";
const String apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  myMHZ19.begin(mySerial);
  myMHZ19.autoCalibration();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int co2 = myMHZ19.getCO2();
  Serial.print("CO2 Concentration: ");
  Serial.print(co2);
  Serial.println(" ppm");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(serverName) + "?api_key=" + apiKey + "&field1=" + String(co2);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
        String payload = http.getString();
        Serial.println(payload);
    }
    http.end();
  }

  delay(2000);
}
