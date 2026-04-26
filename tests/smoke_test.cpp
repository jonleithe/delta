#include "delta.h"

#include <cmath>
#include <string>

namespace {

bool nearly_equal(double lhs, double rhs, double tol = 1e-9) {
    return std::abs(lhs - rhs) < tol;
}

// Returns true and prints nothing on pass; prints the label and returns false on fail.
bool check(bool condition, const char* label) {
    if (!condition) {
        printf("FAIL: %s\n", label);
    }
    return condition;
}

} // namespace

int main() {
    bool ok = true;

    // ——— Temperature ————————————————————————————————————————————————————————
    {
        const delta::Temperature boiling(100.0, delta::TemperatureUnit::Celsius);
        ok &= check(nearly_equal(boiling.to_unit(delta::TemperatureUnit::Kelvin),    373.15),        "100 C -> K");
        ok &= check(nearly_equal(boiling.to_unit(delta::TemperatureUnit::Fahrenheit), 212.0),        "100 C -> F");
        ok &= check(nearly_equal(boiling.to_unit(delta::TemperatureUnit::Rankine),   671.67, 1e-5),  "100 C -> R");

        const delta::Temperature freezing(32.0, delta::TemperatureUnit::Fahrenheit);
        ok &= check(nearly_equal(freezing.to_unit(delta::TemperatureUnit::Celsius), 0.0, 1e-9), "32 F -> 0 C");

        const delta::Temperature abs_zero(0.0, delta::TemperatureUnit::Kelvin);
        ok &= check(nearly_equal(abs_zero.to_unit(delta::TemperatureUnit::Celsius), -273.15), "0 K -> -273.15 C");

        bool threw = false;
        try { delta::Temperature bad(-1.0, delta::TemperatureUnit::Kelvin); }
        catch (const delta::QuantityError&) { threw = true; }
        ok &= check(threw, "negative Kelvin throws");
    }

    // ——— Length ——————————————————————————————————————————————————————————————
    {
        const delta::Length one_km(1.0, delta::LengthUnit::Kilometer);
        ok &= check(nearly_equal(one_km.to_unit(delta::LengthUnit::Meter),   1000.0), "1 km -> 1000 m");
        ok &= check(nearly_equal(one_km.to_unit(delta::LengthUnit::Mile),    1.0 / 1.609344, 1e-9), "1 km -> mi");

        const delta::Length one_inch(1.0, delta::LengthUnit::Inch);
        ok &= check(nearly_equal(one_inch.to_unit(delta::LengthUnit::Meter), 0.0254), "1 in -> 0.0254 m");

        const delta::Length uk_foot(1.0, delta::LengthUnit::UKFoot);
        ok &= check(nearly_equal(uk_foot.to_unit(delta::LengthUnit::Meter),  0.3048), "1 ukft -> 0.3048 m");

        const delta::Length us_survey_foot(1.0, delta::LengthUnit::USSurveyFoot);
        ok &= check(nearly_equal(us_survey_foot.to_unit(delta::LengthUnit::Meter), 1200.0 / 3937.0), "1 usft -> m");

        const delta::Length one_meter(1.0, delta::LengthUnit::Meter);
        ok &= check(nearly_equal(one_meter.to_unit(delta::LengthUnit::USSurveyFoot), 3937.0 / 1200.0), "1 m -> usft");

        bool threw = false;
        try { delta::Length bad(-1.0, delta::LengthUnit::Meter); }
        catch (const delta::QuantityError&) { threw = true; }
        ok &= check(threw, "negative length throws");
    }

    // ——— Volume ——————————————————————————————————————————————————————————————
    {
        const delta::Volume one_liter(1.0, delta::VolumeUnit::Liter);
        ok &= check(nearly_equal(one_liter.to_unit(delta::VolumeUnit::Milliliter),  1000.0),          "1 L -> 1000 mL");
        ok &= check(nearly_equal(one_liter.to_unit(delta::VolumeUnit::CubicMeter),  0.001),            "1 L -> 0.001 m3");
        ok &= check(nearly_equal(one_liter.to_unit(delta::VolumeUnit::Gallon),      1.0 / 3.78541, 1e-5), "1 L -> gal");

        const delta::Volume one_gallon(1.0, delta::VolumeUnit::Gallon);
        ok &= check(nearly_equal(one_gallon.to_unit(delta::VolumeUnit::Liter), 3.78541, 1e-5), "1 gal -> L");

        bool threw = false;
        try { delta::Volume bad(-1.0, delta::VolumeUnit::Liter); }
        catch (const delta::QuantityError&) { threw = true; }
        ok &= check(threw, "negative volume throws");
    }

    // ——— Pressure ————————————————————————————————————————————————————————————
    {
        const delta::Pressure one_atm(1.0, delta::PressureUnit::Atmosphere);
        ok &= check(nearly_equal(one_atm.to_unit(delta::PressureUnit::Pascal),     101325.0),               "1 atm -> Pa");
        ok &= check(nearly_equal(one_atm.to_unit(delta::PressureUnit::Kilopascal), 101.325),                 "1 atm -> kPa");
        ok &= check(nearly_equal(one_atm.to_unit(delta::PressureUnit::Bar),        1.01325, 1e-9),           "1 atm -> bar");
        ok &= check(nearly_equal(one_atm.to_unit(delta::PressureUnit::Psi),        14.6959, 1e-4),           "1 atm -> psi");

        const delta::Pressure one_bar(1.0, delta::PressureUnit::Bar);
        ok &= check(nearly_equal(one_bar.to_unit(delta::PressureUnit::Pascal), 100000.0), "1 bar -> Pa");

        bool threw = false;
        try { delta::Pressure bad(-1.0, delta::PressureUnit::Pascal); }
        catch (const delta::QuantityError&) { threw = true; }
        ok &= check(threw, "negative pressure throws");
    }

    if (ok) {
        printf("All tests passed.\n");
    }

    return ok ? 0 : 1;
}
