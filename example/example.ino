#include "Parameter.h"

Parameter<int> _parameter;

void setup() {
  Serial.begin(115200);
  _parameter.setup("Testname", 9, 0, 9); //name, value, [min, max]
  _parameter.onChange([](String name, int value){Serial.println(name+" changed, new value: "+String(value));});
}

void loop() {
  while (Serial.available() > 0) {
    auto receivedByte = Serial.read();
    switch (receivedByte) {
      case '1': {
          _parameter = 1;
          break;
        }
      case '2': {
          _parameter.set(2);
          break;
        }
      case '3': {
          _parameter.set(2, false); //do not notify listeners
          break;
        }
      default: {
          break;
        }
    }
  }
}
