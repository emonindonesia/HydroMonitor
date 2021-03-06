/*
 * HydroMonitorTemperatureSensor.h
 *
 * (C) Wouter van Marle / City Hydroponics
 * www.cityhydroponics.hk
 */
 
#ifndef TEMPERATURESENSOR_h
#define TEMPERATURESENSOR_h

#include <HydroMonitorCore.h>
#include <HydroMonitorLogging.h>
#include <HydroMonitorSensorBase.h>

#ifdef USE_DHT22
#include <DHT22.h>
#endif
#ifdef USE_BMP180
#include <BMP180.h>
#elif defined(USE_BMP280) || defined(USE_BME280)
#include <BME280.h>
#endif

class HydroMonitorTemperatureSensor: public HydroMonitorSensorBase
{
  public:

    struct Settings {
    };

    // Constructor.
    HydroMonitorTemperatureSensor(void);
    
    // The functions required for all sensors.
#ifdef USE_DHT22
    void begin(HydroMonitorCore::SensorData*, HydroMonitorLogging *logging, DHT22*);   // Use the DHT22 sensor.
#endif
#ifdef USE_BMP180
    void begin(HydroMonitorCore::SensorData*, HydroMonitorLogging *logging, BMP180*);  // Use the BMP180 sensor.
#elif defined(USE_BMP280) || defined(USE_BME280)
    void begin(HydroMonitorCore::SensorData*, HydroMonitorLogging *logging, BME280*);  // Use the BMP280 or BME280 sensor.
#endif
    void readSensor(bool readNow = false);
    void dataHtml(ESP8266WebServer*);            // Provides html code with the sensor data.
    void settingsHtml(ESP8266WebServer*);
    bool settingsJSON(ESP8266WebServer*);
    void updateSettings(ESP8266WebServer*);

  private:
    Settings settings;
#ifdef USE_DHT22
    DHT22 *dht22;
#endif
#ifdef USE_BMP180
    BMP180 *bmp180;
#elif defined(USE_BMP280) || defined(USE_BME280)
    BME280 *bmp280;
#endif
    HydroMonitorCore core;
    HydroMonitorCore::SensorData *sensorData;
    HydroMonitorLogging *logging;
    uint32_t lastWarned;
};
#endif

