# Hardware Components

This project uses an **ESP32 Dev Board** and a **2.9" 3-Color E-Ink Display** to show Proxmox system and LXC information over Wi-Fi.

Below is a list of all required components along with descriptions, wiring info, and example purchase links.

---

## 🔌 Required Components

### 1. ESP32 Dev Board
A Wi-Fi-enabled microcontroller used to connect to the Proxmox API and control the e-ink display.

- **Example**: ESP32-WROOM-32 DevKit v1 (or similar)
- **USB Powered**, programmable via Arduino IDE
- **Price**: ~£3-5

**🔗 [Buy on AliExpress](https://www.aliexpress.com/item/1005006336964908.html?spm=a2g0o.order_list.order_list_main.129.1bd71802lPRV7r)**

---

### 2. GxEPD2-Compatible E-Ink Display (2.9" 3-Color)
Used to display system status in black, white, and red/yellow.

- **Model**: GxEPD2_290_C90c  
- **Resolution**: 296 × 128  
- **Driver**: UC8151 (compatible with GxEPD2 library)  
- **Display Size**: 2.9 inches  
- **Colors**: Black / White / Red
- - **Price**: ~£5-10


**🔗 [Buy on AliExpress](https://www.aliexpress.com/item/1005004644515880.html?spm=a2g0o.order_list.order_list_main.134.1bd71802lPRV7r)**

---


## 🔧 Wiring Guide

| E-Ink Display Pin | Connects to ESP32 |
|------------------|-------------------|
| VCC              | 3.3V              |
| GND              | GND               |
| DIN (MOSI)       | GPIO 23           |
| CLK (SCK)        | GPIO 18           |
| CS               | GPIO 5            |
|
