// FLOAT -> 4 byte
// zs-042 => first 16 byte use DS3231
// for show test data in setup press -> RESET button in Arduino

#include "I2C_eeprom.h"
#define DEVICE1_ADDRESS 0x57
I2C_eeprom i2c_eeprom;    // initialization

void setup() {
  Serial.begin(115200);
//  i2c_eeprom.float_put(DEVICE1_ADDRESS, 20, 3.14);
//  i2c_eeprom.float_put(DEVICE1_ADDRESS, 24, 7.64);
//  i2c_eeprom.float_put(DEVICE1_ADDRESS, 28, 22.4);

  Serial.println("[20] => " + String(i2c_eeprom.float_get(DEVICE1_ADDRESS, 20)));
  Serial.println("[24] => " + String(i2c_eeprom.float_get(DEVICE1_ADDRESS, 24)));
  Serial.println("[28] => " + String(i2c_eeprom.float_get(DEVICE1_ADDRESS, 28)));

  Serial.println("update [20]");
  i2c_eeprom.float_update(DEVICE1_ADDRESS, 20, 780.25);
  Serial.println("[20] => " + String(i2c_eeprom.float_get(DEVICE1_ADDRESS, 20)));
  
  boolean avaible = i2c_eeprom.is_avaible(DEVICE1_ADDRESS);
  Serial.println("[0x57] is avaible: " + String(avaible));
  
  String scanner_text = i2c_eeprom.device_scanner();
  Serial.println(scanner_text);
}

void loop() {

}
