# 🌱 AgriShield — Active AI Crop Diagnostic Rover

![AgriShield Banner](Docs/architecture_diagram.png)

## 📌 Overview
**AgriShield** is a smart agricultural monitoring system designed to assist in the early identification of crop-health problems through **Artificial Intelligence, Embedded Systems, and Mobile Robotics**.

The system uses a custom-fabricated four-wheel rover equipped with an **ESP32-CAM** and multiple environmental sensors to inspect plants directly in the field. A **MobileNetV2 deep-learning model** screens captured leaf images for crop diseases, visible pest damage, and nutrient-stress symptoms. When a plant is identified as suspicious, the rover performs a secondary diagnostic scan (soil moisture, pH, temperature, humidity) to assess possible causes of stress.

The project achieved a **validation accuracy of 88.16%** on 4,585 images (correctly classifying 4,042 images) for tomato leaf disease detection.

---

## ✨ Key Features
- **AI Disease Detection:** MobileNetV2 (TensorFlow/Keras) classifies tomato leaf diseases with confidence scores.
- **Multimodal Sensing:** Soil moisture, soil pH, temperature, humidity, and optional NPK sensors.
- **Mobile Rover Platform:** Four-wheel DC-geared rover for field navigation.
- **Wireless Communication:** ESP32 Wi-Fi module transmits data to a Flask backend.
- **Web Dashboard:** Real-time display of crop images, predictions, sensor readings, and field risk maps.
- **Modular Design:** Easily extendable to new crops, pests, and sensors.

---

## 🛠️ Hardware Requirements
| Component | Purpose |
| :--- | :--- |
| **ESP32 Microcontroller** | Central controller for sensors, motors, and Wi-Fi |
| **ESP32-CAM** | Captures crop/leaf images for AI analysis |
| **Soil Moisture Sensor** | Measures soil water content |
| **Soil pH Sensor** | Measures soil acidity/alkalinity |
| **DHT22 (Temp & Humidity)** | Monitors environmental conditions |
| **L298N Motor Driver** | Controls DC geared motors |
| **DC Geared Motors (×4)** | Rover mobility |
| **12V Rechargeable Battery** | Power supply with buck converter |
| **Rover Chassis** | Mechanical platform |

---

## 💻 Software Requirements
| Tool | Purpose |
| :--- | :--- |
| **Arduino IDE** | ESP32 & ESP32-CAM programming |
| **Python 3.x** | AI model, backend, and dashboard |
| **TensorFlow / Keras** | MobileNetV2 model training and inference |
| **OpenCV / PIL** | Image preprocessing (resize to 224×224) |
| **Flask / FastAPI** | REST API backend for predictions |
| **HTML / CSS / JS** | Web dashboard frontend |
| **Google Colab** | Model training and evaluation |
| **Firebase (Optional)** | Cloud storage for logs and history |

---

🧠 System Architecture


The system consists of five interconnected modules:

Sensor Module — Soil moisture, pH, DHT22.

Image Acquisition — ESP32-CAM.

AI Processing — MobileNetV2 on server/PC.

Rover Mobility — ESP32 + L298N + DC motors.

Communication & Dashboard — Wi-Fi + Flask + Web UI.

https://docs/architecture_diagram.png

⚙️ Working Principle

Rover navigates the field via ESP32-controlled motors.

Sensors collect soil and environmental data.

ESP32-CAM captures leaf images.

Data is transmitted via Wi-Fi to a Flask backend.

MobileNetV2 classifies the image and returns the disease + confidence.

The dashboard displays results, alerts, and field risk maps.

🧪 Results

Model Validation Accuracy: 88.16% (4,042 / 4,585 images correctly classified).

Inference: Performed on a PC/server (ESP32 resource-limited).

Communication: HTTP/REST over Wi-Fi with JSON payloads.

Dashboard: Real-time display of predictions and sensor data.

⚠️ Further field testing under real agricultural conditions is required to evaluate the accuracy, reliability, and practical performance of the complete rover.

🚀 Future Scope

Support for additional crops (rice, wheat, maize, potato).

GPS-based field tracking and risk mapping.

Autonomous navigation and obstacle avoidance.

Mobile app for remote monitoring.

Cloud integration (Firebase) for historical logs.

Solar power integration for extended operation.

Treatment recommendations based on verified agricultural guidelines.

👨‍💻 Authors

A. Bharath (2411EC010061)

A. Raghuram (2411EC010062)

R. Pranay Sai (2411EC010091)

B. Akshay (2411EC010119)

Department of Electronics and Communication Engineering (ECE)
Malla Reddy University, Hyderabad

Project Guide: Dr. G. Anand Kumar (Head of the Department, ECE)

## 📂 Repository Structure
```text
agrishield-ai-crop-diagnostic-rover/
│
├── Hardware/
│   ├── Circuit_Diagram.png
│   ├── Rover_Assembly.jpg
│   └── Pin_Connections.md
│
├── ESP32_Firmware/               # Embedded C/C++ (Arduino)
│   ├── agrishield_main.ino       # Sensor + motor control + Wi-Fi
│   └── esp32_cam_capture.ino     # Image capture & upload
│
├── AI_Model/                     # Python / TensorFlow
│   ├── train_model.py            # MobileNetV2 training script
│   ├── predict.py                # Inference script
│   ├── agrishield_model.keras    # Trained model weights
│   └── class_names.json          # Disease class labels
│
├── Backend/                      # Flask REST API
│   └── app.py                    # /api/sensors, /api/predict, /api/status
│
├── Dashboard/                    # Web UI
│   └── index.html                # HTML + JS dashboard
│
├── Docs/                         # Report & Images
│   ├── architecture_diagram.png
│   ├── sequence_diagram.png
│   ├── activity_diagram.png
│   ├── rover_photo.jpg
│   └── results_summary.pdf
│
├── README.md
└── LICENSE
