#include <Wire.h>
#include "lsm6dsm.h"

int current_operation = -1; //0-set pin mode 1-digital write 2-digital read
int current_pin = -1; 
int current_value = -1; //0(0-input, 1-output), 1-2(0-low, 1-high)

void printarray(char* arr,int arr_start, int arr_end);

void setup() {
  Serial.begin(9600);
  Wire.begin(8);
  Wire.onRequest(RequestEvent);
  Wire.onReceive(ReceiveEvent);
}

void loop() {
}

void RequestEvent() {
    Serial.printf("%d,%d,%d\n", current_operation, current_pin, current_value);
    switch(current_operation) {
      case 2: {
      bool value = digitalRead(current_pin);
      Wire.write(value + '0');
     
      break;
      }
      case 4:
      int value_int = analogRead(current_pin);
      char* value_char = new char[4];
      int* values = new int[4];
      values[0] = (value_int / 1000) + 4;
      values[1] = (value_int / 100) % 10;
      values[2] = (value_int / 10) % 10;
      values[3] = (value_int % 10);
      value_char[0] = values[0] + '0';
      value_char[1] = values[1] + '0';
      value_char[2] = values[2] + '0';
      value_char[3] = values[3] + '0';
//      printarray(value_char, 0, 4);
      Wire.write(value_char);
      break;
    }
}

void ReceiveEvent(int bytes_read) {
    char* received = new char[8];
    int i = 0;
    while (Wire.available()) {
      received[i] = Wire.read();
      i++;
    }
    printarray(received, 0, 8);
    
    current_pin = ((received[2]-'0' - 1) * 10) + received[3]-'0';
    current_operation = received[0]-'0';
    

    Serial.printf("%d,%d,%d\n", current_operation, current_pin, current_value);

     switch(current_operation) {
      case 0:
      current_value = received[5]-'0';
      if (current_value == 1) {
        pinMode(current_pin, OUTPUT);
        Serial.println(current_pin);
      }
      else if (current_value == 0) {
        pinMode(current_pin, INPUT);
      }
      break;

      case 1:
      current_value = received[5]-'0';
      if (current_value == 1) {
        digitalWrite(current_pin, HIGH);  
      } else if (current_value == 0) {
        digitalWrite(current_pin, LOW);
//        Serial.printf("write 0 to %d", current_pin);
      }
      break;
      
      case 3:
      current_value = (received[5]-'0' - 4) * 1000 + (received[6]-'0') * 100 + (received[7]-'0') * 10 + received[8];
      analogWrite(current_pin, current_value);
      break;
    }
}

void printarray(char* arr,int arr_start, int arr_end) {
  for (int i=arr_start; i < arr_end; i++) {
    Serial.print(arr[i]);
  }
  Serial.println("");
}

// #define D0  (uint8_t)(23)
// #define D1 (uint8_t)(22)
// #define D2 (uint8_t)(1)
// #define D3 (uint8_t)(3)
// #define D4 (uint8_t)(21)
// #define D5 (uint8_t)(19)
// #define D6 (uint8_t)(18)
// #define D7 (uint8_t)(5)
// #define D8 (uint8_t)(0)
// #define D9 (uint8_t)(2)
// #define D10  (uint8_t)(4)
// #define D11  (uint8_t)(15)
// #define D12  (uint8_t)(13)
// #define D13  (uint8_t)(12)
// #define D14  (uint8_t)(14)
// #define D15  (uint8_t)(27)
