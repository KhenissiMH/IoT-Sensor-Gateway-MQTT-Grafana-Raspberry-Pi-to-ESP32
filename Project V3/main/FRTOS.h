SemaphoreHandle_t mqttMutex;

TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;

void publishSensorTemperatureHumidity(void *parameter) {
  while(1) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    dtostrf(t, 4, 2, payload);

    if (xSemaphoreTake(mqttMutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
      if (client.connected()) {
        client.publish("sensors/esp32c6/temp", payload);
        Serial.print("[Thread Temperature&Humidity] Published Temperature: ");
        Serial.println(payload);
        dtostrf(h, 4, 2, payload);
        client.publish("sensors/esp32c6/humidity", payload);
        Serial.print("[Thread Temperature&Humidity] Published Humidity: ");
        Serial.println(payload);
      }
      xSemaphoreGive(mqttMutex);
    } 
    else {
      Serial.println("[Thread Temperature&Humidity] Could not acquire Mutex; skipping interval.");
    }

    vTaskDelay(pdMS_TO_TICKS(2000));    
   
  }
}

void publishSensorLight(void *parameter) {
  while(1) {
    float lux = lightMeter.readLightLevel();
    dtostrf(lux, 4, 2, payload);

    if (xSemaphoreTake(mqttMutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
      if (client.connected()) {
        client.publish("sensors/esp32c6/light", payload);
        Serial.print("[Thread Light Level] Published Light lvl: ");
        Serial.println(payload);
      }
      xSemaphoreGive(mqttMutex);
    } 
    else {
      Serial.println("[Thread Light Level] Could not acquire Mutex; skipping interval.");
    }

    vTaskDelay(pdMS_TO_TICKS(2000)); 
  }
}

void publish(){
  mqttMutex = xSemaphoreCreateMutex();
  
  if (mqttMutex == NULL) {
    Serial.println("Error creating Mutex!");

  }
  ConnectWifi();
  client.setServer(mqtt_server, mqtt_port);
  
  if (client.connect(clientId.c_str())) {
    xTaskCreatePinnedToCore(publishSensorTemperatureHumidity,"Temperature&HumidityTask",4096,NULL,1,&Task1Handle,0);
    xTaskCreatePinnedToCore(publishSensorLight,"LightTask",4096,NULL,1,&Task2Handle,0);
  }
}

void PL(){
  if (!client.connected()) {
    if (xSemaphoreTake(mqttMutex, portMAX_DELAY) == pdTRUE) {
      reconnect();
      xSemaphoreGive(mqttMutex);
      }
    }
  client.loop();
}