# 🎮 ZoneEye

<p align="center">

![ZoneEye Logo](docs/logo.png)

**Enterprise Gaming Center Management & Match Monitoring System**

*"Built for the next generation of PlayStation Gaming Centers."*

</p>

---

# 🚀 Overview

ZoneEye is an enterprise-grade monitoring and management platform designed for PlayStation gaming centers.

The system automatically monitors FIFA matches, tracks playing time, detects match states, records gameplay evidence, communicates with a central server, and provides accurate billing information.

Unlike traditional timer applications, ZoneEye uses Computer Vision (OpenCV) and OCR (Tesseract) to understand what is happening on the game screen.

---

# ✨ Features

## 🎯 Intelligent Match Detection

- Detect FIFA match timer
- Detect Kickoff
- Detect First Half
- Detect Halftime
- Detect Second Half
- Detect Full Time
- Detect Extra Time
- Detect Match End

---

## 🖥 Agent Software

Runs on every gaming station.

Features:

- Heartbeat system
- Screenshot capture
- OCR timer reading
- Automatic uploads
- Offline buffering
- Station identification
- Auto reconnect

---

## 🌐 Server

Central management server.

Features:

- Station registration
- Match logging
- Revenue calculation
- Screenshot storage
- Event history
- REST API

---

## 📊 Dashboard

Provides administrators with

- Active stations
- Online / Offline status
- Current matches
- Revenue
- Match history
- Screenshot review
- Reports

---

# 🧠 Technologies

| Technology | Purpose |
|------------|---------|
| C++17 | Agent |
| Go | Backend Server |
| SQLite | Local Database |
| OpenCV | Image Processing |
| Tesseract OCR | Timer Recognition |
| libcurl | HTTP Communication |
| CMake | Build System |
| vcpkg | Dependency Management |

---

# 📁 Project Structure

```
ZoneEye/
│
├── agent/
│   ├── src/
│   ├── include/
│   ├── build/
│   ├── CMakeLists.txt
│
├── backend/
│
├── frontend/
│
├── docs/
│
├── screenshots/
│
└── README.md
```

---

# ⚙ Current Development Status

## ✅ Completed

- Project Structure
- Agent Architecture
- Logger
- Configuration Loader
- Heartbeat Manager
- Upload Manager
- HTTP Client
- OpenCV Integration
- Tesseract Integration
- Timer Cropping
- Image Preprocessing
- OCR Pipeline
- CMake + vcpkg Integration

---

## 🚧 In Progress

- OCR Accuracy Improvement
- Live HDMI Capture
- FIFA Timer Detection
- Match State Machine

---

## 📅 Planned

- HDMI Capture Device Integration
- Automatic Match Detection
- Full Billing Engine
- Screenshot Compression
- Offline Synchronization
- Multi Gaming Center Support
- Cloud Dashboard
- Analytics
- Reporting
- Auto Updates

---

# 🔄 Agent Workflow

```
Game Screen

      │

      ▼

Screenshot Capture

      │

      ▼

Image Preprocessing

      │

      ▼

OCR Recognition

      │

      ▼

Match State Detection

      │

      ▼

Create Event

      │

      ▼

Upload to Server

      │

      ▼

Dashboard Update
```

---

# 🧩 OCR Pipeline

```
FIFA Screen

      │

      ▼

Crop Timer

      │

      ▼

Grayscale

      │

      ▼

Resize

      │

      ▼

Threshold

      │

      ▼

Tesseract OCR

      │

      ▼

Timer String

      │

      ▼

Match Logic
```

---

# 🛠 Build

Requirements

- Visual Studio 2022
- CMake
- vcpkg
- OpenCV
- Tesseract
- libcurl

Configure

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Build

```bash
cmake --build .
```

Run

```bash
.\Debug\ZoneEyeAgent.exe
```

---

# 📦 Dependencies

Installed using vcpkg.

```
opencv4
curl
tesseract
leptonica
```

---

# 📸 Current OCR Pipeline

```
Image

↓

Crop Timer

↓

Gray Scale

↓

Resize (4x)

↓

Threshold

↓

OCR

↓

Timer Text
```

---

# 🎯 Vision

Our goal is to create the world's most intelligent gaming center management system.

ZoneEye combines

- Computer Vision
- Artificial Intelligence
- OCR
- Event Detection
- Real-time Monitoring

to automate gaming center operations without modifying the PlayStation console.

---

# 👨‍💻 Author

**Abenezer Abadi**

Founder & Developer

ZoneEye Project

---

# 📄 License

This project is currently under private development.

Copyright © ZoneEye.

All Rights Reserved.

Unauthorized copying, distribution, or commercial use is prohibited.

---

# ⭐ Project Status

🚧 Active Development

Current Version

**ZoneEye Enterprise v1.0**

Every time you change C++ code
Step 1 — Open Developer PowerShell
