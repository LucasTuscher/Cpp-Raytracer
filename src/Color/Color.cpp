/* Color.cpp */
#include "Color.h"
#include <algorithm> // für std::clamp
#include <cmath>     // für std::fabs


/* --------  ConstructorsAndFunctions  -------- */
#pragma region ConstructorsAndFunctions

Color::Color(double r, double g, double b) : r(r), g(g), b(b) {}

// Addiert zwei Farben komponentenweise und gibt eine neue Color zurück
Color Color::operator+(const Color& c) const {
    return Color(r + c.r, g + c.g, b + c.b);
}

// Multipliziert jede Farbkomponente mit einem Skalar und gibt eine neue Color zurück
Color Color::operator*(double s) const {
    return Color(r * s, g * s, b * s);
}

// Begrenzt alle Farbkomponenten auf den Bereich [0,1] und gibt eine neue Color zurück
Color Color::clamped() const {
    return Color(
        std::clamp(r, 0.0, 1.0),
        std::clamp(g, 0.0, 1.0),
        std::clamp(b, 0.0, 1.0)
    );
}

// Vergleicht zwei Farben mit einer kleinen Toleranz (eps) und gibt true zurück, 
// wenn alle Komponenten nahezu gleich sind
bool Color::equals(const Color& c, double eps) const {
    return std::fabs(r - c.r) < eps &&
           std::fabs(g - c.g) < eps &&
           std::fabs(b - c.b) < eps;
}

const Color Color::BLACK() { return Color(0, 0, 0); }
const Color Color::WHITE() { return Color(1, 1, 1); }
const Color Color::RED()   { return Color(1, 0, 0); }
const Color Color::GREEN() { return Color(0, 1, 0); }
const Color Color::BLUE()  { return Color(0, 0, 1); }

#pragma endregion
/* --------  ConstructorsAndFunctions  -------- */