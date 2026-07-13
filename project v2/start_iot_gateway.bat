@echo off
title IoT Gateway Launcher

echo Starting Mosquitto broker...
start "Mosquitto Broker" cmd /k "cd /d "C:\Program Files\mosquitto" && mosquitto.exe -v -c mosquitto.conf"

REM Give Mosquitto a couple seconds to start before the Python script connects
timeout /t 3 /nobreak >nul

echo Starting Python MQTT subscriber...
start "MQTT Data Script" cmd /k "cd /d "C:\Users\EKRco\Desktop" && py mqtt_data.py"

REM Give the subscriber a moment to start writing to SQLite
timeout /t 2 /nobreak >nul

echo Starting FastAPI dashboard server...
start "FastAPI Dashboard" cmd /k "cd /d "C:\Users\EKRco\Desktop\your_project" && py -m uvicorn main:app --host 0.0.0.0 --port 8000"

REM Give the server a moment to come up before opening the browser
timeout /t 3 /nobreak >nul

echo Opening dashboard in browser...
start "" "http://localhost:8000"

echo All processes launched.
exit