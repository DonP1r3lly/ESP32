#include <WiFi.h>

const char* apSSID = "ESP32_AP";
const char* apPassword = "password123";

bool scanning = false;

struct NetworkInfo {
  String ssid;
  int rssi;
  int channel;
};

NetworkInfo networks[20]; 

void setup() {
  Serial.begin(115200);

  WiFi.softAP(apSSID, apPassword);

  Serial.println("Punto de acceso creado");

  Serial.print("Dirección IP del servidor: ");
  Serial.println(WiFi.softAPIP());

  startServer();
}

void loop() {
}

void swapNetworks(int i, int j) {
  NetworkInfo temp = networks[i];
  networks[i] = networks[j];
  networks[j] = temp;
}

void scanNetworks() {
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == 0) {
    Serial.println("No WiFi Found.");
  } else {
    Serial.println("WiFi Networks Classified by Signal Strength:");
    
    for (int i = 0; i < numNetworks; i++) {
      networks[i].ssid = WiFi.SSID(i);
      networks[i].rssi = WiFi.RSSI(i);
      networks[i].channel = WiFi.channel(i);
    }

    for (int i = 0; i < numNetworks; i++) {
      for (int j = i + 1; j < numNetworks; j++) {
        if (networks[j].rssi > networks[i].rssi) {
          // Intercambia las posiciones de las redes para ordenarlas
          swapNetworks(i, j);
        }
      }
    }

    for (int i = 0; i < numNetworks; i++) {
      Serial.print("SSID: ");
      Serial.print(networks[i].ssid);
      Serial.print(" - RSSI: ");
      Serial.print(networks[i].rssi);
      Serial.print(" - Canal: ");
      Serial.println(networks[i].channel);
    }
  }
}

void startServer() {
  WiFiServer server(80);

  server.begin();

  while (true) {
    WiFiClient client = server.available();
    
    if (client) {
      Serial.println("Nuevo cliente conectado");

      String request = client.readStringUntil('\r');
      client.flush();

      if (request.indexOf("GET /scan") != -1) {
        scanning = true;
      }

      sendResponse(client);

      client.stop();
      Serial.println("Cliente desconectado");
    }

    if (scanning) {
      scanNetworks();
      scanning = false;
    }
  }
}

void sendResponse(WiFiClient client) {
 
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");

  
  client.println("<html><body>");
  client.println("<h2>WiFi Networks Classified by Signal Strength:</h2>");
  

  client.println("<form action='/scan' method='get'><input type='submit' value='NEW SCAN'></form>");
  
  client.println("</body></html>");
}
