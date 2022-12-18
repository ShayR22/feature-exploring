#pragma once

#include <optional>

template<typename Value, typename Error>
class Expected {
public:
    Expected(Value value) { _value = value;}
    Expected(Error error) { _error = error;}

    bool is_error() const {return _error.has_value();}
    Error error() const {return _error.value(); }
    Value value() const {return _value.value(); }
private:
    std::optional<Value> _value;
    std::optional<Error> _error;
};