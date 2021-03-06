/*----------------------------------------------------------------------*
 * I2CSensorParticle.cpp - Particle library for the Sensor version of*
 * I2C Soil Moisture Sensor version from Chrirp                         *
 * (https://github.com/Miceuz/i2c-moisture-sensor).                     *
 *                                                                      *
 * Jibsgrl 18Jan2016                                                    *
 * https://github.com/jbgariel                                          *
 *                                                                      *
 * Originaly from :                                                     *
 * Ingo Fischer 11Nov2015                                               *
 * https://github.com/Apollon77/I2CSoilMoistureSensor                   *
 *                                                                      *
 * MIT license                                                          *
 *----------------------------------------------------------------------*/ 

#include "I2CSensorParticle.h"

/*----------------------------------------------------------------------*
 * Constructor.                                                         *
 * Optionally set sensor I2C address if different from default          *
 *----------------------------------------------------------------------*/
I2CSensorParticle::I2CSensorParticle(uint8_t addr) : sensorAddress(addr) {
  // nothing to do ... Wire.begin needs to be put outside of class
}
  
/*----------------------------------------------------------------------*
 * Initializes anything ... it does a reset only at the moment          *
 *----------------------------------------------------------------------*/
void I2CSensorParticle::begin() {
  Serial.println("Start begin()");
  resetSensor();
}

/*----------------------------------------------------------------------*
 * Return measured Soil Moisture Capacitance                            *
 * Moisture is somewhat linear. More moisture will give you higher      *
 * reading. Normally all sensors give about 290 - 310 as value in free  * 
 * air at 5V supply.                                                    *
 *----------------------------------------------------------------------*/
unsigned int I2CSensorParticle::getCapacitance() {
  return readI2CRegister16bitUnsigned(sensorAddress, SOILMOISTURESENSOR_GET_CAPACITANCE);
}

/*----------------------------------------------------------------------*
 * Change I2C address of the sensor to the provided address (1..127)    *
 * and do a reset after it in order for the new address to become       *
 * effective if second parameter is true.                               *
 * Method returns true if the new address is set successfully on sensor.*
 *----------------------------------------------------------------------*/
bool I2CSensorParticle::setAddress(int addr, bool reset) {
  writeI2CRegister8bit(sensorAddress, SOILMOISTURESENSOR_SET_ADDRESS, addr);
  if (reset) {
    resetSensor();
    delay(1000);
  }
  sensorAddress=addr;
  return (readI2CRegister8bit(sensorAddress, SOILMOISTURESENSOR_GET_ADDRESS) == addr);
}

/*----------------------------------------------------------------------*
 * Return current Address of the Sensor                                 *
 *----------------------------------------------------------------------*/
uint8_t I2CSensorParticle::getAddress() {
  return sensorAddress;
}

/*----------------------------------------------------------------------*
 * Starts the measurement for the Light sensor. Wait at least 3 seconds *
 * till you call method getLight to get the Light value.                *
 *----------------------------------------------------------------------*/
void I2CSensorParticle::startMeasureLight() {
  writeI2CRegister8bit(sensorAddress, SOILMOISTURESENSOR_MEASURE_LIGHT);
}

/*----------------------------------------------------------------------*
 * Read the Light Measurement from the sensor. When used without        *
 * parameter or parameter value is false then a former call to          *
 * method startMeasureLight and a waiting time of at least 3 seconds is *
 * expected.                                                            *
 * Alternatively use true as parameter and the method does the call to  *
 * startMeasureLight and a 3 seconds delay automatically and no former  *
 * call is needed.                                                      *
 * The measurement gives 65535 in a dark room away form desk lamp - so  *
 * more light, lower reading. When it's dark, it takes longer to        *
 * measure light, reading the light register while measurement is in    *
 * progress (e.g. wait time too short) will return the previous reading.*
 * Be aware, light sensor is pretty noisy.                              *
 *----------------------------------------------------------------------*/
unsigned int I2CSensorParticle::getLight(bool wait) {
  if (wait) {
    startMeasureLight();
    delay(3000);
  }
  return readI2CRegister16bitUnsigned(sensorAddress, SOILMOISTURESENSOR_GET_LIGHT);
}

/*----------------------------------------------------------------------*
 * Read the Temperature Measurement. Temperature is measured by the     *
 * thermistor on the tip of the sensor. Calculated absolute measurement *
 * accuracy is better than 2%. The returned value is in degrees Celsius *
 * with factor 10, so need to divide by 10 to get real value            *
 *----------------------------------------------------------------------*/
int I2CSensorParticle::getTemperature() {
  return readI2CRegister16bitSigned(sensorAddress, SOILMOISTURESENSOR_GET_TEMPERATURE);
}

/*----------------------------------------------------------------------*
 * Resets sensor. Give the sensor 0.5-1 second time to boot up after    *
 * reset.                                                               *
 *----------------------------------------------------------------------*/
void I2CSensorParticle::resetSensor() {
  writeI2CRegister8bit(sensorAddress, SOILMOISTURESENSOR_RESET);
}

/*----------------------------------------------------------------------*
 * Get Firmware Version. 0x22 means 2.2                                  *
 *----------------------------------------------------------------------*/
uint8_t I2CSensorParticle::getVersion() {
  return readI2CRegister8bit(sensorAddress, SOILMOISTURESENSOR_GET_VERSION);
}


/*----------------------------------------------------------------------*
 * Helper method to write an 8 bit value to the sensor via I2C          *
 *----------------------------------------------------------------------*/
void I2CSensorParticle::writeI2CRegister8bit(int addr, int value) {
  Wire.beginTransmission(addr);
  Wire.write(value);
  Wire.endTransmission();
}

/*----------------------------------------------------------------------*
 * Helper method to write an 8 bit value to the sensor via I2C to the   *
 * given register                                                       *
 *----------------------------------------------------------------------*/
void I2CSensorParticle::writeI2CRegister8bit(int addr, int reg, int value) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

/*----------------------------------------------------------------------*
 * Helper method to read a 16 bit unsigned value from the given register*
 *----------------------------------------------------------------------*/
unsigned int I2CSensorParticle::readI2CRegister16bitUnsigned(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(20);
  Wire.requestFrom(addr, 2);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

/*----------------------------------------------------------------------*
 * Helper method to read a 16 bit signed value from the given register*
 *----------------------------------------------------------------------*/
int I2CSensorParticle::readI2CRegister16bitSigned(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(20);
  Wire.requestFrom(addr, 2);
  int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

/*----------------------------------------------------------------------*
 * Helper method to read a 8 bit value from the given register          *
 *----------------------------------------------------------------------*/
uint8_t I2CSensorParticle::readI2CRegister8bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(20);
  Wire.requestFrom(addr, 1);
  return Wire.read();
}
