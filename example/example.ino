#include "Parameter.h"

ParameterGroup _parameters;
// Parameter<int> _intParameter;
IntParameter _intParameter;

void setup() {
  Serial.begin(115200);
  
  // Setting up the parameter with name, value, min, max, and description
  _intParameter.setup("intparameter", 9, 0, 9, "this is an int parameter"); 

  // Setting a custom serializer for the parameter (e.g. CSV format)
  _intParameter.setSerializer([](Parameter<int>& parameter) {
    String message = parameter.getName();
    message += ",";
    message += parameter.getDescription();
    message += ",";
    message += String(parameter.get());
    return message;
  });

  // Adding a listener that notifies when the parameter changes
  _intParameter.addListener([](Parameter<int>& parameter) {
    Serial.println(parameter.getName() + " changed, new value: " + String(parameter.get()));
    // Serial.println(parameter.serialize());
  });

  // Adding the parameter to the ParameterGroup
  _parameters.add(_intParameter);
}

void loop() {
  if (Serial.available() > 0) {
    auto receivedByte = Serial.read();
    
    switch (receivedByte) {
      case '1': {
          _intParameter = 1;
          break;
        }
      case '2': {
          _intParameter.set(2);
          break;
        }
      case '3': {
          _intParameter.set(3, false); // Do not notify listeners
          break;
        }
      case 'v': {
          Serial.print("current value: "); 
          Serial.println(_intParameter.get());
          break;
        }
      case 'a': {
          int value = _intParameter;
          Serial.println("Assigned value: " + String(value));
          break;
        }
      case 'd': {
          // Deserialize logic if needed
          break;
        }
      case 's': {
          Serial.println(_intParameter.serialize());
          break;
        }
      case 't': {
          // Access parameter by name from ParameterGroup
          auto& param = _parameters["intparameter"].as<int>();
          param = 5; // Set the parameter's value
          break;
        }
      default: {
          break;
        }
    }
  }
}
