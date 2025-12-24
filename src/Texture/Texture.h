#pragma once
#include "../Color/Color.h"
#include "../Point/Point.h"
#include "../UV/UV.h"

/**
 * Texture (abstrakt)
 *
 * Liefert eine Farbe anhand von UV-Koordinaten (und optional dem 3D-Punkt).
 */
class Texture {
public:
    virtual ~Texture() = default;

    /**
     * Samplet die Textur.
     *
     * @param uv UV-Koordinaten (typisch im Bereich [0,1])
     * @param point Punkt auf der Oberfläche (Weltkoordinaten)
     */
    virtual Color sample(const UV& uv, const Point& point) const = 0;
};

