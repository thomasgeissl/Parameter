#pragma once
#include <functional>
#include<type_traits>

namespace CHECK
{
  struct No {}; 
  template<typename T, typename Arg> No operator< (const T&, const Arg&);
    template<typename T, typename Arg> No operator< (const T&, const Arg&);

 
  template<typename T, typename Arg = T>
  struct ComperatorExists
  {
    enum { value = !std::is_same<decltype(*(T*)(0) < *(Arg*)(0)), No>::value };
  };  
}


template<typename ParameterType>
class Parameter {
  public:
    void setup(String name, ParameterType value, String description = ""){
      _name = name;
      _value = value;
      _description = description;
    }
    void setup(String name, ParameterType value, ParameterType min, ParameterType max, String description = ""){
      _name = name;
      _value = value;
      _min = min;
      _max = max;
      _description = description;
    }
    void operator = (ParameterType value) {
      set(value);
    }

    void onChange(void func(ParameterType value)) {
      _changeHandler = func;
    }
    void onChange(void func(String name, ParameterType value)) {
      _changeHandlerWithName = func;
    }
    void onChange(void func(Parameter<ParameterType>, ParameterType value)) {
      // _changeHandlerWithParameter = func;
    }
    void set(ParameterType value, bool notify = true) {
      if(CHECK::ComperatorExists<ParameterType>::value){

      }else{
        _value = value;
      }
      if (notify) {
        notifyListeners(value);
      }
    }

    void setMinMax(ParameterType min, ParameterType max){
      _min = min;
      _max = max;
    }

    ParameterType get() {
      return _value;
    }

    void setName(String name) {
      _name = name;
    }
    String getName() {
      return _name;
    }

    void setDescription(String value) {
      _description = value;
    }
    String getDescription() {
      return _description;
    }

  protected:
    void notifyListeners(ParameterType value) {
      _changeHandler(value);
      _changeHandlerWithName(_name, value);
      // _changeHandlerWithParameter(this, value);
    }
  private:
    ParameterType _value;
    ParameterType _min;
    ParameterType _max;

    String _name;
    String _description;

    //TODO: use vector to be able to add more than one listener
    std::function<void(ParameterType)> _changeHandler;
    std::function<void(String, ParameterType)> _changeHandlerWithName;
    // std::function<void(Parameter<ParameterType>, ParameterType)> _changeHandlerWithParameter;

};
