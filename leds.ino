#include <unordered_map>
#include <iterator>

#define PIN_FRONT_LAMP_1 LED_PWR
#define PIN_FRONT_LAMP_2 LED_BUILTIN

#define UUID_LIGHT_SERVICE1 "19B10001-E8F2-537E-4F6C-D104768A1214"
#define UUID_LIGHT_SERVICE2 "19B10002-E8F2-537E-4F6C-D104768A1214"
#define BLE_CHAR_BSS_CTRL_POINT "2B2B"
#define BLE_CHAR_ANALOG_OUTPUT "2A59"

#define BLE_DESC_CHAR_CONFIG "2902"
#define BLE_DESC_CHAR_USER_DESC "2901"


void buildLEDService() {
  BLEService ledService(UUID_LIGHT_SERVICE1);
  BLEBooleanCharacteristic switchChar(BLE_CHAR_BSS_CTRL_POINT, BLERead | BLEWrite);
  BLEDescriptor switchDesc(BLE_DESC_CHAR_USER_DESC, "LED orange on/off");
  switchChar.addDescriptor(switchDesc);
  
  switchChar.setEventHandler(BLERead, readOrangeLED);
  switchChar.setEventHandler(BLEWritten, writeOrangeLED);
  ledService.addCharacteristic(switchChar);
  
  BLEUnsignedShortCharacteristic dimChar(BLE_CHAR_ANALOG_OUTPUT, BLERead | BLEWrite);
  BLEDescriptor dimDesc(BLE_DESC_CHAR_USER_DESC, "LED orange light level");
  dimChar.addDescriptor(dimDesc);
  
  dimChar.setEventHandler(BLERead, readOrangeDim);
  dimChar.setEventHandler(BLEWritten, writeOrangeDim);
  ledService.addCharacteristic(dimChar);
  
  BLE.addService(ledService);

  
  char* name2 = "LED green";
  BLEService ledService2(UUID_LIGHT_SERVICE2);
  BLEBooleanCharacteristic switchChar2(BLE_CHAR_BSS_CTRL_POINT, BLERead | BLEWrite);
  BLEDescriptor switchDesc2(BLE_DESC_CHAR_USER_DESC, "LED green on/off");
  switchChar2.addDescriptor(switchDesc2);
  switchChar2.setEventHandler(BLERead, readGreenLED);
  switchChar2.setEventHandler(BLEWritten, writeGreenLED);
  ledService2.addCharacteristic(switchChar2);
  
  BLEUnsignedShortCharacteristic dimChar2(BLE_CHAR_ANALOG_OUTPUT, BLERead | BLEWrite);
  BLEDescriptor dimDesc2(BLE_DESC_CHAR_USER_DESC, "LED green light level");
  dimChar2.addDescriptor(dimDesc2);
  dimChar2.setEventHandler(BLERead, readGreenDim);
  dimChar2.setEventHandler(BLEWritten, writeGreenDim);
  ledService2.addCharacteristic(dimChar2);
  
  BLE.addService(ledService2);
}

void readOrangeLED(BLEDevice central, BLECharacteristic characteristic) {
  uint8_t value = digitalRead(PIN_FRONT_LAMP_2);
  characteristic.writeValue(value);
}

void writeOrangeLED(BLEDevice central, BLECharacteristic characteristic) {
  digitalWrite(PIN_FRONT_LAMP_2, (characteristic.value() ? HIGH : LOW));
}

void readGreenLED(BLEDevice central, BLECharacteristic characteristic) {
  uint8_t value = digitalRead(PIN_FRONT_LAMP_1);
  characteristic.writeValue(value);
}

void writeGreenLED(BLEDevice central, BLECharacteristic characteristic) {
  digitalWrite(PIN_FRONT_LAMP_1, (characteristic.value() ? HIGH : LOW));
}

void readOrangeDim(BLEDevice central, BLECharacteristic characteristic) {
  uint16_t value  = analogRead(PIN_FRONT_LAMP_2);
  characteristic.writeValue(value);
}

void writeOrangeDim(BLEDevice central, BLECharacteristic characteristic) {
  uint16_t value = 0;
  characteristic.readValue(value);
  analogWrite(PIN_FRONT_LAMP_2, value);
}

void readGreenDim(BLEDevice central, BLECharacteristic characteristic) {
  uint16_t value  = analogRead(PIN_FRONT_LAMP_1);
  characteristic.writeValue(value);
}

void writeGreenDim(BLEDevice central, BLECharacteristic characteristic) {
  uint16_t value = 0;
  characteristic.readValue(value);
  analogWrite(PIN_FRONT_LAMP_1, value);
}
