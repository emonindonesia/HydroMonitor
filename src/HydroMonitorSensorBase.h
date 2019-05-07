/*
 * HydroMonitorSensorBase.h
 *
 * (C) Wouter van Marle / City Hydroponics
 * www.cityhydroponics.hk
 */
 
#ifndef HYDROMONITORSENSORBASE_h
#define HYDROMONITORSENSORBASE_h

class HydroMonitorSensorBase
{
  public:
  virtual void readSensor(void);
  virtual void dataHtml(ESP8266WebServer*);
  virtual void settingsHtml(ESP8266WebServer*);
  virtual bool settingsJSON(ESP8266WebServer*); // return true if have anything, false if empty.
//  virtual void updateSettings(String[], String[], uint8_t);
  virtual void updateSettings(ESP8266WebServer*);
};
    
#endif