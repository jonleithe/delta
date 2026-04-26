#include <cctype>
#include <cstdio>
#include <algorithm>

#include "delta.h"



namespace delta{

Length::Length(double value, LengthUnit unit){
    
    switch (unit){
        case LengthUnit::Millimeter:    meters_ = value / 1000.0;
                                        break;
        case LengthUnit::Centimeter:    meters_ = value / 100.0;
                                        break;
        case LengthUnit::Meter:         meters_ = value;
                                        break;
        case LengthUnit::Kilometer:     meters_ = value * 1000.0;
                                        break;
        case LengthUnit::Inch:          meters_ = value * 0.0254;
                                        break;
        case LengthUnit::Foot:          meters_ = value * 0.3048;
                                        break;
        case LengthUnit::Mile:          meters_ = value * 1609.344;
                                        break;
        default:                        throw QuantityError("Invalid length unit");
    } // ———  END OF switch(unit)———————————————————————————————————————————————

    if (meters_ < 0.0) {
        throw QuantityError(quantity_error_messages::kLengthNegative);
    } // ———  END OF if(meters_ < 0)————————————————————————————————————————————

} // ———  END OF function Length::Length————————————————————————————————————————



double Length::to_unit(LengthUnit unit) const{
    switch (unit){
        case LengthUnit::Millimeter:    return meters_ * 1000.0;
        case LengthUnit::Centimeter:    return meters_ * 100.0;
        case LengthUnit::Meter:         return meters_;
        case LengthUnit::Kilometer:     return meters_ / 1000.0;
        case LengthUnit::Inch:          return meters_ / 0.0254;
        case LengthUnit::Foot:          return meters_ / 0.3048;
        case LengthUnit::Mile:          return meters_ / 1609.344;
        default:                        throw QuantityError("Invalid length unit");
    } // ———  END OF switch(unit)———————————————————————————————————————————————
} // ———  END OF function Length::to_unit———————————————————————————————————————



std::string Length::to_string(LengthUnit unit) const{
    static const char* names[] = {"mm", "cm", "m", "km", "in", "ft", "mi"};
    
    return format_value(to_unit(unit),
                        format_settings::kLengthPrecision) 
                        + " " 
                        + names[static_cast<int>(unit)];
} // ———  END OF function Length::to_string—————————————————————————————————————



Length Length::operator+(const Length& other) const{
    Length result(meters_, LengthUnit::Meter);
    result.meters_ = this->meters_ + other.meters_;

    return result;    
} // ———  END OF function Length::operator+—————————————————————————————————————



Length Length::operator-(const Length& other) const{
    Length result(meters_, LengthUnit::Meter);
    result.meters_ = this->meters_ - other.meters_;
    if (result.meters_ < 0.0) {
        throw QuantityError(quantity_error_messages::kLengthSubNegative);
    }

    return result;
} // ———  END OF function Length::operator-—————————————————————————————————————



Length Length::operator*(double scalar) const{
    Length result(meters_, LengthUnit::Meter);
    result.meters_ = this->meters_ * scalar;
    if (result.meters_ < 0.0) {
        throw QuantityError(quantity_error_messages::kLengthMulNegative);
    }

    return result;
} // ———  END OF function Length::operator*—————————————————————————————————————



Length Length::operator/(double scalar) const{
    if (scalar == 0.0) {
        throw QuantityError(quantity_error_messages::kDivisionByZero);
    }

    Length result(meters_, LengthUnit::Meter);
    result.meters_ = this->meters_ / scalar;

    if (result.meters_ < 0.0) {
        throw QuantityError(quantity_error_messages::kLengthDivNegative);
    }

    return result;
} // ———  END OF function Length::operator/—————————————————————————————————————



Length operator*(double scalar, const Length& len){
    return len * scalar;
} // ———  END OF function operator*(Length)—————————————————————————————————————



static bool parse_length_unit(const std::string& unit_str,
                              LengthUnit* out_unit){
    std::string lower = unit_str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (lower == "mm" || lower == "millimeter" || lower == "millimeters") {
        *out_unit = LengthUnit::Millimeter;
        return true;
    }

    if (lower == "cm" || lower == "centimeter" || lower == "centimeters") {
        *out_unit = LengthUnit::Centimeter;
        return true;
    }

    if (lower == "m" || lower == "meter" || lower == "meters") {
        *out_unit = LengthUnit::Meter;
        return true;
    }

    if (lower == "km" || lower == "kilometer" || lower == "kilometers") {
        *out_unit = LengthUnit::Kilometer;
        return true;
    }

    if (lower == "in" || lower == "inch" || lower == "inches") {
        *out_unit = LengthUnit::Inch;
        return true;
    }

    if (lower == "ft" || lower == "foot" || lower == "feet") {
        *out_unit = LengthUnit::Foot;
        return true;
    }

    if (lower == "mi" || lower == "mile" || lower == "miles") {
        *out_unit = LengthUnit::Mile;
        return true;
    }

    return false;
} // ———  END OF function parse_length_unit—————————————————————————————————————



static void convert_length(double value, LengthUnit from_unit){
    Length len(value, from_unit);
    printf("Length conversion of %.4f %s:\n", value,
           (from_unit == LengthUnit::Millimeter ? "mm" :
            from_unit == LengthUnit::Centimeter ? "cm" :
            from_unit == LengthUnit::Meter ? "m" :
            from_unit == LengthUnit::Kilometer ? "km" :
            from_unit == LengthUnit::Inch ? "in" :
            from_unit == LengthUnit::Foot ? "ft" : "mi"));
    printf("  %s\n", len.to_string(LengthUnit::Millimeter).c_str());
    printf("  %s\n", len.to_string(LengthUnit::Centimeter).c_str());
    printf("  %s\n", len.to_string(LengthUnit::Meter).c_str());
    printf("  %s\n", len.to_string(LengthUnit::Kilometer).c_str());
    printf("  %s\n", len.to_string(LengthUnit::Inch).c_str());
    printf("  %s\n", len.to_string(LengthUnit::Foot).c_str());
    printf("  %s\n", len.to_string(LengthUnit::Mile).c_str());
} // ———  END OF function convert_length————————————————————————————————————————



bool try_convert_length(double value, const std::string& unit_str) {
    LengthUnit parsed_unit;
    if (!parse_length_unit(unit_str, &parsed_unit)) {
        return false;
    }

    convert_length(value, parsed_unit);

    return true;
} // ———  END OF function try_convert_length————————————————————————————————————

}  // namespace delta