/** 
 * TestMain.cpp
 * 
 * Basis-Test-Datei für Google Test.
 * Enthält einen einfachen Test zur Verifikation der Test-Infrastruktur.
 */
#include <gtest/gtest.h>

// Einfache Additionsfunktion zum Testen
int add(int a, int b) {
    return a + b;
}

// Einfacher Test zur Verifikation der Test-Infrastruktur
TEST(BasicTest, AddFunction) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(-1, 1), 0);
}
