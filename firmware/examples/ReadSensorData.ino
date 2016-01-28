// This #include statement was automatically added by the Particle IDE.
#include "I2CSensorParticle/I2CSensorParticle.h"

I2CSensorParticle sensor;

void setup() {
  Particle.publish("setup", "Start setup", 300, PRIVATE);
  Serial.begin(9600);
  
  sensor.begin(); // reset sensor
  Particle.publish("setup", "sensor.begin() done", 300, PRIVATE);
  delay(1000); // give some time to boot up
  String address = String(sensor.getAddress());
  String version = String(sensor.getVersion());
  
  // publish sensors info
  Particle.publish("setup", "I2C Soil Moisture Sensor Address: " + address, 300, PRIVATE);
  Particle.publish("setup", "Sensor Firmware version: " + version, 300, PRIVATE);
}

void loop() {
  float moisvalue = sensor.getCapacitance();
  float tempvalue = sensor.getTemperature()/(float)10;
  
  String s_moisvalue = String(moisvalue);
  String s_tempvalue = String(tempvalue);
  
  Particle.publish("dataStreamTest", "Moisture value: " + s_moisvalue + " - Temperature value: " + s_tempvalue, 300, PRIVATE);

  delay(1000);
}
