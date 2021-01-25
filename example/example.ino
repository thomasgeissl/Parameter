#include "Parameter.h"

ParameterGroup _parameters;
Parameter<int> _parameter;

void setup() {
  Serial.begin(115200);
  _parameter.setup("Testname", 9, 0, 9); //name, value, [min, max]
  _parameter.addListener([&](String name, int value) {
    Serial.println(name + " changed, new value: " + String(value));
  });

  _parameters.add(_parameter);
}

void loop() {
  if (Serial.available() > 0) {
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
          _parameter.set(3, false); //do not notify listeners
          break;
        }
      case 'v': {
          Serial.print("current value: "); Serial.println(_parameter.get());
          break;
        }

      case 'a': {
          int value = _parameter;
          break;
        }
      case 't': {
          _parameters["Testname"]; //TODO: cast to int param
          break;
        }
      default: {
          break;
        }
    }
  }
}