# Garden-watering-assistant


🌱 Smart Garden Irrigation System

An automatic garden irrigation system based on the ESP32-C3 SuperMini. The project monitors soil moisture and air conditions and controls a normally closed 220V solenoid valve through a wireless relay.

Features

🌱 Monitoring soil moisture using HW-390 capacitive sensors

🌡️ Measuring air temperature and humidity with an AHT10 sensor

💧 Automatic irrigation based on soil moisture

⏱️ Maximum watering time protection

🛡️ Protection against continuous watering caused by a faulty sensor

📡 Wireless valve control using a 433 MHz transmitter

🔌 Control of a 220V normally closed solenoid valve through a CR-NAC100 relay

🔄 Automatic and manual operating modes

📱 Wi-Fi control and monitoring from a smartphone

📊 Display of soil and air conditions

☀️ Possibility of independent power supply from a solar panel and battery

Hardware

ESP32-C3 SuperMini — main controller

HW-390 — capacitive soil moisture sensors

AHT10 — temperature and air humidity sensor

433 MHz transmitter — wireless communication

CR-NAC100 — wireless relay module

220V NC solenoid valve — controls the water supply

4N35 optocoupler — electrical isolation where required

Water tank and pump (optional)

Basic Architecture

        ┌──────────────────────┐
        │      ESP32-C3        │
        │     SuperMini        │
        └──────────┬───────────┘
                   │
        ┌──────────┼───────────┐
        │          │           │
        ▼          ▼           ▼
     HW-390      AHT10      433 MHz TX
    Soil Sensors   Temp/       │
                  Humidity     ▼
                         ┌───────────┐
                         │ CR-NAC100 │
                         └─────┬─────┘
                               │
                             220V
                               │
                               ▼
                     ┌─────────────────┐
                     │ NC Solenoid     │
                     │     Valve       │
                     └────────┬────────┘
                              │
                              ▼
                           💧 Water
 Current GPIO Configuration
 
 Component	ESP32-C3 GPIO
 
 HW-390 #1-------GPIO4
 
 HW-390 #2-------GPIO5
 
 HW-390 #3-------GPIO6
 
 AHT10 SDA-------GPIO8
 
 AHT10 SCL-------GPIO9
 
 433 MHz TX------GPIO10
 
 Project Status
 

🚧 Work in progress

The project is currently in the testing and prototyping stage. The first step is testing the ESP32-C3 and soil moisture sensors, followed by AHT10, 433 MHz communication, and finally the irrigation valve control.

Safety

The irrigation valve operates from 220V AC. The high-voltage section must be electrically isolated from the ESP32 low-voltage electronics. All mains wiring should be performed with the power disconnected and using appropriate electrical protection.

Goal: Create a reliable, autonomous and remotely controllable irrigation system that waters plants only when necessary while reducing water consumption and protecting the system from sensor failures.


Excalidraw: https://excalidraw.com/#json=Sd81TdtqGX3Bt-XbbjyrT,U83_Y1ocFmggsrylD8pWHg
