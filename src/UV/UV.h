#pragma once
#include <cmath>

/**
 * UV-Struktur
 *
 * Repräsentiert Texturkoordinaten im Bereich [0,1].
 * u: horizontal (links->rechts)
 * v: vertikal (oben->unten)
 */
struct UV {
    double u;
    double v;

    UV(double u = 0.0, double v = 0.0) : u(u), v(v) {}

    bool equals(const UV& other, double eps = 1e-6) const {
        return std::fabs(u - other.u) <= eps &&
               std::fabs(v - other.v) <= eps;
    }
};

