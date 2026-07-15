@echo off
title IoT Gateway Launcher
echo Starting Mosquitto broker...
start "Mosquitto Broker" cmd /k "cd /d "C:\Program Files\mosquitto" && mosquitto.exe -v -c mosquitto.conf"
REM Give Mosquitto a couple seconds to start before the Python script connects
timeout /t 10 /nobreak >nul
echo Starting Python MQTT subscriber...
start "MQTT Data Script" cmd /k "cd /d "C:\Users\EKRco\Desktop\Project V3" && python mqtt_data.py"
echo Both processes launched.
exit