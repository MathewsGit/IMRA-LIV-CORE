# IMRA-LIV+ | Intelligent Medical Robotic Assistant 🤖🚑

IMRA-LIV+ is a multi-tier robotic ecosystem designed for independent elderly care. It integrates **Computer Vision**, **Real-time IoT Telemetry**, and **Cloud Synchronization** to provide a safety net for individuals living alone.

---

## 🏗️ System Architecture

The system is distributed across three specialized processing layers:

### 1. Vision Layer (Python & OpenCV)
* **Face Tracking:** Utilizes `haarcascades` and the `MIL Tracker` to lock onto a user's face.
* **Proportional Control Logic:** Calculates the face's distance from the frame center to generate steering commands for the robotic base.

### 2. Control Layer (Arduino Mega)
* **Sensor Fusion:** Monitors DHT11 (Temp/Humidity), MQ-Series (Gas), and LDR (Ambient Light) simultaneously.
* **Human-Machine Interface (HMI):** Custom 16x2 LCD "eye" animations that change state based on system alerts.
* **Emergency Protocol:** Manages SMS/Calling via SIM800L GSM module when critical thresholds are breached.
* **Motor Drive:** L298N based differential drive system.

### 3. Connectivity Layer (ESP8266 & Firebase)
* **Cloud Sync:** Bridges local sensor data to a Firebase Real-Time Database.
* **Remote Command Execution:** Pulls "Intruder Alert" and "Manual Override" flags from the cloud to control the physical robot via mobile app.

---

## 🛠️ Tech Stack
* **Languages:** Python 3.x, C++ (Arduino/ESP Core)
* **Libraries:** OpenCV (cv2), Firebase-Arduino, SoftwareSerial, LiquidCrystal_I2C.
* **Hardware:** Arduino Mega 2560, ESP8266 (NodeMCU), SIM800L GSM, DHT11, MQ-8.

---

## 🚥 Safety Logic Flow
The system uses a priority-based warning system:
1. **Critical:** Gas Leak (`C`) or High Temp (`T`) -> Triggers `warn()` and Cloud Alert.
2. **Security:** Intrusion Detection flag received from Vision/Cloud -> Moves robot to alert state.
3. **Environmental:** Automated lighting adjustment based on ambient LDR values to prevent falls.

---

## 🔧 Installation & Setup

1. **Vision:** Run `python following.py` to start the vision tracking node.
2. **Firmware:** Flash `mega.ino` to the Arduino Mega and `esp_wifi_bridge.ino` to the NodeMCU.
3. **Database:** Configure your `FIREBASE_HOST` and `FIREBASE_AUTH` in the ESP8266 code.

---

## 👨‍💻 Author
**Mathews Joby**
*Computer Science Student @ Heriot-Watt | Robotics Champion*
