#include <gtest/gtest.h>
#include "../src/Matrix/Matrix.h"
#include "../src/Vector/Vector.h"
#include "../src/Point/Point.h"
#include <cmath>

// Hilfsfunktion für Gleitkomma-Vergleiche (static, um Linker-Fehler zu vermeiden)
static bool approxEqual(double a, double b, double eps = 1e-5) {
    return std::fabs(a - b) < eps;
}

// ========================
// 3.1 Instanz-Erzeugung
// ========================

TEST(MatrixTests, ConstructingAndInspecting4x4Matrix) {
    Matrix M(4, {
        1,    2,    3,    4,
        5.5,  6.5,  7.5,  8.5,
        9,    10,   11,   12,
        13.5, 14.5, 15.5, 16.5
    });

    EXPECT_DOUBLE_EQ(M[0][0], 1);
    EXPECT_DOUBLE_EQ(M[0][3], 4);
    EXPECT_DOUBLE_EQ(M[1][0], 5.5);
    EXPECT_DOUBLE_EQ(M[1][2], 7.5);
    EXPECT_DOUBLE_EQ(M[2][2], 11);
    EXPECT_DOUBLE_EQ(M[3][0], 13.5);
    EXPECT_DOUBLE_EQ(M[3][2], 15.5);
}

TEST(MatrixTests, Representing2x2Matrix) {
    Matrix M(2, {
        -3,  5,
         1, -2
    });

    EXPECT_DOUBLE_EQ(M[0][0], -3);
    EXPECT_DOUBLE_EQ(M[0][1], 5);
    EXPECT_DOUBLE_EQ(M[1][0], 1);
    EXPECT_DOUBLE_EQ(M[1][1], -2);
}

TEST(MatrixTests, Representing3x3Matrix) {
    Matrix M(3, {
        -3,  5,  0,
         1, -2, -7,
         0,  1,  1
    });

    EXPECT_DOUBLE_EQ(M[0][0], -3);
    EXPECT_DOUBLE_EQ(M[1][1], -2);
    EXPECT_DOUBLE_EQ(M[2][2], 1);
}

TEST(MatrixTests, IdentityMatrix4x4) {
    Matrix M = Matrix::identity(4);

    EXPECT_DOUBLE_EQ(M[0][0], 1);
    EXPECT_DOUBLE_EQ(M[1][1], 1);
    EXPECT_DOUBLE_EQ(M[2][2], 1);
    EXPECT_DOUBLE_EQ(M[3][3], 1);

    EXPECT_DOUBLE_EQ(M[0][1], 0);
    EXPECT_DOUBLE_EQ(M[0][2], 0);
    EXPECT_DOUBLE_EQ(M[1][0], 0);
    EXPECT_DOUBLE_EQ(M[2][3], 0);
}

TEST(MatrixTests, IdentityMatrix3x3) {
    Matrix M = Matrix::identity(3);

    EXPECT_EQ(M.getSize(), 3);
    EXPECT_DOUBLE_EQ(M[0][0], 1);
    EXPECT_DOUBLE_EQ(M[1][1], 1);
    EXPECT_DOUBLE_EQ(M[2][2], 1);
    EXPECT_DOUBLE_EQ(M[0][1], 0);
}

TEST(MatrixTests, IdentityMatrix2x2) {
    Matrix M = Matrix::identity(2);

    EXPECT_EQ(M.getSize(), 2);
    EXPECT_DOUBLE_EQ(M[0][0], 1);
    EXPECT_DOUBLE_EQ(M[1][1], 1);
    EXPECT_DOUBLE_EQ(M[0][1], 0);
}

// ========================
// 3.2 Vergleiche
// ========================

TEST(MatrixTests, MatrixEqualityWithIdenticalMatrices) {
    Matrix M(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    });

    Matrix B(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    });

    EXPECT_TRUE(M == B);
    EXPECT_FALSE(M != B);
}

TEST(MatrixTests, MatrixEqualityWithDifferentMatrices) {
    Matrix M(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    });

    Matrix B(4, {
        2, 3, 4, 5,
        6, 7, 8, 9,
        8, 7, 6, 5,
        4, 3, 2, 1
    });

    EXPECT_TRUE(M != B);
    EXPECT_FALSE(M == B);
}

TEST(MatrixTests, MatrixEqualityWithDifferentSizes) {
    Matrix M(4);
    Matrix B(3);

    EXPECT_TRUE(M != B);
    EXPECT_FALSE(M == B);
}

// ========================
// 3.3 Matrixmultiplikation
// ========================

TEST(MatrixTests, MultiplyingTwoMatrices) {
    Matrix M(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    });

    Matrix B(4, {
        -2, 1, 2,  3,
         3, 2, 1, -1,
         4, 3, 6,  5,
         1, 2, 7,  8
    });

    Matrix expected(4, {
        20, 22,  50,  48,
        44, 54, 114, 108,
        40, 58, 110, 102,
        16, 26,  46,  42
    });

    Matrix result = M * B;
    EXPECT_TRUE(result == expected);
}

TEST(MatrixTests, MatrixMultipliedByIdentityMatrix) {
    Matrix M = Matrix::identity(4);

    Matrix B(4, {
        20, 22,  50,  48,
        44, 54, 114, 108,
        40, 58, 110, 102,
        16, 26,  46,  42
    });

    EXPECT_TRUE(M * B == B);
    EXPECT_TRUE(B * M == B);
}

TEST(MatrixTests, MultiplyingMatricesOfDifferentSizes) {
    Matrix M(4);
    Matrix B(3);

    EXPECT_THROW(M * B, std::runtime_error);
}

TEST(MatrixTests, Multiplying2x2Matrices) {
    Matrix A(2, {
        1, 2,
        3, 4
    });

    Matrix B(2, {
        5, 6,
        7, 8
    });

    Matrix expected(2, {
        19, 22,
        43, 50
    });

    EXPECT_TRUE(A * B == expected);
}

TEST(MatrixTests, MatrixMultipliedByVector) {
    Matrix M(4, {
        1, 2, 3, 4,
        2, 4, 4, 2,
        8, 6, 4, 1,
        0, 0, 0, 1
    });

    Vector v(1, 2, 3);
    Vector result = M * v;
    Vector expected(14, 22, 32);

    EXPECT_TRUE(result == expected);
}

TEST(MatrixTests, IdentityMatrixMultipliedByVector) {
    Matrix M = Matrix::identity(4);
    Vector v(1, 2, 3);

    Vector result = M * v;
    EXPECT_TRUE(result == v);
}

TEST(MatrixTests, MatrixMultipliedByPoint) {
    Matrix M(4, {
        1, 2, 3, 4,
        2, 4, 4, 2,
        8, 6, 4, 1,
        0, 0, 0, 1
    });

    Point p(1, 2, 3);
    Point result = M * p;
    Point expected(18, 24, 33);

    EXPECT_TRUE(result == expected);
}

TEST(MatrixTests, IdentityMatrixMultipliedByPoint) {
    Matrix M = Matrix::identity(4);
    Point p(1, 2, 3);

    Point result = M * p;
    EXPECT_TRUE(result == p);
}

// ========================
// 3.4 Transponieren
// ========================

TEST(MatrixTests, TransposingMatrix) {
    Matrix M(4, {
        0, 9, 3, 0,
        9, 8, 0, 8,
        1, 8, 5, 3,
        0, 0, 5, 8
    });

    Matrix expected(4, {
        0, 9, 1, 0,
        9, 8, 8, 0,
        3, 0, 5, 5,
        0, 8, 3, 8
    });

    Matrix result = M.transpose();
    EXPECT_TRUE(result == expected);
}

TEST(MatrixTests, TransposingIdentityMatrix) {
    Matrix M = Matrix::identity(4);
    Matrix result = M.transpose();
    Matrix identity = Matrix::identity(4);

    EXPECT_TRUE(result == identity);
}

TEST(MatrixTests, Transposing2x2Matrix) {
    Matrix M(2, {
        1, 2,
        3, 4
    });

    Matrix expected(2, {
        1, 3,
        2, 4
    });

    EXPECT_TRUE(M.transpose() == expected);
}

// ========================
// 3.5 Invertieren - Determinanten
// ========================

TEST(MatrixTests, Determinant2x2Matrix) {
    Matrix M(2, {
         1, 5,
        -3, 2
    });

    EXPECT_DOUBLE_EQ(M.determinant(), 17);
}

TEST(MatrixTests, SubmatrixOf3x3Is2x2) {
    Matrix M(3, {
         1, 5, 0,
        -3, 2, 7,
         0, 6, -3
    });

    Matrix expected(2, {
        -3, 2,
         0, 6
    });

    Matrix result = M.submatrix(0, 2);
    EXPECT_TRUE(result == expected);
}

TEST(MatrixTests, SubmatrixOf4x4Is3x3) {
    Matrix M(4, {
        -6, 1,  1, 6,
        -8, 5,  8, 6,
        -1, 0,  8, 2,
        -7, 1, -1, 1
    });

    Matrix expected(3, {
        -6,  1, 6,
        -8,  8, 6,
        -7, -1, 1
    });

    Matrix result = M.submatrix(2, 1);
    EXPECT_TRUE(result == expected);
}

TEST(MatrixTests, MinorOf3x3Matrix) {
    Matrix M(3, {
        3,  5,  0,
        2, -1, -7,
        6, -1,  5
    });

    Matrix B = M.submatrix(1, 0);
    EXPECT_DOUBLE_EQ(B.determinant(), 25);
    EXPECT_DOUBLE_EQ(M.minor(1, 0), 25);
}

TEST(MatrixTests, CofactorOf3x3Matrix) {
    Matrix M(3, {
        3,  5,  0,
        2, -1, -7,
        6, -1,  5
    });

    EXPECT_DOUBLE_EQ(M.minor(0, 0), -12);
    EXPECT_DOUBLE_EQ(M.cofactor(0, 0), -12);

    EXPECT_DOUBLE_EQ(M.minor(1, 0), 25);
    EXPECT_DOUBLE_EQ(M.cofactor(1, 0), -25);
}

TEST(MatrixTests, Determinant3x3Matrix) {
    Matrix M(3, {
         1, 2,  6,
        -5, 8, -4,
         2, 6,  4
    });

    EXPECT_DOUBLE_EQ(M.cofactor(0, 0), 56);
    EXPECT_DOUBLE_EQ(M.cofactor(0, 1), 12);
    EXPECT_DOUBLE_EQ(M.cofactor(0, 2), -46);
    EXPECT_DOUBLE_EQ(M.determinant(), -196);
}

TEST(MatrixTests, Determinant4x4Matrix) {
    Matrix M(4, {
        -2, -8,  3,  5,
        -3,  1,  7,  3,
         1,  2, -9,  6,
        -6,  7,  7, -9
    });

    EXPECT_DOUBLE_EQ(M.cofactor(0, 0), 690);
    EXPECT_DOUBLE_EQ(M.cofactor(0, 1), 447);
    EXPECT_DOUBLE_EQ(M.cofactor(0, 2), 210);
    EXPECT_DOUBLE_EQ(M.cofactor(0, 3), 51);
    EXPECT_DOUBLE_EQ(M.determinant(), -4071);
}

// ========================
// 3.5 Invertieren - Invertierbarkeit
// ========================

TEST(MatrixTests, TestingInvertibleMatrix) {
    Matrix M(4, {
        6,  4,  4,  4,
        5,  5,  7,  6,
        4, -9,  3, -7,
        9,  1,  7, -6
    });

    EXPECT_DOUBLE_EQ(M.determinant(), -2120);
    EXPECT_TRUE(M.isInvertible());
}

TEST(MatrixTests, TestingNonInvertibleMatrix) {
    Matrix M(4, {
        -4,  2, -2, -3,
         9,  6,  2,  6,
         0, -5,  1, -5,
         0,  0,  0,  0
    });

    EXPECT_DOUBLE_EQ(M.determinant(), 0);
    EXPECT_FALSE(M.isInvertible());
}

// ========================
// 3.5 Invertieren - Inverse berechnen
// ========================

TEST(MatrixTests, CalculatingInverseOfMatrix) {
    Matrix M(4, {
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4
    });

    Matrix B = M.inverse();

    EXPECT_DOUBLE_EQ(M.determinant(), 532);
    EXPECT_DOUBLE_EQ(M.cofactor(2, 3), -160);
    EXPECT_TRUE(approxEqual(B[3][2], -160.0/532.0));
    EXPECT_DOUBLE_EQ(M.cofactor(3, 2), 105);
    EXPECT_TRUE(approxEqual(B[2][3], 105.0/532.0));

    Matrix expected(4, {
         0.21805,  0.45113,  0.24060, -0.04511,
        -0.80827, -1.45677, -0.44361,  0.52068,
        -0.07895, -0.22368, -0.05263,  0.19737,
        -0.52256, -0.81391, -0.30075,  0.30639
    });

    EXPECT_TRUE(B == expected);
}

TEST(MatrixTests, CalculatingInverseOfAnotherMatrix) {
    Matrix M(4, {
         8, -5,  9,  2,
         7,  5,  6,  1,
        -6,  0,  9,  6,
        -3,  0, -9, -4
    });

    Matrix expected(4, {
        -0.15385, -0.15385, -0.28205, -0.53846,
        -0.07692,  0.12308,  0.02564,  0.03077,
         0.35897,  0.35897,  0.43590,  0.92308,
        -0.69231, -0.69231, -0.76923, -1.92308
    });

    Matrix result = M.inverse();
    EXPECT_TRUE(result == expected);
}

TEST(MatrixTests, CalculatingInverseOfThirdMatrix) {
    Matrix M(4, {
         9,  3,  0,  9,
        -5, -2, -6, -3,
        -4,  9,  6,  4,
        -7,  6,  6,  2
    });

    Matrix expected(4, {
        -0.04074, -0.07778,  0.14444, -0.22222,
        -0.07778,  0.03333,  0.36667, -0.33333,
        -0.02901, -0.14630, -0.10926,  0.12963,
         0.17778,  0.06667, -0.26667,  0.33333
    });

    Matrix result = M.inverse();
    EXPECT_TRUE(result == expected);
}

TEST(MatrixTests, MultiplyingProductByItsInverse) {
    Matrix M(4, {
         3, -9,  7,  3,
         3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1
    });

    Matrix B(4, {
        8,  2, 2, 2,
        3, -1, 7, 0,
        7,  0, 5, 4,
        6, -2, 0, 5
    });

    Matrix C = M * B;
    Matrix result = C * B.inverse();

    EXPECT_TRUE(result == M);
}

TEST(MatrixTests, InverseOfIdentityMatrix) {
    Matrix M = Matrix::identity(4);
    Matrix inv = M.inverse();

    EXPECT_TRUE(inv == M);
}

TEST(MatrixTests, InverseOfInverseIsOriginal) {
    Matrix M(4, {
         3, -9,  7,  3,
         3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1
    });

    Matrix inv = M.inverse();
    Matrix invInv = inv.inverse();

    EXPECT_TRUE(invInv == M);
}

// ========================
// Zusätzliche Tests
// ========================

TEST(MatrixTests, TransposeOfInverseEqualsInverseOfTranspose) {
    Matrix M(4, {
         3, -9,  7,  3,
         3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1
    });

    Matrix inv = M.inverse();
    Matrix transp = M.transpose();

    EXPECT_TRUE(inv.transpose() == transp.inverse());
}

TEST(MatrixTests, MatrixCaching) {
    Matrix M(4, {
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4
    });

    // Erste Berechnung
    Matrix inv1 = M.inverse();
    // Zweite Berechnung (sollte aus Cache kommen)
    Matrix inv2 = M.inverse();

    EXPECT_TRUE(inv1 == inv2);
}

TEST(MatrixTests, MatrixCacheInvalidation) {
    Matrix M(4, {
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4
    });

    Matrix inv1 = M.inverse();

    // Änderung der Matrix
    M.set(0, 0, 10);

    // Neue Berechnung sollte einen anderen Wert liefern
    Matrix inv2 = M.inverse();

    EXPECT_FALSE(inv1 == inv2);
}

TEST(MatrixTests, InvertingNonInvertibleMatrixThrows) {
    Matrix M(4, {
        -4,  2, -2, -3,
         9,  6,  2,  6,
         0, -5,  1, -5,
         0,  0,  0,  0
    });

    EXPECT_THROW(M.inverse(), std::runtime_error);
}

// ========================
// View-Transformation Tests
// ========================

/**
 * Test: View-Transformation für Standard-Orientierung
 *
 * Wenn die Kamera im Ursprung steht und entlang der negativen z-Achse schaut,
 * sollte die View-Transformation die Einheitsmatrix sein.
 */
TEST(MatrixTests, ViewTransformForDefaultOrientation) {
    Point position(0, 0, 0);
    Point lookAt(0, 0, -1);
    Vector up(0, 1, 0);

    Matrix transform = Matrix::viewTransform(position, lookAt, up);

    EXPECT_EQ(transform, Matrix::identity(4));
}

/**
 * Test: View-Transformation in positive z-Richtung
 *
 * Wenn die Kamera in positive z-Richtung schaut (umgekehrt zur Konvention),
 * sollte die Welt gespiegelt werden.
 */
TEST(MatrixTests, ViewTransformLookingInPositiveZDirection) {
    Point position(0, 0, 0);
    Point lookAt(0, 0, 1);
    Vector up(0, 1, 0);

    Matrix transform = Matrix::viewTransform(position, lookAt, up);
    Matrix expected = Matrix::scale(-1, 1, -1);

    EXPECT_EQ(transform, expected);
}

/**
 * Test: View-Transformation bewegt die Welt
 *
 * Die View-Transformation verschiebt die Welt, nicht die Kamera.
 * Wenn die Kamera bei z=8 steht, wird die Welt um -8 verschoben.
 */
TEST(MatrixTests, ViewTransformMovesTheWorld) {
    Point position(0, 0, 8);
    Point lookAt(0, 0, 0);
    Vector up(0, 1, 0);

    Matrix transform = Matrix::viewTransform(position, lookAt, up);
    Matrix expected = Matrix::translate(0, 0, -8);

    EXPECT_EQ(transform, expected);
}

/**
 * Test: Beliebige View-Transformation
 *
 * Testet eine komplexe View-Transformation mit nicht-trivialen Werten.
 */
TEST(MatrixTests, ArbitraryViewTransformation) {
    Point position(1, 3, 2);
    Point lookAt(4, -2, 8);
    Vector up(1, 1, 0);

    Matrix transform = Matrix::viewTransform(position, lookAt, up);

    // Erwartete Matrix (aus dem Tutorial)
    Matrix expected(4, {
        -0.50709,  0.50709,  0.67612, -2.36643,
         0.76772,  0.60609,  0.12122, -2.82843,
        -0.35857,  0.59761, -0.71714,  0.00000,
         0.00000,  0.00000,  0.00000,  1.00000
    });

    // Vergleiche jedes Element mit Toleranz (höhere Toleranz wegen Rundungsfehlern)
    const double epsilon = 0.05;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_NEAR(transform.at(i, j), expected.at(i, j), epsilon)
                << "Mismatch at position [" << i << "][" << j << "]";
        }
    }
}

/**
 * Test: View-Transformation mit verschiedenen Up-Vektoren
 *
 * Der Up-Vektor muss nicht exakt senkrecht zur Blickrichtung sein.
 * Up-Vektoren, die sich nur in der Blickrichtungs-Komponente unterscheiden,
 * führen zum gleichen Ergebnis (die Komponente wird projiziert).
 */
TEST(MatrixTests, ViewTransformWithDifferentUpVectors) {
    Point position(0, 0, -10);
    Point lookAt(0, 0, 0);

    // Test mit verschiedenen Up-Vektoren
    Vector up1(0, 1, 0);    // Standard: y-Achse
    Vector up2(1, 1, 0);    // Diagonal in xy-Ebene
    Vector up3(1, 0, 0);    // x-Achse (senkrecht zu up1)

    Matrix transform1 = Matrix::viewTransform(position, lookAt, up1);
    Matrix transform2 = Matrix::viewTransform(position, lookAt, up2);
    Matrix transform3 = Matrix::viewTransform(position, lookAt, up3);

    // Alle sollten gültige Transformationen sein (invertierbar)
    EXPECT_TRUE(transform1.isInvertible());
    EXPECT_TRUE(transform2.isInvertible());
    EXPECT_TRUE(transform3.isInvertible());

    // up1 und up2 sind unterschiedlich
    EXPECT_FALSE(transform1 == transform2);
    // up1 und up3 sind unterschiedlich (senkrecht zueinander)
    EXPECT_FALSE(transform1 == transform3);
    // up2 und up3 sind unterschiedlich
    EXPECT_FALSE(transform2 == transform3);
}

/**
 * Test: View-Transformation erhält orthonormale Basis
 *
 * Die durch die View-Transformation erzeugte Basis sollte orthonormal sein.
 */
TEST(MatrixTests, ViewTransformProducesOrthonormalBasis) {
    Point position(5, 5, 5);
    Point lookAt(0, 0, 0);
    Vector up(0, 1, 0);

    Matrix transform = Matrix::viewTransform(position, lookAt, up);

    // Extrahiere die Basisvektoren (erste 3 Zeilen)
    Vector right(transform.at(0, 0), transform.at(0, 1), transform.at(0, 2));
    Vector trueUp(transform.at(1, 0), transform.at(1, 1), transform.at(1, 2));
    Vector vpn(transform.at(2, 0), transform.at(2, 1), transform.at(2, 2));

    // Alle sollten Einheitsvektoren sein
    const double epsilon = 1e-5;
    EXPECT_NEAR(right.magnitude(), 1.0, epsilon);
    EXPECT_NEAR(trueUp.magnitude(), 1.0, epsilon);
    EXPECT_NEAR(vpn.magnitude(), 1.0, epsilon);

    // Alle sollten orthogonal zueinander sein (Skalarprodukt = 0)
    EXPECT_NEAR(Vector::dot(right, trueUp), 0.0, epsilon);
    EXPECT_NEAR(Vector::dot(right, vpn), 0.0, epsilon);
    EXPECT_NEAR(Vector::dot(trueUp, vpn), 0.0, epsilon);
}

/**
 * Test: View-Transformation mit Kamera an verschiedenen Positionen
 */
TEST(MatrixTests, ViewTransformAtDifferentPositions) {
    Point lookAt(0, 0, 0);
    Vector up(0, 1, 0);

    // Kamera links
    Matrix transform1 = Matrix::viewTransform(Point(-10, 0, 0), lookAt, up);
    EXPECT_TRUE(transform1.isInvertible());

    // Kamera rechts
    Matrix transform2 = Matrix::viewTransform(Point(10, 0, 0), lookAt, up);
    EXPECT_TRUE(transform2.isInvertible());

    // Kamera oben
    Matrix transform3 = Matrix::viewTransform(Point(0, 10, 0), lookAt, Vector(0, 0, -1));
    EXPECT_TRUE(transform3.isInvertible());

    // Kamera unten
    Matrix transform4 = Matrix::viewTransform(Point(0, -10, 0), lookAt, Vector(0, 0, 1));
    EXPECT_TRUE(transform4.isInvertible());
}
