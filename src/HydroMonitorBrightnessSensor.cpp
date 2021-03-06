#include <HydroMonitorBrightnessSensor.h>

#ifdef USE_BRIGHTNESS_SENSOR
/*
   Measure the ambient light brightness.
*/
HydroMonitorBrightnessSensor::HydroMonitorBrightnessSensor () {
  brightnessSensorPresent = false;
}

/*
   Configure the sensor.
*/
void HydroMonitorBrightnessSensor::begin(HydroMonitorCore::SensorData *sd, HydroMonitorLogging *l) {

#if defined(USE_TSL2561)
  tsl = TSL2561(TSL2561_ADDR_FLOAT, 12345);
  l->writeTrace(F("HydroMonitorBrightnessSensor: configured TSL2561 sensor."));
#elif defined(USE_TSL2591)
  tsl = TSL2591();
  l->writeTrace(F("HydroMonitorBrightnessSensor: configured TSL2591 sensor."));
#endif

  logging = l;
  sensorData = sd;
  if (BRIGHTNESS_SENSOR_EEPROM > 0) {
    EEPROM.get(BRIGHTNESS_SENSOR_EEPROM, settings);
  }
}

/*
   Take a measurement from the sensor.

   Returns the current lux value, or -1 if sensor is not found.
*/
void HydroMonitorBrightnessSensor::readSensor(bool readNow) {
  static uint32_t lastReadSensor = -REFRESH_SENSORS;
  if (millis() - lastReadSensor > REFRESH_SENSORS ||
      readNow) {

    // Check whether the sensor is connected and initialised.
    if (!brightnessSensorPresent) {

      // Try whether we have one now, it never hurts to check.
      brightnessSensorPresent = tsl.begin();

#if defined(USE_TSL2561)
      // If we just detected a sensor, set it up.
      if (brightnessSensorPresent) {
        tsl.enableAutoRange(true);
        tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS); // medium fast and medium resolution - good for us
      }
#endif
    }
    if (brightnessSensorPresent) {

#if defined(USE_TSL2561)
      sensors_event_t event;
      tsl.getEvent(&event);       // Get sensor event.
      sensorData->brightness = event.light;   // Read the current value from the sensor.
      if (sensorData->brightness == 65536) {  // The sensor is either saturated or has been disconnected.
        brightnessSensorPresent = tsl.begin(); // this should return false if not connected.
        if (!brightnessSensorPresent) sensorData->brightness = -1;
      }
#elif defined(USE_TSL2591)
      sensorData->brightness = tsl.readSensor();
#endif
    }
  }
}

/*
   The html code for the sensor specific settings.
*/
void HydroMonitorBrightnessSensor::settingsHtml(ESP8266WebServer *server) {
}

/*
   The settings as JSON.
*/
bool HydroMonitorBrightnessSensor::settingsJSON(ESP8266WebServer *server) {
  return false;
}

/*
   The html code for the sensor data.
*/
void HydroMonitorBrightnessSensor::dataHtml(ESP8266WebServer *server) {
  server->sendContent_P(PSTR("<tr>\n\
    <td>Brightness</td>\n\
    <td>"));
  if (sensorData->brightness < 0) {
    server->sendContent_P(PSTR("Sensor not connected.</td>\n\
  </tr>"));
  }
  else {
    char buff[12];
    server->sendContent(ultoa(sensorData->brightness, buff, 10));
    server->sendContent_P(PSTR(" lux.</td>\n\
  </tr>"));
  }
}

/*
   Update the settings for this sensor, if any.
*/
void HydroMonitorBrightnessSensor::updateSettings(ESP8266WebServer *server) {
}
#endif

