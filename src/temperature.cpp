#include <cctype>
#include <cstdio>
#include <algorithm>

#include "delta.h"



namespace delta {

Temperature::Temperature(double value, TemperatureUnit unit){
    switch (unit){
        case TemperatureUnit::Celsius:      kelvin_ = value + 273.15;
                                            break;
        case TemperatureUnit::Fahrenheit:   kelvin_ = (value - 32.0) * 5.0 / 9.0 + 273.15;
                                            break;
        case TemperatureUnit::Kelvin:       kelvin_ = value;
                                            break;
        case TemperatureUnit::Rankine:      kelvin_ = value * 5.0 / 9.0;
                                            break;
        default:                            throw QuantityError("Invalid temperature unit");
    } // ———  END OF switch(unit)———————————————————————————————————————————————

    if (kelvin_ < 0.0) {
        throw QuantityError(quantity_error_messages::kTempBelowAbsoluteZero);
    } // ———  END OF if(kelvin_ < 0)————————————————————————————————————————————
} // ———  END OF function Temperature::Temperature——————————————————————————————



double Temperature::to_unit(TemperatureUnit unit) const{
    switch (unit){
        case TemperatureUnit::Celsius:      return kelvin_ - 273.15;
        case TemperatureUnit::Fahrenheit:   return (kelvin_ - 273.15) * 9.0 / 5.0 + 32.0;
        case TemperatureUnit::Kelvin:       return kelvin_;
        case TemperatureUnit::Rankine:      return kelvin_ * 9.0 / 5.0;
        default:                            throw QuantityError("Invalid temperature unit");
    } // ———  END OF switch(unit)———————————————————————————————————————————————
} // ———  END OF function Temperature::to_unit——————————————————————————————————



std::string Temperature::to_string(TemperatureUnit unit) const{
    static const char* names[] = {"Celsius", "Fahrenheit", "Kelvin", "Rankine"};

    return format_value(to_unit(unit),
                        format_settings::kTemperaturePrecision)
                        + " "
                        + names[static_cast<int>(unit)];
} // ———  END OF function Temperature::to_string————————————————————————————————



static bool parse_temperature_unit(const std::string& unit_str,
                                   TemperatureUnit* out_unit){
    std::string lower = unit_str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (lower == "c" || lower == "celsius") {
        *out_unit = TemperatureUnit::Celsius;
        return true;
    }

    if (lower == "f" || lower == "fahrenheit") {
        *out_unit = TemperatureUnit::Fahrenheit;
        return true;
    }

    if (lower == "k" || lower == "kelvin") {
        *out_unit = TemperatureUnit::Kelvin;
        return true;
    }

    if (lower == "r" || lower == "rankine") {
        *out_unit = TemperatureUnit::Rankine;
        return true;
    }

    return false;
} // ———  END OF function parse_temperature_unit————————————————————————————————



static void convert_temperature(double value, TemperatureUnit from_unit){
    Temperature temp(value, from_unit);
    printf("Temperature conversion of %.2f %s:\n", value,
           (from_unit == TemperatureUnit::Celsius ? "\xC2\xB0" "C" :
            from_unit == TemperatureUnit::Fahrenheit ? "\xC2\xB0" "F" :
            from_unit == TemperatureUnit::Kelvin ? "K" : "\xC2\xB0" "R"));
    printf("  %s\n", temp.to_string(TemperatureUnit::Celsius).c_str());
    printf("  %s\n", temp.to_string(TemperatureUnit::Fahrenheit).c_str());
    printf("  %s\n", temp.to_string(TemperatureUnit::Kelvin).c_str());
    printf("  %s\n", temp.to_string(TemperatureUnit::Rankine).c_str());
} // ———  END OF function convert_temperature———————————————————————————————————



bool try_convert_temperature(double value, const std::string& unit_str){
    TemperatureUnit parsed_unit;
    if (!parse_temperature_unit(unit_str, &parsed_unit)) {
        return false;
    }

    convert_temperature(value, parsed_unit);

    return true;
} // ———  END OF function try_convert_temperature———————————————————————————————

}  // namespace delta