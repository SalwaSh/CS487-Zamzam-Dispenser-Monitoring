# Zamzam-Dispenser-Monitoring 🌙

## Summary 🌐
The problems that we aim to address are the **manual checking** and **hourly tours** required to monitor and replace Zamzam dispensers. This process **consumes time and manpower** that could be utilized more efficiently. This project aims to establish a comprehensive monitoring system for water levels in containers situated at **Al Haramain🌙, Al-Masajid Haram, and Al-Masajid a Nabawi**, with a specific focus on Zamzam water. 

## Project Requirements  📂

### 1. Hardware Requirements
- Ultrasonic Sensors to measure the water level. (at least one)
- LEDs as visual alert. (at least one)
- ESP8266 as receiver and senders for data. (at least two)
- Resistors. (at least one)

### 2. Software Requirements
- Cloud database (Firebase).
- Integrated Development Environment- IDE (Arduino).
- Web Technolgies (HTML, CSS, JS).

### 3. Communication Protocols
-  ESP NOW protocol.
-  HTTP protocol.

## How It Works ⚙️

Our solution utilizes ultrasonic sensors and ESP8266 devices attached to each container to accurately measure water levels. Then, the data is transmitted to a gateway and stored in a cloud-based database using ESP8266 devices. Each container also has an LED indicator that illuminates when the water level reaches a pre-set threshold, providing a visual alert. In addition, we have developed a web page dashboard for easy access to real-time water level updates, ensuring efficient and uninterrupted water supply management.

**System Architecture**

![image](https://github.com/SalwaSh/CS487-Zamzam-Dispenser-Monitoring/assets/97047182/2e6d437f-53b4-47b3-b70d-77a6f53fa8ef)

## Usage 📑
1. Clone the repository.
3. Navigate to Sensor.ino file on the Backend folder.
5. Reassign these variables: broadcastAddress, BOARD_ID, TRIG_PIN, ECHO_PIN, LED_PIN depend on your setup.
6. Upload the code to the first ESP8266 device.
8. Navigate to Gateway.ino file on the Backend folder.
9. Reassign these variables: WIFI_SSID and WIFI_PASSWORD depend on your setup.
10. Upload the code to the second ESP8266 device.
11. Create a new project in your Firebase account.
12. Create a new tabele inside the project.
13. Navigate to DB_connection.js file in Frontend folder.
14. Reassign the firebaseConfig variable depend on your setup.
15. Open home.html file on your browser.


## Demo 📹
[![video](https://github.com/SalwaSh/CS487-Zamzam-Dispenser-Monitoring/assets/97047182/e4f5e268-6585-4455-8de7-0c469e8dc2ed)](https://drive.google.com/file/d/1eTXR1kgr41IQh0Yt_Mk1I6aeLT0HsQwF/view?usp=sharing)


## Contributors ✍️

- Abeer Jeilani
- Salwa Shamma
- Sana Shmama
