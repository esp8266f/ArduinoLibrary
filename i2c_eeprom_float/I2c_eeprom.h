#include <Wire.h>

/*
	Author: Andriy Kasandyuk
	Made in Ukraine
	Year: 2021
	Version: 1.0
	FOR PERSONAL USE
*/

	/*
	description:
	
	Arduino class for write data to external eeprom in i2c bus (zs-042 module)(ds3231).
	
	// FLOAT -> 4 byte
	// zs-042 => first 16 byte use DS3231
	
	// example for use:
	
	#include "I2C_eeprom.h"
	#define DEVICE1_ADDRESS 0x57
	I2C_eeprom i2c_eeprom;		// initialization
	
	void setup() {
		Serial.begin(115200);
		i2c_eeprom.float_put(DEVICE1_ADDRESS, 20, 3.14);
		i2c_eeprom.float_put(DEVICE1_ADDRESS, 24, 7.64);
		i2c_eeprom.float_put(DEVICE1_ADDRESS, 28, 22.4);
		
		Serial.println(i2c_eeprom.float_get(DEVICE1_ADDRESS, 20));
		i2c_eeprom.float_update(DEVICE1_ADDRESS, 20, 780.25);
		boolean avaible = i2c_eeprom.is_avaible(DEVICE1_ADDRESS);
		String text = i2c_eeprom.device_scanner();
		Serial.println(avaible);
		Serial.println(text);
	}
	
	void loop() {
		
	}
	
	
	*/
	

class I2C_eeprom{
	public:
		I2C_eeprom();
		boolean is_avaible(int device_address);
		void float_put(int device_address, unsigned int address, float value);
		float float_get(int device_address, unsigned int address);
		void float_update(int device_address, unsigned int address, float value);
		String device_scanner();
			
};

	// init
	I2C_eeprom::I2C_eeprom(){				// constructor
		 Wire.begin();
	}
	
	// device_address is avaible in i2c line
	boolean I2C_eeprom::is_avaible(int device_address) {
		boolean is_avaible = false;
		Wire.beginTransmission(device_address);
		byte error = Wire.endTransmission();
		if (error == 0) is_avaible = true;
		return is_avaible;
	}
	
	// WRITE FLOAT!!!!*******************************
void I2C_eeprom::float_put(int device_address, unsigned int address, float value) {
  byte *byte_data = (byte *)&value; // (value) convert float to byte
  for (int i = 0; i < 4; i++) {
    address = address + i;
    Wire.beginTransmission(device_address);
    Wire.write((int)(address >> 8)); // MSB
    Wire.write((int)(address & 0xFF)); // LSB
    Wire.write(byte_data[i]);
    Wire.endTransmission();
    delay(10);
  }
}

// READ FLOAT!!!!*******************************
float I2C_eeprom::float_get(int device_address, unsigned int address) {
  union {
    byte byte_data[4];
    float float_value = 0;
  } u;
  // read and write 4 byte to array
  for (int i = 0; i < 4; i++) {
    address = address + i;
    Wire.beginTransmission(device_address);
    Wire.write((int)(address >> 8)); // MSB
    Wire.write((int)(address & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(device_address, 1);
    if (Wire.available()) u.byte_data[i] = Wire.read();
  }
  return  u.float_value; // float value
}


// UPDATE FLOAT!!!!*******************************
void I2C_eeprom::float_update(int device_address, unsigned int address, float value) {
  if (I2C_eeprom::float_get(device_address, address) != value) {
    I2C_eeprom::float_put(device_address, address, value);
  }
  /*else {
    Serial.println("Found duplicate!");
    }
  */
}

// I2C scanner
String I2C_eeprom::device_scanner() {
  byte error, address;
  int nDevices;
  String text = "";
  text = "\nScanning...";
  nDevices = 0;
  for (address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)    {
      text += "\nI2C device found at address [0x";
      if (address < 16) text += "0";
      text += String(address, HEX);
      text += "]";
      nDevices++;
    }
    else if (error == 4) {
      text += "Unknow error at address [0x";
      if (address < 16) text += "0";
      text += "\n" + String(address, HEX);
      text += "]";
    }
  }
  if (nDevices == 0) {
    text += "\nNo I2C devices found\n";
  } else {
    text += "\ndone\n";
  }
  return text;
}