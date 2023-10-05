#include <Wire.h>

void printarray(char* arr,int arr_start, int arr_end);

class PinExtender {
  public:
  int address;
  PinExtender(int address): address(address) {};

  void begin() {
      Wire.begin();
  }
  
  char* SayHello() {
    Wire.beginTransmission(address);
    Wire.write("hello");
    Wire.endTransmission();

    Wire.requestFrom(address, 5);

    char* response = new char[5];
    int i = 0;
    while (Wire.available() && i < 5) {
      response[i] = Wire.read();
      i++;
    }
    return response;
  }

  void SetPinMode(int pin, int pin_mode) {
    Wire.beginTransmission(address);
    char buf[6];
    sprintf(buf, "%d-%d-%d", 0, pin + 10, pin_mode);
    Wire.write(buf);
    Wire.endTransmission();
  }

  void DigitalWrite(int pin, bool value) {
    Wire.beginTransmission(address);
    char buf[6];
    sprintf(buf, "%d-%d-%d", 1, pin + 10, value);
    Wire.write(buf);
    Wire.endTransmission();
  }

  void AnalogWrite(int pin, bool value) {
    Wire.beginTransmission(address);
    char buf[9];
    sprintf(buf, "%d-%d-%d", 3, pin + 10, value + 4000);
    Wire.write(buf);
    Wire.endTransmission();
  }

  char* DigitalRead(int pin) {
    Wire.beginTransmission(address);
    char buf[6];
    sprintf(buf, "%d-%d-%d", 2, pin + 10, 0);
    Wire.write(buf);
    Wire.endTransmission();

    Wire.requestFrom(address, 1);
    char* response = new char[1];
    int i = 0;
    while (Wire.available() && i < 1) {
      response[i] = Wire.read();
      i++;
    }
    return response;
  }

  char* AnalogRead(int pin) {
    Wire.beginTransmission(address);
    char buf[6];
    sprintf(buf, "%d-%d-%d", 4, pin + 10, 0);
    Wire.write(buf);
    Wire.endTransmission();

    Wire.requestFrom(address, 4);
    char* response = new char[4];
    int i = 0;
    while (Wire.available() && i < 4) {
      response[i] = Wire.read();
      i++;
    }
    response[0] = response[0] - 4;
    return response;
  }

  ~PinExtender() {
    Wire.end(); 
  }
};

PinExtender pin_extender(8);

void setup() {
  pin_extender.begin();
  Serial.begin(9600);
  delay(100);
  pin_extender.SetPinMode(D15,1);
  delay(100);
  pin_extender.SetPinMode(D14,0);
  delay(100);
  pin_extender.SetPinMode(A1, 0);
  delay(100);
}

void loop() {
  //pin_extender.SetPinMode(D15,1);
  delay(10);
  pin_extender.DigitalWrite(D15, 1);
//  pin_extender.SetPinMode(A1, 0);
  delay(100);
//  pin_extender.SetPinMode(10,0);
//  Serial.println(pin_extender.DigitalRead(D14)[0]);
//  delay(1000);
//  Serial.println("");
  printarray(pin_extender.AnalogRead(A1), 0, 4);
  delay(100);
}

void printarray(char* arr,int arr_start, int arr_end) {
  for (int i=arr_start; i < arr_end; i++) {
    Serial.print(arr[i]);
  }
  Serial.println("");
}
