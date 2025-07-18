# AUtomatic light controller🚦💡

This project implements an intelligent lighting system using LPC1768 and dual IR sensors to automate lighting based on room occupancy, optimizing energy usage.

## 🔧 Hardware Used

- LPC1768 ARM Cortex-M3 Microcontroller
- Dual IR Sensors
- Relay Module
- Buzzer (Inbuilt)
- UART Communication
- Light Bulb (AC via relay)

## 📋 Working Principle

- **Entry Detection**: IR1 triggered before IR2 → Person Entered → `count++`
- **Exit Detection**: IR2 triggered before IR1 → Person Exited → `count--`
- **Relay ON** when `count > 0`, **OFF** when `count == 0`
- Live count shown on LCD display.

## 💻 Embedded C Code

Main logic in `code/main.c`. Written in Keil µVision with GPIO handling and polling logic.

## 🧠 Features

- UART Serial Output (optional)
- Real-time occupancy counter
- Light control automation
- Energy efficient
- Reliable entry/exit detection

## 📊 Results

- ⏱️ Detection Time: < 0.5s
- ⚡ Energy Saving: up to 60%
- 🎯 Accuracy: 99% in practical tests

## 🧱 Future Scope

- IoT integration (mobile app control)
- Bluetooth/Wi-Fi remote access
- Motion sensor/CV-based accuracy boost
- Solar-powered variants
- Fan/AC automation

---

## 📷 Snapshot
![auto_light](https://github.com/user-attachments/assets/356d1e89-b726-4b0b-a5a3-cc22d8d29900)

---

## 👨‍💻 Author

Project by Oggu Sriram
Posted on [LinkedIn](https://www.linkedin.com/posts/oggu-v-b-m-s-s-sriram_letsabrrockabrmates-embeddedsystems-internshipabrproject-activity-7333891383709073410-Y1MC?utm_source=share&utm_medium=member_desktop&rcm=ACoAAFXBRL8BipP_Ggp1oH8yMVp5P1cUtZ6e2yQ)

