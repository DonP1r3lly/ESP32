#include <WiFi.h>

const char* apSSID = "ESP32_AP";
const char* apPassword = "password123";

bool scanning = false;

struct NetworkInfo {
  String ssid;
  int rssi;
  int channel;
  int signalStrength;
  String signalStrengthClass;
};

NetworkInfo networks[20];
int numNetworks = 0;

void setup() {
  Serial.begin(115200);

  
  WiFi.softAP(apSSID, apPassword);

  Serial.println("Ap created");

  Serial.print("Address IP server: ");
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

void classifySignalStrength() {
  for (int i = 0; i < numNetworks; i++) {
    if (networks[i].rssi >= -50) {
      networks[i].signalStrengthClass = "Hard";
    } else if (networks[i].rssi >= -70) {
      networks[i].signalStrengthClass = "Mid";
    } else {
      networks[i].signalStrengthClass = "Low";
    }
  }
}

void scanNetworksAsync() {
  scanning = true;
  numNetworks = WiFi.scanNetworks();
  scanning = false;

  if (numNetworks == 0) {
    Serial.println("No Wifi Found");
  } else {
    Serial.println("WiFi Networks Classified by Signal Strength:");

    for (int i = 0; i < numNetworks; i++) {
      networks[i].ssid = WiFi.SSID(i);
      networks[i].rssi = WiFi.RSSI(i);
      networks[i].channel = WiFi.channel(i);
      networks[i].signalStrength = map(networks[i].rssi, -100, -40, 1, 10); 
    }

    for (int i = 0; i < numNetworks; i++) {
      for (int j = i + 1; j < numNetworks; j++) {
        if (networks[j].rssi > networks[i].rssi) {
          swapNetworks(i, j);
        }
      }
    }

    classifySignalStrength();
  }
}

void startServer() {
  WiFiServer server(80);

  server.begin();

  while (true) {
    WiFiClient client = server.available();

    if (client) {
      Serial.println("new client connected");

      String request = client.readStringUntil('\r');
      client.flush();

      if (request.indexOf("GET /scan") != -1) {
        scanNetworksAsync();
      }

      sendResponse(client);

      client.stop();
      Serial.println("disconect client");

      if (scanning) {
        WiFi.softAP(apSSID, apPassword);
        scanning = false;
      }
    }
  }
}

void sendResponse(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");

  client.println("<html><body>");
  client.println("<h2>WiFi Networks Classified by Signal Strength:</h2>");

  client.println("<table border='1'>");
  client.println("<tr><th>SSID</th><th>RSSI</th><th>Canal</th><th>Intensity</th><th>Class</th></tr>");

  for (int i = 0; i < numNetworks; i++) {
    client.print("<tr><td>");
    client.print(networks[i].ssid);
    client.print("</td><td>");
    client.print(networks[i].rssi);
    client.print("</td><td>");
    client.print(networks[i].channel);
    client.print("</td><td>");
    client.print(networks[i].signalStrength);
    client.print("</td><td>");
    client.print(networks[i].signalStrengthClass);
    client.println("</td></tr>");
  }

  client.println("</table>");

  client.println("<form action='/scan' method='get'><input type='submit' value='New Scan'></form>");

  client.println("</body></html>");
}
