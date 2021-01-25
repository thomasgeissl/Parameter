#pragma once
#include <functional>
#include <vector>
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

class BaseParameter{
  public:
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
    String _name;
    String _description;

};

template<typename ParameterType>
class Parameter : public BaseParameter{
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
    ParameterType operator* (){
      return _value;
    }
    operator ParameterType&() { return _value; }
    operator ParameterType() const { return _value; }

    void addListener(void func(ParameterType value)) {
      _changeHandler.push_back(func);
    }
    void addListener(void func(String name, ParameterType value)) {
      _changeHandlerWithName.push_back(func);
    }
    void addListener(void func(Parameter<ParameterType>, ParameterType value)) {
      // _changeHandlerWithParameter.push_back(func);
    }
    void set(ParameterType value, bool notify = true) {
      if(CHECK::ComperatorExists<ParameterType>::value && _min != _max){
        if(value >= _min && value <= _max){
          _value = value;
        }
      }else{
        _value = value;
      }
      if (notify) {
        notifyListeners(_value);
      }
    }

    void setMinMax(ParameterType min, ParameterType max){
      _min = min;
      _max = max;
    }

    ParameterType get() {
      return _value;
    }



  protected:
    void notifyListeners(ParameterType value) {
      for(auto changeHandler : _changeHandler){
        changeHandler(value);
      }
      for(auto changeHandler : _changeHandlerWithName){
        changeHandler(_name, value);
      }
      // for(auto changeHandler : _changeHandlerWithParameter){
      //   changeHandler(this, value);
      // }
    }
  private:
    ParameterType _value;
    ParameterType _min;
    ParameterType _max;


    std::vector<std::function<void(ParameterType)>> _changeHandler;
    std::vector<std::function<void(String, ParameterType)>> _changeHandlerWithName;
    // std::function<void(Parameter<ParameterType>, ParameterType)> _changeHandlerWithParameter;

};


class ParameterGroup {
  public:
    void setup(String name){
      _name = name;
    }
    BaseParameter & operator [] (String name) {
      for(auto & parameter : _parameters){
        if(parameter.getName() == name){
          return parameter;
        }
      }
      // TODO: throw exception
    }

    void add(BaseParameter &parameter){
      _parameters.push_back(parameter);
    }
    void add(ParameterGroup &parameterGroup){
      
    }
    void setName(String name){
      _name = name;
    }
    String getName(){
      return _name;
    }
    std::vector<BaseParameter> _parameters;
  String _name;
};