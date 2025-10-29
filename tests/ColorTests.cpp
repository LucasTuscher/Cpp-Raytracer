/**
 * ColorTests.cpp
 * 
 * Unit-Tests für die Color-Klasse.
 * Testet alle Operationen und Eigenschaften von Farben im RGB-Modell.
 * 
 * Basiert auf den Test-Szenarien aus dem Universitätsskript.
 */
#include <gtest/gtest.h>
#include "../src/Color/Color.h"

// Test: Eine neue Farbe hat alle Komponenten auf Null gesetzt
TEST(ColorTest, NewColorHasAllComponentsZero) {
    Color color;
    EXPECT_DOUBLE_EQ(color.r, 0.0);
    EXPECT_DOUBLE_EQ(color.g, 0.0);
    EXPECT_DOUBLE_EQ(color.b, 0.0);
}

// Test: Standard-Konstruktor mit drei Parametern
TEST(ColorTest, StandardConstructor) {
    Color color(0.3, 0.2, 0.4);
    EXPECT_DOUBLE_EQ(color.r, 0.3);
    EXPECT_DOUBLE_EQ(color.g, 0.2);
    EXPECT_DOUBLE_EQ(color.b, 0.4);
}

// Test: Addition zweier Farben (Lichter kombinieren)
TEST(ColorTest, AddingColors) {
    Color color(0.9, 0.6, 0.75);
    Color color1(0.7, 0.1, 0.25);
    Color result = color + color1;
    EXPECT_TRUE(result == Color(1.6, 0.7, 1.0));
}

// Test: Multiplikation einer Farbe mit einem Skalar (Helligkeit ändern)
TEST(ColorTest, MultiplyingColorByScalar) {
    Color color1(0.2, 0.3, 0.4);
    Color result1 = color1 * 2;
    EXPECT_TRUE(result1 == Color(0.4, 0.6, 0.8));

    Color color2(0.8, 1.2, 1.6);
    Color result2 = color2 * 0.5;
    EXPECT_TRUE(result2 == Color(0.4, 0.6, 0.8));
}

// Test: Hadamard-Produkt (komponentenweise Multiplikation für Farbfilterung)
TEST(ColorTest, HadamardProduct) {
    Color color(1.0, 0.2, 0.4);
    Color color1(0.9, 1, 0.1);
    Color result = color * color1;
    EXPECT_TRUE(result == Color(0.9, 0.2, 0.04));
}

// Test: Clamping von Farben auf den Bereich [0, 1]
TEST(ColorTest, ClampingColors) {
    Color color1(0.1, 0.2, 0.3);
    Color result1 = color1.clamped();
    EXPECT_TRUE(result1 == Color(0.1, 0.2, 0.3));

    Color color2(-0.1, -0.2, 0.3);
    Color result2 = color2.clamped();
    EXPECT_TRUE(result2 == Color(0.0, 0.0, 0.3));

    Color color3(0.1, 1.2, 1.3);
    Color result3 = color3.clamped();
    EXPECT_TRUE(result3 == Color(0.1, 1.0, 1.0));
}

// Test: Vordefinierte Standardfarben
TEST(ColorTest, PredefinedColors) {
    Color black = Color::BLACK();
    EXPECT_TRUE(black == Color(0.0, 0.0, 0.0));

    Color white = Color::WHITE();
    EXPECT_TRUE(white == Color(1.0, 1.0, 1.0));

    Color red = Color::RED();
    EXPECT_TRUE(red == Color(1.0, 0.0, 0.0));

    Color green = Color::GREEN();
    EXPECT_TRUE(green == Color(0.0, 1.0, 0.0));

    Color blue = Color::BLUE();
    EXPECT_TRUE(blue == Color(0.0, 0.0, 1.0));
}

// Test: Gleichheit und Ungleichheit von Farben (mit Epsilon-Toleranz)
TEST(ColorTest, ColorEquality) {
    Color color1(0.5, 0.5, 0.5);
    Color color2(0.5, 0.5, 0.5);
    EXPECT_TRUE(color1 == color2);

    Color color3(0.5, 0.5, 0.5);
    Color color4(0.6, 0.5, 0.5);
    EXPECT_TRUE(color3 != color4);
}
