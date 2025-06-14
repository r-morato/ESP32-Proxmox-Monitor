# Proxmox E-Ink Monitor

An ESP32-based real-time status monitor for your Proxmox node and LXCs using a low-power 2.9" 3-color E-Ink display.

---

## Features

- Connects to your Wi-Fi network to fetch Proxmox API data securely  
- Displays node status: CPU usage, memory usage, uptime, kernel version  
- Lists running LXCs on your Proxmox node  
- Alternates between status and running containers every 30 seconds  
- Low power consumption thanks to E-Ink display hibernation  
- Uses popular libraries: [GxEPD2](https://github.com/ZinggJM/GxEPD2), [ArduinoJson](https://arduinojson.org/)

---

## Hardware Components

- **ESP32 Dev Board**  
- **GxEPD2 2.9" 3-color E-Ink Display (Model: GxEPD2_290_C90c)**  
- Connecting pins:
  - CS: GPIO 5  
  - DC: GPIO 21  
  - RST: GPIO 2  
  - BUSY: GPIO 15  

---

## Software Dependencies

- [ESP32 Board Support Package](https://github.com/espressif/arduino-esp32) (via Arduino IDE Board Manager)  
- [GxEPD2](https://github.com/ZinggJM/GxEPD2) library  
- [ArduinoJson](https://arduinojson.org/) (version 6.x recommended)  
- WiFi and HTTPClient libraries included with ESP32 core

---

## Setup Instructions

1. **Clone or download** this repository.  
2. Open the `src/proxmox_monitor.ino` file in Arduino IDE or PlatformIO.  
3. Modify the following placeholders with your own values:
    ```cpp
    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASSWORD";
    const char* node = "YOUR_PROXMOX_NODE_NAME";
    const char* urlStatus = "https://YOUR_PROXMOX_IP:8006/api2/json/nodes/YOUR_NODE/status";
    const char* urlLXC = "https://YOUR_PROXMOX_IP:8006/api2/json/nodes/YOUR_NODE/lxc";
    const char* token = "PVEAPIToken=YOUR_API_TOKEN";
    ```
4. Install all required libraries via Arduino Library Manager or PlatformIO.  
5. Connect your ESP32 and E-Ink display per hardware spec.  
6. Compile and upload the sketch to your ESP32 board.  
7. Open Serial Monitor at 115200 baud to view connection and debug info.  

---

## Usage

- Upon boot, the device connects to your Wi-Fi network and fetches Proxmox node and container status.  
- The E-Ink display alternates every 30 seconds between:
  - Node system information screen  
  - Running LXCs list screen  
- Display hibernates between updates to save power.

---

## Images
<img width="488" alt="image" src="https://github.com/user-attachments/assets/bc8c48d3-f385-471e-8ea8-944a8f99c639" /><img width="387" alt="image" src="https://github.com/user-attachments/assets/1e4abc7f-0309-4970-aa5c-2c2231e549c5" />
<img width="601" alt="Screenshot 2025-06-14 at 20 53 09" src="https://github.com/user-attachments/assets/0935f6c9-9776-4837-a7ab-6589685505bd" />

---
## Troubleshooting

- Ensure your Proxmox API token has the necessary permissions for node status and LXC info.  
- Confirm your ESP32 board and libraries are up to date.  
- Check Wi-Fi credentials and network reachability.  
- Serial Monitor logs will help identify HTTP or JSON parsing errors.

---

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use and modify!

---

## Contributions

Contributions, issues, and feature requests are welcome!  
Please open a GitHub issue or pull request.

