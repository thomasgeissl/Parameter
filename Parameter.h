#pragma once
#include <functional>
#include <vector>
#include <type_traits>

namespace CHECK
{
  struct No
  {
  };
  template <typename T, typename Arg>
  No operator<(const T &, const Arg &);

  template <typename T, typename Arg = T>
  struct ComperatorExists
  {
    enum
    {
      value = !std::is_same<decltype(*(T *)(0) < *(Arg *)(0)), No>::value
    };
  };
}

template <typename ParameterType>
class Parameter;

class BaseParameter
{
public:
  virtual ~BaseParameter(){};
  
  void setName(String name)
  {
    _name = name;
  }

  String getName()
  {
    return _name;
  }

  void setDescription(String value)
  {
    _description = value;
  }

  String getDescription()
  {
    return _description;
  }

  template <typename T>
  Parameter<T> &as()
  {
    auto param = static_cast<Parameter<T> *>(this);
    if (!param)
    {
      Serial.println("could not cast param");
    }
    return *param;
  }

protected:
  String _name;
  String _description;
  bool _isSerializable;
};

template <typename ParameterType>
class Parameter : public BaseParameter
{
public:
  Parameter() {}

  void setup(String name, ParameterType value, String description = "")
  {
    _name = name;
    _value = value;
    _description = description;
  }

  void setup(String name, ParameterType value, ParameterType min, ParameterType max, String description = "")
  {
    _name = name;
    _value = value;
    _min = min;
    _max = max;
    _description = description;
  }

  void operator=(ParameterType value)
  {
    set(value);
  }

  ParameterType operator*()
  {
    return _value;
  }

  operator ParameterType &()
  {
    return _value;
  }

  operator ParameterType() const
  {
    return _value;
  }

  void addListener(std::function<void(Parameter<ParameterType>&)> func)
  {
    _changeHandler.push_back(func);
  }

  void set(ParameterType value, bool notify = true, bool forceNotify = false)
  {
    auto valueHasChanged = true;
    if (CHECK::ComperatorExists<ParameterType>::value && _min != _max)
    {
      if (value == _value)
      {
        valueHasChanged = false;
      }
      if (value < _min)
      {
        value = _min;
      }
      else if (value > _max)
      {
        value = _max;
      }
      if (value >= _min && value <= _max)
      {
        _value = value;
      }
    }
    else
    {
      _value = value;
    }
    if ((notify && valueHasChanged) || forceNotify)
    {
      notifyListeners();
    }
  }

  void setMinMax(ParameterType min, ParameterType max)
  {
    _min = min;
    _max = max;
  }

  ParameterType get()
  {
    return _value;
  }

  ParameterType getMin()
  {
    return _min;
  }

  ParameterType getMax()
  {
    return _max;
  }

  void setSerializer(std::function<String(Parameter<ParameterType>&)> serializer)
  {
    _serializer = serializer;
  }

  String serialize()
  {
    return _serializer(*this);
  }

protected:
  void notifyListeners()
  {
    for (auto &changeHandler : _changeHandler)
    {
      changeHandler(*this);
    }
  }

private:
  ParameterType _value;
  ParameterType _min;
  ParameterType _max;

  std::vector<std::function<void(Parameter<ParameterType>&)>> _changeHandler;
  std::function<String(Parameter<ParameterType>&)> _serializer;
};

class ParameterGroup
{
public:
  void setup(String name)
  {
    _name = name;
  }

  BaseParameter &operator[](String name)
  {
    for (auto &parameter : _parameters)
    {
      if (parameter.getName() == name)
      {
        return parameter;
      }
    }
    // TODO: throw exception
  }

  void add(BaseParameter &parameter)
  {
    _parameters.push_back(parameter);
  }

  void add(ParameterGroup &parameterGroup)
  {
    // Additional logic can be added here if needed
  }

  void setName(String name)
  {
    _name = name;
  }

  String getName()
  {
    return _name;
  }

  bool includes(String name)
  {
    for (auto &parameter : _parameters)
    {
      if (parameter.getName() == name)
      {
        return true;
      }
    }
    return false;
  }

  std::vector<BaseParameter> _parameters;
  String _name;
};

// Type aliases for common Parameter types
typedef Parameter<bool> BoolParameter;
typedef Parameter<int> IntParameter;
typedef Parameter<float> FloatParameter;
typedef Parameter<double> DoubleParameter;
typedef Parameter<byte> ByteParameter;
typedef Parameter<String> StringParameter;
