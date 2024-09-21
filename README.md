# Parameter
## description
Templatized parameter library for arduino

this is a work in progress, use at your own risk, still needs some testing after the recent changes


## usage
* declare parameters and optionally a parameter group (as global variables outside of setup or loop)
```cpp
ParameterGroup _parameters;
Parameter<int> _intParameter;
Parameter<bool> _boolParameter;
```
* set them up (in setup)
```cpp
  _intParameter.setup("intparameter", 9, 0, 9, "this is an int parameter"); 
  _boolParameter.setup("boolparameter", false, "this is a bool parameter"); 
```
* add a custom serializer (in setup)
```cpp
  _intParameter.setSerializer([](Parameter<int>& parameter) {
    String message = parameter.getName();
    message += ",";
    message += parameter.getDescription();
    message += ",";
    message += String(parameter.get());
    return message;
  });
```
* add a change listener (in setup)
```cpp
  _intParameter.addListener([](Parameter<int>& parameter) {
    Serial.println(parameter.getName() + " changed, new value: " + String(parameter.get()));
    // Serial.println(parameter.serialize());
  });
```
* use it pretty much like a normal variable (in loop)
```cpp
_intParameter = 1;
_intParameter.set(2);
_intParameter.set(2, false);//set without event notification
int value = _intParameter;
String serializedParameter = _intParameter.serialize();
```