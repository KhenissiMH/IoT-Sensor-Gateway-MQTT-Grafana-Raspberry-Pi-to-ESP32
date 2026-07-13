import sqlite3
import time
import paho.mqtt.client as mqtt
from datetime import datetime

DB_PATH = r"C:\Users\EKRco\Desktop\data.db"
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
TOPIC_LIGHT = "sensors/esp32c6/light"
TOPIC_TEMP = "sensors/esp32c6/temp"
TOPIC_HUMIDITY = "sensors/esp32c6/humidity"

latest = {
    "temperature": None,
    "light": None,
    "humidity": None
}

conn = sqlite3.connect(DB_PATH)
cur = conn.cursor()
cur.execute('''CREATE TABLE IF NOT EXISTS data (
               id INTEGER PRIMARY KEY AUTOINCREMENT,
               timestamp TEXT,
               temperature REAL,
               light REAL,
               humidity REAL
               )''')
conn.commit()


def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker, rc:", rc)
    result, mid = client.subscribe("#")  # wildcard, catches everything for debugging
    print("Subscribe result:", result, "mid:", mid)


def on_message(client, userdata, msg):
    try:
        value = float(msg.payload.decode())
    except ValueError:
        print("Bad payload, could not convert to float:", msg.payload)
        return

    if msg.topic == TOPIC_LIGHT:
        latest["light"] = value
    elif msg.topic == TOPIC_TEMP:
        latest["temperature"] = value
    elif msg.topic == TOPIC_HUMIDITY:
        latest["humidity"] = value
    else:
        return  # unknown topic, ignore

    ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    conn.execute(
        "INSERT INTO data (timestamp, temperature, light, humidity) VALUES (?, ?, ?, ?)",
        (ts, latest["temperature"], latest["light"], latest["humidity"])
    )
    conn.commit()

    print(f"[{ts}] Inserted -> temp: {latest['temperature']} °C, "
          f"light: {latest['light']} lux, humidity: {latest['humidity']} %")


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(MQTT_BROKER, MQTT_PORT, 60)
client.loop_forever()