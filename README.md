<div align="center">

# 🔥 ESP32 Smart Pellet Bag Counter

<p align="center">
  <img src="./Preview/pellet_counter_cover.png" alt="Pellet Counter Cover" width="700" style="border-radius: 8px;" />
</p>

*A smart, automated inventory tracking system for pellet storage, powered by an ESP32, infrared distance sensors, and native Zigbee integration for Home Assistant.*

---

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![Author](https://img.shields.io/badge/author-Adrien%20Brune-orange.svg)
![Language](https://img.shields.io/badge/language-C%2F++-yellow.svg)
![Hardware](https://img.shields.io/badge/hardware-ESP32H2-red.svg)
![Protocol](https://img.shields.io/badge/protocol-Zigbee-blueviolet.svg)
![Platform](https://img.shields.io/badge/platform-Home%20Assistant%20%7C%20Z2M-lightgrey.svg)
![Status](https://img.shields.io/badge/status-active-success.svg)

</div>

## 💡 Introduction

Never run out of heating pellets again! The **Smart Pellet Bag Counter** is a DIY IoT solution designed to monitor your pellet reserve in real-time. 

Installed discreetly above your pellet storage area, the system uses precision infrared distance sensors to measure the height of the stack. Through a simple calibration process (combining total floor-to-sensor distance and individual bag thickness), the ESP32 calculates the exact number of remaining bags and transmits the live data straight to **Home Assistant** via the **Zigbee** protocol. 

The system is fully modular: you can easily add multiple sensors to monitor several storage stacks independently.

---

## ✨ Key Features

* **Real-Time Inventory:** Instantly know how many bags of pellets are left in your reserve directly from your Home Assistant dashboard.
* **Infrared Distance Sensing:** Non-contact measurement using reliable IR sensors mounted above the stacks.
* **Simple Mathematical Calibration:** Computes the remaining stock dynamically based on baseline stack height and individual bag dimensions.
* **Scalable / Multi-Stack Support:** Easily extensible architecture allowing additional sensors to be plugged in for multiple storage piles.
* **Native Zigbee Integration:** Seamless pairing with Home Assistant (Zigbee2MQTT) with low power consumption and reliable local communication.

---

## 🛠️ Technical Stack & Hardware

* **Microcontroller:** ESP32H2 (Wireless SoC)
* **Firmware Language:** C++
* **Communication Protocol:** Zigbee
* **Sensors:** Infrared distance sensors (IR modules)
* **Smart Home Platform:** Home Assistant

