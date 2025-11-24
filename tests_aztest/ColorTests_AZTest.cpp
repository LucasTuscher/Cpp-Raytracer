/**
 * ColorTests_AZTest.cpp
 *
 * Unit-Tests für die Color-Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/Color/Color.h"

// ============================================================================
// Feature: Color
// ============================================================================

/**
 * Scenario: A new color has all components zero
 */
TEST(Color, NewColorHasAllComponentsZero) {
    // Given color is a Color
    Color color;

    // Then color.r = 0.0
    EXPECT_NEAR(color.r, 0.0, 1e-10);

    // And color.g = 0.0
    EXPECT_NEAR(color.g, 0.0, 1e-10);

    // And color.b = 0.0
    EXPECT_NEAR(color.b, 0.0, 1e-10);
}

/**
 * Scenario: Standard constructor with three parameters
 */
TEST(Color, StandardConstructor) {
    // Given color is a Color(0.3, 0.2, 0.4)
    Color color(0.3, 0.2, 0.4);

    // Then color.r = 0.3
    EXPECT_NEAR(color.r, 0.3, 1e-10);

    // And color.g = 0.2
    EXPECT_NEAR(color.g, 0.2, 1e-10);

    // And color.b = 0.4
    EXPECT_NEAR(color.b, 0.4, 1e-10);
}

/**
 * Scenario: Adding colors
 */
TEST(Color, AddingColors) {
    // Given color is a Color(0.9, 0.6, 0.75)
    Color color(0.9, 0.6, 0.75);

    // And color1 is a Color(0.7, 0.1, 0.25)
    Color color1(0.7, 0.1, 0.25);

    // When result is color + color1
    Color result = color + color1;

    // Then result = Color(1.6, 0.7, 1.0)
    EXPECT_TRUE(result == Color(1.6, 0.7, 1.0));
}

/**
 * Scenario: Multiplying color by scalar
 */
TEST(Color, MultiplyingColorByScalar) {
    // Given color1 is a Color(0.2, 0.3, 0.4)
    Color color1(0.2, 0.3, 0.4);

    // When result1 is color1 * 2
    Color result1 = color1 * 2;

    // Then result1 = Color(0.4, 0.6, 0.8)
    EXPECT_TRUE(result1 == Color(0.4, 0.6, 0.8));

    // Given color2 is a Color(0.8, 1.2, 1.6)
    Color color2(0.8, 1.2, 1.6);

    // When result2 is color2 * 0.5
    Color result2 = color2 * 0.5;

    // Then result2 = Color(0.4, 0.6, 0.8)
    EXPECT_TRUE(result2 == Color(0.4, 0.6, 0.8));
}

/**
 * Scenario: Hadamard product (component-wise multiplication)
 */
TEST(Color, HadamardProduct) {
    // Given color is a Color(1.0, 0.2, 0.4)
    Color color(1.0, 0.2, 0.4);

    // And color1 is a Color(0.9, 1, 0.1)
    Color color1(0.9, 1, 0.1);

    // When result is color * color1
    Color result = color * color1;

    // Then result = Color(0.9, 0.2, 0.04)
    EXPECT_TRUE(result == Color(0.9, 0.2, 0.04));
}

/**
 * Scenario: Clamping colors to [0, 1] range
 */
TEST(Color, ClampingColors) {
    // Given color1 is a Color(0.1, 0.2, 0.3)
    Color color1(0.1, 0.2, 0.3);

    // When result1 is color1.clamped()
    Color result1 = color1.clamped();

    // Then result1 = Color(0.1, 0.2, 0.3) (no clamping needed)
    EXPECT_TRUE(result1 == Color(0.1, 0.2, 0.3));

    // Given color2 is a Color(-0.1, -0.2, 0.3)
    Color color2(-0.1, -0.2, 0.3);

    // When result2 is color2.clamped()
    Color result2 = color2.clamped();

    // Then result2 = Color(0.0, 0.0, 0.3) (negative values clamped to 0)
    EXPECT_TRUE(result2 == Color(0.0, 0.0, 0.3));

    // Given color3 is a Color(0.1, 1.2, 1.3)
    Color color3(0.1, 1.2, 1.3);

    // When result3 is color3.clamped()
    Color result3 = color3.clamped();

    // Then result3 = Color(0.1, 1.0, 1.0) (values > 1.0 clamped to 1.0)
    EXPECT_TRUE(result3 == Color(0.1, 1.0, 1.0));
}

/**
 * Scenario: Predefined standard colors
 */
TEST(Color, PredefinedColors) {
    // When black is Color::BLACK()
    Color black = Color::BLACK();
    // Then black = Color(0.0, 0.0, 0.0)
    EXPECT_TRUE(black == Color(0.0, 0.0, 0.0));

    // When white is Color::WHITE()
    Color white = Color::WHITE();
    // Then white = Color(1.0, 1.0, 1.0)
    EXPECT_TRUE(white == Color(1.0, 1.0, 1.0));

    // When red is Color::RED()
    Color red = Color::RED();
    // Then red = Color(1.0, 0.0, 0.0)
    EXPECT_TRUE(red == Color(1.0, 0.0, 0.0));

    // When green is Color::GREEN()
    Color green = Color::GREEN();
    // Then green = Color(0.0, 1.0, 0.0)
    EXPECT_TRUE(green == Color(0.0, 1.0, 0.0));

    // When blue is Color::BLUE()
    Color blue = Color::BLUE();
    // Then blue = Color(0.0, 0.0, 1.0)
    EXPECT_TRUE(blue == Color(0.0, 0.0, 1.0));
}

/**
 * Scenario: Color equality and inequality (with epsilon tolerance)
 */
TEST(Color, ColorEquality) {
    // Given color1 is a Color(0.5, 0.5, 0.5)
    Color color1(0.5, 0.5, 0.5);

    // And color2 is a Color(0.5, 0.5, 0.5)
    Color color2(0.5, 0.5, 0.5);

    // Then color1 == color2
    EXPECT_TRUE(color1 == color2);

    // Given color3 is a Color(0.5, 0.5, 0.5)
    Color color3(0.5, 0.5, 0.5);

    // And color4 is a Color(0.6, 0.5, 0.5)
    Color color4(0.6, 0.5, 0.5);

    // Then color3 != color4
    EXPECT_TRUE(color3 != color4);
}
