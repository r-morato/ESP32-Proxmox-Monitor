#include <WiFi.h> 
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMono9pt7b.h> 

// Wi-Fi credentials for connecting to the network
// Replace with your Wi-Fi credentials
const char* ssid = "WIFINAME";
const char* password = "WIFIPASSWORD!";

// Proxmox API details
const char* node = "PROXMOXNODE";
const char* urlStatus = "https://PROXMOXNODEIP:8006/api2/json/nodes/prox-prod-1/status";
const char* urlLXC = "https://PROXMOXNODEIP:8006/api2/json/nodes/prox-prod-1/lxc";
const char* token = "PVEAPIToken=apiuser@pPVETOKEN";

// E-Ink display setup (128x296)
GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT> display(GxEPD2_290_C90c(5, 21, 2, 15));

// Timing
unsigned long lastScreenChange = 0;
const unsigned long screenInterval = 30000; // 30 seconds
int screenIndex = 0;

// Cached data
float cpuUsage = 0;
String nodeStatus = "Unknown";
int memoryUsage = 0;
long uptime = 0;
String kernelVersion = "Unknown";
String runningLXCs[10];
int runningLXCCount = 0;  // Number of running LXCs
int totalLXCCount = 0;    // Total number of LXCs

void setup() {
  Serial.begin(115200);

  display.init(115200, true, 50, false);
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");

  fetchData();  // Fetch data initially
  showScreen0();  // Show the first screen with node info
  lastScreenChange = millis();  // Set the time for the first screen change
}

void loop() {
  if (millis() - lastScreenChange > screenInterval) {
    screenIndex = (screenIndex + 1) % 2;  // Toggle between screen 0 and screen 1
    fetchData();  // Refresh data before switching screens

    if (screenIndex == 0) {
      showScreen0();  // Show node status screen
    } else {
      showScreen1();  // Show running LXC list screen
    }

    lastScreenChange = millis();  // Update the last screen change timestamp
  }
}

// Fetch both node status and LXC info
void fetchData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Node status
    http.begin(urlStatus);
    http.addHeader("Authorization", token);
    int code = http.GET();
    if (code > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(8192);
      deserializeJson(doc, payload);

      cpuUsage = doc["data"]["cpu"] | 0.0;
      nodeStatus = (cpuUsage > 0.0) ? "Online" : "Idle";
      uint64_t memUsed = doc["data"]["memory"]["used"].as<uint64_t>();
      uint64_t memTotal = doc["data"]["memory"]["total"].as<uint64_t>();
      memoryUsage = (memUsed * 100) / max(memTotal, (uint64_t)1);
      uptime = (doc["data"]["uptime"] | 0) / 3600;
      kernelVersion = doc["data"]["current-kernel"]["release"] | "Unknown";
    } else {
      Serial.println("Failed to fetch node status.");
    }
    http.end();

    // LXC list
    http.begin(urlLXC);
    http.addHeader("Authorization", token);
    code = http.GET();
    if (code > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(16384);
      deserializeJson(doc, payload);

      JsonArray containers = doc["data"].as<JsonArray>();
      totalLXCCount = containers.size();  // Total number of LXCs
      runningLXCCount = 0;  // Reset running LXC count
      for (JsonObject container : containers) {
        String status = container["status"] | "stopped";
        if (status == "running" && runningLXCCount < 10) {
          runningLXCs[runningLXCCount++] = container["name"] | "Unknown";
        }
      }
    } else {
      Serial.println("Failed to fetch LXC list.");
    }
    http.end();
  }
}

// Main Status Screen
void showScreen0() {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);

    display.setCursor(10, 20);
    display.setFont(&FreeMonoBold9pt7b);
    display.print("Node: ");
    display.println(node);

    display.drawLine(5, 25, 291, 25, GxEPD_BLACK);

    display.setFont(&FreeMono9pt7b);
    display.setCursor(10, 45);
    display.print("Status: ");
    display.println(nodeStatus);

    display.setCursor(10, 65);
    display.print("CPU: ");
    display.print(cpuUsage * 100, 1);
    display.println("%");

    display.setCursor(10, 85);
    display.print("Memory: ");
    display.print(memoryUsage);
    display.println("%");

    display.setCursor(10, 105);
    display.print("Uptime: ");
    display.print(uptime);
    display.println(" hrs");

    display.setCursor(10, 125);
    display.print("LXCs Running: ");
    display.print(runningLXCCount);  // Display running LXCs
    display.print("/");
    display.println(totalLXCCount);  // Display total LXCs

  } while (display.nextPage());

  display.hibernate();
}

// Running LXC List Screen
void showScreen1() {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);

    display.setCursor(10, 20);
    display.setFont(&FreeMonoBold9pt7b);
    display.println("Running LXCs:");

    display.setFont(&FreeMono9pt7b);

    int xLeft = 10;
    int xRight = 150;
    int yBase = 45;
    int rowSpacing = 18;

    for (int i = 0; i < runningLXCCount && i < 10; i++) {
      int col = i / 5;
      int row = i % 5;

      int x = (col == 0) ? xLeft : xRight;
      int y = yBase + (row * rowSpacing);

      display.setCursor(x, y);
      display.println(runningLXCs[i]);
    }

  } while (display.nextPage());

  display.hibernate();
}
