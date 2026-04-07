---
title: BridgeGuard AI
emoji: 🌉
colorFrom: blue
colorTo: indigo
sdk: docker
pinned: false
app_port: 7860
---

# 🌉 BridgeGuard AI — Bridge Crack Detection System

An AI-powered bridge inspection system using **YOLOv8** + **ESP32-CAM** drone simulation.

## 🚀 Features
- **Real-Time Defect Detection** via drone/ESP32 camera feed
- **AI Classification** — Crack, Spalling, Corrosion, Exposed Rebar
- **3D Geospatial Visualization** of defect locations
- **PDF Report Generation** with severity analysis
- **Live Dashboard** with animated stats

## 🛠️ Tech Stack
- Python Flask, YOLOv8 (Ultralytics), OpenCV
- Chart.js, Three.js, Particles.js
- SQLite, FPDF, Matplotlib

## 📡 How It Works
1. ESP32-CAM / drone captures bridge images
2. YOLOv8 model detects & classifies structural defects
3. Results displayed on a premium dark-mode dashboard
4. PDF reports generated with GPS coordinates & severity levels
