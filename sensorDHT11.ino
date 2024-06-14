#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>  
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* serverAddress = "http://192.168.1.17:5000/data";

#define DHTPIN 4      // Pin tempat sensor DHT22 terhubung
#define DHTTYPE DHT22 // Jenis sensor DHT yang digunakan

DHT dht(DHTPIN, DHTTYPE);

float getTemperature() {
  float suhu = dht.readTemperature(); // Membaca suhu dari sensor
  Serial.print("suhu: ");
  Serial.println(suhu);
  return suhu;
}

float getHumidity() {
  float kelembaban = dht.readHumidity(); // Membaca kelembaban dari sensor
  Serial.print("kelembaban: ");
  Serial.println(kelembaban);
  return kelembaban;
}

void postSensorData(){
  WiFiClient client;
  HTTPClient http;

  float suhu = getTemperature();
  float kelembaban = getHumidity();

  http.begin(client, serverAddress);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"suhu\": " + String(suhu) + ", \"kelembaban\": " + String(kelembaban) + "}";

  Serial.println("Sending request to: " + String(serverAddress));
  Serial.println("Payload: " + payload);

  int httpResponseCode = http.POST(payload);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  http.end();

  delay(5000);
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);

  dht.begin();

  // Cek status koneksi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Tampilkan pesan sukses
  Serial.println("");
  Serial.println("WiFi connected!");
}

void loop() {
  postSensorData();
}
