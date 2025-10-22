/* Color.h */
#pragma once 

/* --------  ConstructorsAndFunctions  -------- */
#pragma region Color ConstructorsAndFunctions

class Color {
public:
    const double r;
    const double g;
    const double b;

    Color(double r = 0.0, double g = 0.0, double b = 0.0);

    Color operator+(const Color& c) const;
    Color operator*(double s) const;
    Color operator*(const Color& c) const;
    Color clamped() const;
    bool equals(const Color& c, double eps = 1e-6) const;

    static const Color BLACK();
    static const Color WHITE();
    static const Color RED();
    static const Color GREEN();
    static const Color BLUE();
};

#pragma endregion
/* --------  ConstructorsAndFunctions  -------- */