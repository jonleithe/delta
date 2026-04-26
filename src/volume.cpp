#include <cctype>
#include <cstdio>
#include <algorithm>

#include "delta.h"



namespace delta {

Volume::Volume(double value, VolumeUnit unit){
    switch (unit){
        case VolumeUnit::Milliliter:        liters_ = value / 1000.0;
                                            break;
        case VolumeUnit::Liter:             liters_ = value;
                                            break;
        case VolumeUnit::Gallon:            liters_ = value * 3.78541;
                                            break;
        case VolumeUnit::FluidOunce:        liters_ = value * 0.0295735;
                                            break;
        case VolumeUnit::CubicMeter:        liters_ = value * 1000.0;
                                            break;
        case VolumeUnit::CubicMillimeter:   liters_ = value / 1000000.0;
                                            break;
        case VolumeUnit::CubicCentimeter:   liters_ = value / 1000.0;
                                            break;
        default:                            throw QuantityError("Invalid volume unit");
    } // ———  END OF switch(unit)———————————————————————————————————————————————

    if (liters_ < 0.0) {
        throw QuantityError(quantity_error_messages::kVolumeNegative);
    }
} // ———  END OF function Volume::Volume————————————————————————————————————————



double Volume::to_unit(VolumeUnit unit) const{
    switch (unit){
        case VolumeUnit::Milliliter:        return liters_ * 1000.0;
        case VolumeUnit::Liter:             return liters_;
        case VolumeUnit::Gallon:            return liters_ / 3.78541;
        case VolumeUnit::FluidOunce:        return liters_ / 0.0295735;
        case VolumeUnit::CubicMeter:        return liters_ / 1000.0;
        case VolumeUnit::CubicMillimeter:   return liters_ * 1000000.0;
        case VolumeUnit::CubicCentimeter:   return liters_ * 1000.0;
        default:                            throw QuantityError("Invalid volume unit");
    } // ———  END OF switch(unit)———————————————————————————————————————————————
} // ———  END OF function Volume::to_unit———————————————————————————————————————



std::string Volume::to_string(VolumeUnit unit) const{
    static const char* names[] = {"mL",
                                  "L",
                                  "gal",
                                  "fl oz",
                                  "m\xC2\xB3",
                                  "mm\xC2\xB3",
                                  "cm\xC2\xB3"};
    
    return format_value(to_unit(unit),
                        format_settings::kVolumePrecision)
                        + " "
                        + names[static_cast<int>(unit)];
} // ———  END OF function Volume::to_string—————————————————————————————————————



Volume Volume::operator+(const Volume& other) const{
    Volume result(liters_, VolumeUnit::Liter);
    result.liters_ = this->liters_ + other.liters_;
    
    return result;
} // ———  END OF function Volume::operator+—————————————————————————————————————



Volume Volume::operator-(const Volume& other) const{
    Volume result(liters_, VolumeUnit::Liter);
    result.liters_ = this->liters_ - other.liters_;
    if (result.liters_ < 0.0) {
        throw QuantityError(quantity_error_messages::kVolumeSubNegative);
    }

    return result;
} // ———  END OF function Volume::operator-—————————————————————————————————————



Volume Volume::operator*(double scalar) const{
    Volume result(liters_, VolumeUnit::Liter);
    result.liters_ = this->liters_ * scalar;
    if (result.liters_ < 0.0) {
        throw QuantityError(quantity_error_messages::kVolumeMulNegative);
    }
    
    return result;
} // ———  END OF function Volume::operator*—————————————————————————————————————



Volume Volume::operator/(double scalar) const{
    if (scalar == 0.0) {
        throw QuantityError(quantity_error_messages::kDivisionByZero);
    }

    Volume result(liters_, VolumeUnit::Liter);
    result.liters_ = this->liters_ / scalar;
    if (result.liters_ < 0.0) {
        throw QuantityError(quantity_error_messages::kVolumeDivNegative);
    }

    return result;
} // ———  END OF function Volume::operator/—————————————————————————————————————



Volume operator*(double scalar, const Volume& vol){
    return vol * scalar;
} // ———  END OF function operator*(Volume)—————————————————————————————————————



static bool parse_volume_unit(const std::string& unit_str,
                              VolumeUnit* out_unit){
    std::string lower = unit_str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (lower == "ml" || lower == "milliliter" || lower == "milliliters") {
        *out_unit = VolumeUnit::Milliliter;
        return true;
    }
    
    if (lower == "l" || lower == "liter" || lower == "liters") {
        *out_unit = VolumeUnit::Liter;
        return true;
    }
    
    if (lower == "gal" || lower == "gallon" || lower == "gallons") {
        *out_unit = VolumeUnit::Gallon;
        return true;
    }
    
    if (lower == "fl oz" || lower == "fluidounce" || lower == "fluidounces" || lower == "floz") {
        *out_unit = VolumeUnit::FluidOunce;
        return true;
    }
    
    if (lower == "m3" || lower == "m\xC2\xB3" || lower == "cubicmeter" || lower == "cubicmeters") {
        *out_unit = VolumeUnit::CubicMeter;
        return true;
    }

    if (lower == "mm3" || lower == "mm\xC2\xB3" || lower == "cubicmm" || lower == "cubicmillimeter") {
        *out_unit = VolumeUnit::CubicMillimeter;
        return true;
    }

    if (lower == "cm3" || lower == "cm\xC2\xB3" || lower == "cubiccm" || lower == "cubiccentimeter") {
        *out_unit = VolumeUnit::CubicCentimeter;
        return true;
    }

    return false;
} // ———  END OF function parse_volume_unit—————————————————————————————————————



static void convert_volume(double value, VolumeUnit from_unit){
    Volume vol(value, from_unit);
    printf("Volume conversion of %.4f %s:\n", value,
           (from_unit == VolumeUnit::Milliliter ? "mL" :
            from_unit == VolumeUnit::Liter ? "L" :
            from_unit == VolumeUnit::Gallon ? "gal" :
            from_unit == VolumeUnit::FluidOunce ? "fl oz" :
            from_unit == VolumeUnit::CubicMeter ? "m\xC2\xB3" :
            from_unit == VolumeUnit::CubicMillimeter ? "mm\xC2\xB3" : "cm\xC2\xB3"));
    printf("  %s\n", vol.to_string(VolumeUnit::Milliliter).c_str());
    printf("  %s\n", vol.to_string(VolumeUnit::Liter).c_str());
    printf("  %s\n", vol.to_string(VolumeUnit::Gallon).c_str());
    printf("  %s\n", vol.to_string(VolumeUnit::FluidOunce).c_str());
    printf("  %s\n", vol.to_string(VolumeUnit::CubicMeter).c_str());
    printf("  %s\n", vol.to_string(VolumeUnit::CubicMillimeter).c_str());
    printf("  %s\n", vol.to_string(VolumeUnit::CubicCentimeter).c_str());
} // ———  END OF function convert_volume————————————————————————————————————————



bool try_convert_volume(double value, const std::string& unit_str){
    VolumeUnit parsed_unit;
    if (!parse_volume_unit(unit_str, &parsed_unit)) {
        return false;
    }

    convert_volume(value, parsed_unit);

    return true;
} // ———  END OF function try_convert_volume————————————————————————————————————

}  // namespace delta
