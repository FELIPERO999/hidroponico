#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// Credenciales WiFi
const char* ssid = "Felipe";      // Tu SSID
const char* password = "AAAAAAAAA";  // Tu contraseña WiFi

// API Key de ThingSpeak
const String apiKey = "1JU2MMS8HAPS0DA9"; // Tu API Key de ThingSpeak

// Dirección de ThingSpeak
const String server = "http://api.thingspeak.com/update";

// Inicialización de GPS
TinyGPSPlus gps;
HardwareSerial mygps(1);

// Configuración de la conexión WiFi
WiFiClient client;

void setup() {
  // Iniciar la comunicación Serial
  Serial.begin(115200);
  
  // Iniciar el puerto serial para el GPS
  mygps.begin(9600, SERIAL_8N1, 16, 17); // RX en 16 y TX en 17

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a WiFi");
}

void loop() {
  while (mygps.available() > 0) {
    gps.encode(mygps.read());
    
    if (gps.location.isUpdated()) {
      // Obtener las coordenadas GPS
      float lat = gps.location.lat();
      float lon = gps.location.lng();

      // Mostrar las coordenadas en el Serial Monitor
      Serial.print("Latitud: ");
      Serial.println(lat, 6);
      Serial.print("Longitud: ");
      Serial.println(lon, 6);

      // Crear la URL para enviar las coordenadas como Location (formato: latitud,longitud)
      String url = server + "?api_key=" + apiKey + "&location=" + String(lat, 6) + "," + String(lon, 6);
      
      // Enviar los datos a ThingSpeak
      if (client.connect("api.thingspeak.com", 80)) {
        client.println("GET " + url + " HTTP/1.1");
        client.println("Host: api.thingspeak.com");
        client.println("Connection: close");
        client.println();

        Serial.println("Datos enviados a ThingSpeak");
      } else {
        Serial.println("Error de conexión");
      }

      // Esperar 15 segundos antes de enviar los siguientes datos
      delay(15000);  // ThingSpeak limita la frecuencia a 15 segundos
    }
  }
}


