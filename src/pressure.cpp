#include <cctype>
#include <cstdio>
#include <algorithm>

#include "delta.h"



namespace delta {

Pressure::Pressure(double value, PressureUnit unit){
    switch (unit){
        case PressureUnit::Pascal:      pascal_ = value;
                                        break;
        case PressureUnit::Kilopascal:  pascal_ = value * 1000.0;
                                        break;
        case PressureUnit::Bar:         pascal_ = value * 100000.0;
                                        break;
        case PressureUnit::Atmosphere:  pascal_ = value * 101325.0;
                                        break;
        case PressureUnit::Psi:         pascal_ = value * 6894.757293168;
                                        break;
        case PressureUnit::Torr:        pascal_ = value * 133.322368421;
                                        break;
        case PressureUnit::MmHg:        pascal_ = value * 133.322387415;
                                        break;
        default:                        throw QuantityError("Invalid pressure unit");
    } // ———  END OF switch(unit)———————————————————————————————————————————————

    if (pascal_ < 0.0) {
        throw QuantityError(quantity_error_messages::kPressureNegative);
    }
} // ———  END OF function Pressure::Pressure————————————————————————————————————



double Pressure::to_unit(PressureUnit unit) const{
    switch (unit){
        case PressureUnit::Pascal:      return pascal_;
        case PressureUnit::Kilopascal:  return pascal_ / 1000.0;
        case PressureUnit::Bar:         return pascal_ / 100000.0;
        case PressureUnit::Atmosphere:  return pascal_ / 101325.0;
        case PressureUnit::Psi:         return pascal_ / 6894.757293168;
        case PressureUnit::Torr:        return pascal_ / 133.322368421;
        case PressureUnit::MmHg:        return pascal_ / 133.322387415;
        default:                        throw QuantityError("Invalid pressure unit");
    } // ———  END OF switch(unit)———————————————————————————————————————————————
} // ———  END OF function Pressure::to_unit—————————————————————————————————————



std::string Pressure::to_string(PressureUnit unit) const{
    static const char* names[] = {"Pa", "kPa", "bar", "atm", "psi", "torr", "mmHg"};

    return format_value(to_unit(unit), format_settings::kPressurePrecision) + " " + names[static_cast<int>(unit)];
} // ———  END OF function Pressure::to_string———————————————————————————————————



Pressure Pressure::operator+(const Pressure& other) const{
    Pressure result(pascal_, PressureUnit::Pascal);
    result.pascal_ = this->pascal_ + other.pascal_;

    return result;
} // ———  END OF function Pressure::operator+———————————————————————————————————



Pressure Pressure::operator-(const Pressure& other) const{
    Pressure result(pascal_, PressureUnit::Pascal);
    result.pascal_ = this->pascal_ - other.pascal_;
    if (result.pascal_ < 0.0) {
        throw QuantityError(quantity_error_messages::kPressureSubNegative);
    }

    return result;
} // ———  END OF function Pressure::operator-———————————————————————————————————



Pressure Pressure::operator*(double scalar) const{
    Pressure result(pascal_, PressureUnit::Pascal);
    result.pascal_ = this->pascal_ * scalar;
    if (result.pascal_ < 0.0) {
        throw QuantityError(quantity_error_messages::kPressureMulNegative);
    }

    return result;
} // ———  END OF function Pressure::operator*———————————————————————————————————



Pressure Pressure::operator/(double scalar) const{
    if (scalar == 0.0) {
        throw QuantityError(quantity_error_messages::kDivisionByZero);
    }

    Pressure result(pascal_, PressureUnit::Pascal);
    result.pascal_ = this->pascal_ / scalar;
    if (result.pascal_ < 0.0) {
        throw QuantityError(quantity_error_messages::kPressureDivNegative);
    }

    return result;
} // ———  END OF function Pressure::operator/———————————————————————————————————



Pressure operator*(double scalar, const Pressure& p) {
    return p * scalar;
} // ———  END OF function operator*(Pressure)———————————————————————————————————



static bool parse_pressure_unit(const std::string& unit_str,
                                PressureUnit* out_unit){
    std::string lower = unit_str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (lower == "pa" || lower == "pascal" || lower == "pascals") {
        *out_unit = PressureUnit::Pascal;
        return true;
    }

    if (lower == "kpa" || lower == "kilopascal" || lower == "kilopascals") {
        *out_unit = PressureUnit::Kilopascal;
        return true;
    }

    if (lower == "bar") {
        *out_unit = PressureUnit::Bar;
        return true;
    }

    if (lower == "atm" || lower == "atmosphere" || lower == "atmospheres") {
        *out_unit = PressureUnit::Atmosphere;
        return true;
    }

    if (lower == "psi") {
        *out_unit = PressureUnit::Psi;
        return true;
    }

    if (lower == "torr") {
        *out_unit = PressureUnit::Torr;
        return true;
    }

    if (lower == "mmhg") {
        *out_unit = PressureUnit::MmHg;
        return true;
    }

    return false;
} // ———  END OF function parse_pressure_unit———————————————————————————————————



static void convert_pressure(double value, PressureUnit from_unit){
    Pressure p(value, from_unit);
    printf("Pressure conversion of %.4f %s:\n", value,
           (from_unit == PressureUnit::Pascal ? "Pa" :
            from_unit == PressureUnit::Kilopascal ? "kPa" :
            from_unit == PressureUnit::Bar ? "bar" :
            from_unit == PressureUnit::Atmosphere ? "atm" :
            from_unit == PressureUnit::Psi ? "psi" :
            from_unit == PressureUnit::Torr ? "torr" : "mmHg"));
    printf("  %s\n", p.to_string(PressureUnit::Pascal).c_str());
    printf("  %s\n", p.to_string(PressureUnit::Kilopascal).c_str());
    printf("  %s\n", p.to_string(PressureUnit::Bar).c_str());
    printf("  %s\n", p.to_string(PressureUnit::Atmosphere).c_str());
    printf("  %s\n", p.to_string(PressureUnit::Psi).c_str());
    printf("  %s\n", p.to_string(PressureUnit::Torr).c_str());
    printf("  %s\n", p.to_string(PressureUnit::MmHg).c_str());
} // ———  END OF function convert_pressure——————————————————————————————————————



bool try_convert_pressure(double value, const std::string& unit_str){
    PressureUnit parsed_unit;
    if (!parse_pressure_unit(unit_str, &parsed_unit)) {
        return false;
    }

    convert_pressure(value, parsed_unit);

    return true;
} // ———  END OF function try_convert_pressure——————————————————————————————————

}  // namespace delta