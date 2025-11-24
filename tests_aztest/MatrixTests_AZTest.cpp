/**
 * MatrixTests_AZTest.cpp
 *
 * Unit-Tests für die Matrix-Klasse mit AZTest-Framework
 */

#include <AZTest/AZTest.h>
#include "../src/Matrix/Matrix.h"
#include "../src/Vector/Vector.h"
#include "../src/Point/Point.h"
#include <cmath>

// ============================================================================
// Feature: Matrix - Instanz-Erzeugung
// ============================================================================

/**
 * Scenario: Constructing and inspecting a 4x4 matrix
 */
TEST(Matrix, ConstructingAndInspecting4x4Matrix) {
    // Given M is a 4x4 Matrix with specific values
    Matrix M(4, {
        1,    2,    3,    4,
        5.5,  6.5,  7.5,  8.5,
        9,    10,   11,   12,
        13.5, 14.5, 15.5, 16.5
    });

    // Then M[0][0] = 1
    EXPECT_NEAR(M[0][0], 1, 1e-10);

    // And M[0][3] = 4
    EXPECT_NEAR(M[0][3], 4, 1e-10);

    // And M[1][0] = 5.5
    EXPECT_NEAR(M[1][0], 5.5, 1e-10);

    // And M[1][2] = 7.5
    EXPECT_NEAR(M[1][2], 7.5, 1e-10);

    // And M[2][2] = 11
    EXPECT_NEAR(M[2][2], 11, 1e-10);

    // And M[3][0] = 13.5
    EXPECT_NEAR(M[3][0], 13.5, 1e-10);

    // And M[3][2] = 15.5
    EXPECT_NEAR(M[3][2], 15.5, 1e-10);
}

/**
 * Scenario: Representing a 2x2 matrix
 */
TEST(Matrix, Representing2x2Matrix) {
    // Given M is a 2x2 Matrix
    Matrix M(2, {
        -3,  5,
         1, -2
    });

    // Then M[0][0] = -3
    EXPECT_NEAR(M[0][0], -3, 1e-10);

    // And M[0][1] = 5
    EXPECT_NEAR(M[0][1], 5, 1e-10);

    // And M[1][0] = 1
    EXPECT_NEAR(M[1][0], 1, 1e-10);

    // And M[1][1] = -2
    EXPECT_NEAR(M[1][1], -2, 1e-10);
}

/**
 * Scenario: Representing a 3x3 matrix
 */
TEST(Matrix, Representing3x3Matrix) {
    // Given M is a 3x3 Matrix
    Matrix M(3, {
        -3,  5,  0,
         1, -2, -7,
         0,  1,  1
    });

    // Then M[0][0] = -3
    EXPECT_NEAR(M[0][0], -3, 1e-10);

    // And M[1][1] = -2
    EXPECT_NEAR(M[1][1], -2, 1e-10);

    // And M[2][2] = 1
    EXPECT_NEAR(M[2][2], 1, 1e-10);
}

/**
 * Scenario: Creating a 4x4 identity matrix
 */
TEST(Matrix, IdentityMatrix4x4) {
    // Given M is a 4x4 identity Matrix
    Matrix M = Matrix::identity(4);

    // Then diagonal elements are 1
    EXPECT_NEAR(M[0][0], 1, 1e-10);
    EXPECT_NEAR(M[1][1], 1, 1e-10);
    EXPECT_NEAR(M[2][2], 1, 1e-10);
    EXPECT_NEAR(M[3][3], 1, 1e-10);

    // And off-diagonal elements are 0
    EXPECT_NEAR(M[0][1], 0, 1e-10);
    EXPECT_NEAR(M[0][2], 0, 1e-10);
    EXPECT_NEAR(M[1][0], 0, 1e-10);
    EXPECT_NEAR(M[2][3], 0, 1e-10);
}

/**
 * Scenario: Creating a 3x3 identity matrix
 */
TEST(Matrix, IdentityMatrix3x3) {
    // Given M is a 3x3 identity Matrix
    Matrix M = Matrix::identity(3);

    // Then size is 3
    EXPECT_TRUE(M.getSize() == 3);

    // And diagonal elements are 1
    EXPECT_NEAR(M[0][0], 1, 1e-10);
    EXPECT_NEAR(M[1][1], 1, 1e-10);
    EXPECT_NEAR(M[2][2], 1, 1e-10);

    // And off-diagonal elements are 0
    EXPECT_NEAR(M[0][1], 0, 1e-10);
}

/**
 * Scenario: Creating a 2x2 identity matrix
 */
TEST(Matrix, IdentityMatrix2x2) {
    // Given M is a 2x2 identity Matrix
    Matrix M = Matrix::identity(2);

    // Then size is 2
    EXPECT_TRUE(M.getSize() == 2);

    // And diagonal elements are 1
    EXPECT_NEAR(M[0][0], 1, 1e-10);
    EXPECT_NEAR(M[1][1], 1, 1e-10);

    // And off-diagonal elements are 0
    EXPECT_NEAR(M[0][1], 0, 1e-10);
}

// ============================================================================
// Feature: Matrix - Vergleiche
// ============================================================================

/**
 * Scenario: Matrix equality with identical matrices
 */
TEST(Matrix, MatrixEqualityWithIdenticalMatrices) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    });

    // And B is an identical Matrix
    Matrix B(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    });

    // Then M == B
    EXPECT_TRUE(M == B);

    // And !(M != B)
    EXPECT_FALSE(M != B);
}

/**
 * Scenario: Matrix equality with different matrices
 */
TEST(Matrix, MatrixEqualityWithDifferentMatrices) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    });

    // And B is a different Matrix
    Matrix B(4, {
        2, 3, 4, 5,
        6, 7, 8, 9,
        8, 7, 6, 5,
        4, 3, 2, 1
    });

    // Then M != B
    EXPECT_TRUE(M != B);

    // And !(M == B)
    EXPECT_FALSE(M == B);
}

/**
 * Scenario: Matrix equality with different sizes
 */
TEST(Matrix, MatrixEqualityWithDifferentSizes) {
    // Given M is a 4x4 Matrix
    Matrix M(4);

    // And B is a 3x3 Matrix
    Matrix B(3);

    // Then M != B
    EXPECT_TRUE(M != B);

    // And !(M == B)
    EXPECT_FALSE(M == B);
}

// ============================================================================
// Feature: Matrix - Matrixmultiplikation
// ============================================================================

/**
 * Scenario: Multiplying two matrices
 */
TEST(Matrix, MultiplyingTwoMatrices) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    });

    // And B is another 4x4 Matrix
    Matrix B(4, {
        -2, 1, 2,  3,
         3, 2, 1, -1,
         4, 3, 6,  5,
         1, 2, 7,  8
    });

    // And expected is the expected result
    Matrix expected(4, {
        20, 22,  50,  48,
        44, 54, 114, 108,
        40, 58, 110, 102,
        16, 26,  46,  42
    });

    // When result is M * B
    Matrix result = M * B;

    // Then result == expected
    EXPECT_TRUE(result == expected);
}

/**
 * Scenario: Matrix multiplied by identity matrix
 */
TEST(Matrix, MatrixMultipliedByIdentityMatrix) {
    // Given M is a 4x4 identity Matrix
    Matrix M = Matrix::identity(4);

    // And B is a 4x4 Matrix
    Matrix B(4, {
        20, 22,  50,  48,
        44, 54, 114, 108,
        40, 58, 110, 102,
        16, 26,  46,  42
    });

    // Then M * B == B
    EXPECT_TRUE(M * B == B);

    // And B * M == B
    EXPECT_TRUE(B * M == B);
}

/**
 * Scenario: Multiplying matrices of different sizes throws exception
 */
TEST(Matrix, MultiplyingMatricesOfDifferentSizes) {
    // Given M is a 4x4 Matrix
    Matrix M(4);

    // And B is a 3x3 Matrix
    Matrix B(3);

    // When attempting M * B
    // Then an exception is thrown
    bool exceptionThrown = false;
    try {
        M * B;
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);
}

/**
 * Scenario: Multiplying 2x2 matrices
 */
TEST(Matrix, Multiplying2x2Matrices) {
    // Given A is a 2x2 Matrix
    Matrix A(2, {
        1, 2,
        3, 4
    });

    // And B is another 2x2 Matrix
    Matrix B(2, {
        5, 6,
        7, 8
    });

    // And expected is the expected result
    Matrix expected(2, {
        19, 22,
        43, 50
    });

    // Then A * B == expected
    EXPECT_TRUE(A * B == expected);
}

/**
 * Scenario: Matrix multiplied by vector
 */
TEST(Matrix, MatrixMultipliedByVector) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        1, 2, 3, 4,
        2, 4, 4, 2,
        8, 6, 4, 1,
        0, 0, 0, 1
    });

    // And v is a Vector
    Vector v(1, 2, 3);

    // And expected is the expected result
    Vector expected(14, 22, 32);

    // When result is M * v
    Vector result = M * v;

    // Then result == expected
    EXPECT_TRUE(result == expected);
}

/**
 * Scenario: Identity matrix multiplied by vector
 */
TEST(Matrix, IdentityMatrixMultipliedByVector) {
    // Given M is a 4x4 identity Matrix
    Matrix M = Matrix::identity(4);

    // And v is a Vector
    Vector v(1, 2, 3);

    // When result is M * v
    Vector result = M * v;

    // Then result == v
    EXPECT_TRUE(result == v);
}

/**
 * Scenario: Matrix multiplied by point
 */
TEST(Matrix, MatrixMultipliedByPoint) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        1, 2, 3, 4,
        2, 4, 4, 2,
        8, 6, 4, 1,
        0, 0, 0, 1
    });

    // And p is a Point
    Point p(1, 2, 3);

    // And expected is the expected result
    Point expected(18, 24, 33);

    // When result is M * p
    Point result = M * p;

    // Then result == expected
    EXPECT_TRUE(result == expected);
}

/**
 * Scenario: Identity matrix multiplied by point
 */
TEST(Matrix, IdentityMatrixMultipliedByPoint) {
    // Given M is a 4x4 identity Matrix
    Matrix M = Matrix::identity(4);

    // And p is a Point
    Point p(1, 2, 3);

    // When result is M * p
    Point result = M * p;

    // Then result == p
    EXPECT_TRUE(result == p);
}

// ============================================================================
// Feature: Matrix - Transponieren
// ============================================================================

/**
 * Scenario: Transposing a matrix
 */
TEST(Matrix, TransposingMatrix) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        0, 9, 3, 0,
        9, 8, 0, 8,
        1, 8, 5, 3,
        0, 0, 5, 8
    });

    // And expected is the expected transposed Matrix
    Matrix expected(4, {
        0, 9, 1, 0,
        9, 8, 8, 0,
        3, 0, 5, 5,
        0, 8, 3, 8
    });

    // When result is M.transpose()
    Matrix result = M.transpose();

    // Then result == expected
    EXPECT_TRUE(result == expected);
}

/**
 * Scenario: Transposing the identity matrix
 */
TEST(Matrix, TransposingIdentityMatrix) {
    // Given M is a 4x4 identity Matrix
    Matrix M = Matrix::identity(4);

    // And identity is a 4x4 identity Matrix
    Matrix identity = Matrix::identity(4);

    // When result is M.transpose()
    Matrix result = M.transpose();

    // Then result == identity
    EXPECT_TRUE(result == identity);
}

/**
 * Scenario: Transposing a 2x2 matrix
 */
TEST(Matrix, Transposing2x2Matrix) {
    // Given M is a 2x2 Matrix
    Matrix M(2, {
        1, 2,
        3, 4
    });

    // And expected is the expected transposed Matrix
    Matrix expected(2, {
        1, 3,
        2, 4
    });

    // Then M.transpose() == expected
    EXPECT_TRUE(M.transpose() == expected);
}

// ============================================================================
// Feature: Matrix - Invertieren (Determinanten)
// ============================================================================

/**
 * Scenario: Computing determinant of a 2x2 matrix
 */
TEST(Matrix, Determinant2x2Matrix) {
    // Given M is a 2x2 Matrix
    Matrix M(2, {
         1, 5,
        -3, 2
    });

    // Then determinant of M = 17
    EXPECT_NEAR(M.determinant(), 17, 1e-10);
}

/**
 * Scenario: Submatrix of a 3x3 is a 2x2
 */
TEST(Matrix, SubmatrixOf3x3Is2x2) {
    // Given M is a 3x3 Matrix
    Matrix M(3, {
         1, 5, 0,
        -3, 2, 7,
         0, 6, -3
    });

    // And expected is the expected 2x2 submatrix
    Matrix expected(2, {
        -3, 2,
         0, 6
    });

    // When result is M.submatrix(0, 2)
    Matrix result = M.submatrix(0, 2);

    // Then result == expected
    EXPECT_TRUE(result == expected);
}

/**
 * Scenario: Submatrix of a 4x4 is a 3x3
 */
TEST(Matrix, SubmatrixOf4x4Is3x3) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        -6, 1,  1, 6,
        -8, 5,  8, 6,
        -1, 0,  8, 2,
        -7, 1, -1, 1
    });

    // And expected is the expected 3x3 submatrix
    Matrix expected(3, {
        -6,  1, 6,
        -8,  8, 6,
        -7, -1, 1
    });

    // When result is M.submatrix(2, 1)
    Matrix result = M.submatrix(2, 1);

    // Then result == expected
    EXPECT_TRUE(result == expected);
}

/**
 * Scenario: Computing minor of a 3x3 matrix
 */
TEST(Matrix, MinorOf3x3Matrix) {
    // Given M is a 3x3 Matrix
    Matrix M(3, {
        3,  5,  0,
        2, -1, -7,
        6, -1,  5
    });

    // When B is M.submatrix(1, 0)
    Matrix B = M.submatrix(1, 0);

    // Then determinant of B = 25
    EXPECT_NEAR(B.determinant(), 25, 1e-10);

    // And minor(1, 0) of M = 25
    EXPECT_NEAR(M.minor(1, 0), 25, 1e-10);
}

/**
 * Scenario: Computing cofactor of a 3x3 matrix
 */
TEST(Matrix, CofactorOf3x3Matrix) {
    // Given M is a 3x3 Matrix
    Matrix M(3, {
        3,  5,  0,
        2, -1, -7,
        6, -1,  5
    });

    // Then minor(0, 0) of M = -12
    EXPECT_NEAR(M.minor(0, 0), -12, 1e-10);

    // And cofactor(0, 0) of M = -12
    EXPECT_NEAR(M.cofactor(0, 0), -12, 1e-10);

    // And minor(1, 0) of M = 25
    EXPECT_NEAR(M.minor(1, 0), 25, 1e-10);

    // And cofactor(1, 0) of M = -25
    EXPECT_NEAR(M.cofactor(1, 0), -25, 1e-10);
}

/**
 * Scenario: Computing determinant of a 3x3 matrix
 */
TEST(Matrix, Determinant3x3Matrix) {
    // Given M is a 3x3 Matrix
    Matrix M(3, {
         1, 2,  6,
        -5, 8, -4,
         2, 6,  4
    });

    // Then cofactor(0, 0) of M = 56
    EXPECT_NEAR(M.cofactor(0, 0), 56, 1e-10);

    // And cofactor(0, 1) of M = 12
    EXPECT_NEAR(M.cofactor(0, 1), 12, 1e-10);

    // And cofactor(0, 2) of M = -46
    EXPECT_NEAR(M.cofactor(0, 2), -46, 1e-10);

    // And determinant of M = -196
    EXPECT_NEAR(M.determinant(), -196, 1e-10);
}

/**
 * Scenario: Computing determinant of a 4x4 matrix
 */
TEST(Matrix, Determinant4x4Matrix) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        -2, -8,  3,  5,
        -3,  1,  7,  3,
         1,  2, -9,  6,
        -6,  7,  7, -9
    });

    // Then cofactor(0, 0) of M = 690
    EXPECT_NEAR(M.cofactor(0, 0), 690, 1e-10);

    // And cofactor(0, 1) of M = 447
    EXPECT_NEAR(M.cofactor(0, 1), 447, 1e-10);

    // And cofactor(0, 2) of M = 210
    EXPECT_NEAR(M.cofactor(0, 2), 210, 1e-10);

    // And cofactor(0, 3) of M = 51
    EXPECT_NEAR(M.cofactor(0, 3), 51, 1e-10);

    // And determinant of M = -4071
    EXPECT_NEAR(M.determinant(), -4071, 1e-10);
}

// ============================================================================
// Feature: Matrix - Invertieren (Invertierbarkeit)
// ============================================================================

/**
 * Scenario: Testing an invertible matrix
 */
TEST(Matrix, TestingInvertibleMatrix) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        6,  4,  4,  4,
        5,  5,  7,  6,
        4, -9,  3, -7,
        9,  1,  7, -6
    });

    // Then determinant of M = -2120
    EXPECT_NEAR(M.determinant(), -2120, 1e-10);

    // And M is invertible
    EXPECT_TRUE(M.isInvertible());
}

/**
 * Scenario: Testing a non-invertible matrix
 */
TEST(Matrix, TestingNonInvertibleMatrix) {
    // Given M is a 4x4 Matrix with zero determinant
    Matrix M(4, {
        -4,  2, -2, -3,
         9,  6,  2,  6,
         0, -5,  1, -5,
         0,  0,  0,  0
    });

    // Then determinant of M = 0
    EXPECT_NEAR(M.determinant(), 0, 1e-10);

    // And M is not invertible
    EXPECT_FALSE(M.isInvertible());
}

// ============================================================================
// Feature: Matrix - Invertieren (Inverse berechnen)
// ============================================================================

/**
 * Scenario: Calculating the inverse of a matrix
 */
TEST(Matrix, CalculatingInverseOfMatrix) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4
    });

    // When B is M.inverse()
    Matrix B = M.inverse();

    // Then determinant of M = 532
    EXPECT_NEAR(M.determinant(), 532, 1e-10);

    // And cofactor(2, 3) of M = -160
    EXPECT_NEAR(M.cofactor(2, 3), -160, 1e-10);

    // And B[3][2] = -160/532
    EXPECT_NEAR(B[3][2], -160.0/532.0, 1e-5);

    // And cofactor(3, 2) of M = 105
    EXPECT_NEAR(M.cofactor(3, 2), 105, 1e-10);

    // And B[2][3] = 105/532
    EXPECT_NEAR(B[2][3], 105.0/532.0, 1e-5);

    // And expected is the expected inverse Matrix
    Matrix expected(4, {
         0.21805,  0.45113,  0.24060, -0.04511,
        -0.80827, -1.45677, -0.44361,  0.52068,
        -0.07895, -0.22368, -0.05263,  0.19737,
        -0.52256, -0.81391, -0.30075,  0.30639
    });

    // Then B == expected
    EXPECT_TRUE(B == expected);
}

/**
 * Scenario: Calculating the inverse of another matrix
 */
TEST(Matrix, CalculatingInverseOfAnotherMatrix) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
         8, -5,  9,  2,
         7,  5,  6,  1,
        -6,  0,  9,  6,
        -3,  0, -9, -4
    });

    // And expected is the expected inverse Matrix
    Matrix expected(4, {
        -0.15385, -0.15385, -0.28205, -0.53846,
        -0.07692,  0.12308,  0.02564,  0.03077,
         0.35897,  0.35897,  0.43590,  0.92308,
        -0.69231, -0.69231, -0.76923, -1.92308
    });

    // When result is M.inverse()
    Matrix result = M.inverse();

    // Then result == expected
    EXPECT_TRUE(result == expected);
}

/**
 * Scenario: Calculating the inverse of a third matrix
 */
TEST(Matrix, CalculatingInverseOfThirdMatrix) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
         9,  3,  0,  9,
        -5, -2, -6, -3,
        -4,  9,  6,  4,
        -7,  6,  6,  2
    });

    // And expected is the expected inverse Matrix
    Matrix expected(4, {
        -0.04074, -0.07778,  0.14444, -0.22222,
        -0.07778,  0.03333,  0.36667, -0.33333,
        -0.02901, -0.14630, -0.10926,  0.12963,
         0.17778,  0.06667, -0.26667,  0.33333
    });

    // When result is M.inverse()
    Matrix result = M.inverse();

    // Then result == expected
    EXPECT_TRUE(result == expected);
}

/**
 * Scenario: Multiplying a product by its inverse
 */
TEST(Matrix, MultiplyingProductByItsInverse) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
         3, -9,  7,  3,
         3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1
    });

    // And B is another 4x4 Matrix
    Matrix B(4, {
        8,  2, 2, 2,
        3, -1, 7, 0,
        7,  0, 5, 4,
        6, -2, 0, 5
    });

    // When C is M * B
    Matrix C = M * B;

    // And result is C * B.inverse()
    Matrix result = C * B.inverse();

    // Then result == M
    EXPECT_TRUE(result == M);
}

/**
 * Scenario: Inverse of identity matrix is identity
 */
TEST(Matrix, InverseOfIdentityMatrix) {
    // Given M is a 4x4 identity Matrix
    Matrix M = Matrix::identity(4);

    // When inv is M.inverse()
    Matrix inv = M.inverse();

    // Then inv == M
    EXPECT_TRUE(inv == M);
}

/**
 * Scenario: Inverse of inverse is the original matrix
 */
TEST(Matrix, InverseOfInverseIsOriginal) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
         3, -9,  7,  3,
         3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1
    });

    // When inv is M.inverse()
    Matrix inv = M.inverse();

    // And invInv is inv.inverse()
    Matrix invInv = inv.inverse();

    // Then invInv == M
    EXPECT_TRUE(invInv == M);
}

// ============================================================================
// Feature: Matrix - Zusätzliche Tests
// ============================================================================

/**
 * Scenario: Transpose of inverse equals inverse of transpose
 */
TEST(Matrix, TransposeOfInverseEqualsInverseOfTranspose) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
         3, -9,  7,  3,
         3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1
    });

    // When inv is M.inverse()
    Matrix inv = M.inverse();

    // And transp is M.transpose()
    Matrix transp = M.transpose();

    // Then inv.transpose() == transp.inverse()
    EXPECT_TRUE(inv.transpose() == transp.inverse());
}

/**
 * Scenario: Matrix inverse caching works correctly
 */
TEST(Matrix, MatrixCaching) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4
    });

    // When inv1 is M.inverse() (first calculation)
    Matrix inv1 = M.inverse();

    // And inv2 is M.inverse() (should come from cache)
    Matrix inv2 = M.inverse();

    // Then inv1 == inv2
    EXPECT_TRUE(inv1 == inv2);
}

/**
 * Scenario: Matrix cache is invalidated on modification
 */
TEST(Matrix, MatrixCacheInvalidation) {
    // Given M is a 4x4 Matrix
    Matrix M(4, {
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4
    });

    // When inv1 is M.inverse()
    Matrix inv1 = M.inverse();

    // And M is modified at position (0, 0)
    M.set(0, 0, 10);

    // And inv2 is M.inverse() (new calculation after modification)
    Matrix inv2 = M.inverse();

    // Then inv1 != inv2 (cache was invalidated)
    EXPECT_FALSE(inv1 == inv2);
}

/**
 * Scenario: Inverting a non-invertible matrix throws exception
 */
TEST(Matrix, InvertingNonInvertibleMatrixThrows) {
    // Given M is a non-invertible 4x4 Matrix
    Matrix M(4, {
        -4,  2, -2, -3,
         9,  6,  2,  6,
         0, -5,  1, -5,
         0,  0,  0,  0
    });

    // When attempting to invert M
    // Then an exception is thrown
    bool exceptionThrown = false;
    try {
        M.inverse();
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);
}

// ============================================================================
// Feature: Matrix - View-Transformation
// ============================================================================

/**
 * Scenario: View transformation for default orientation
 * When the camera is at the origin looking along the negative z-axis,
 * the view transformation should be the identity matrix.
 */
TEST(Matrix, ViewTransformForDefaultOrientation) {
    // Given position is Point(0, 0, 0)
    Point position(0, 0, 0);

    // And lookAt is Point(0, 0, -1)
    Point lookAt(0, 0, -1);

    // And up is Vector(0, 1, 0)
    Vector up(0, 1, 0);

    // When transform is viewTransform(position, lookAt, up)
    Matrix transform = Matrix::viewTransform(position, lookAt, up);

    // Then transform == identity matrix
    EXPECT_TRUE(transform == Matrix::identity(4));
}

/**
 * Scenario: View transformation looking in positive z direction
 * When the camera looks in positive z direction (opposite to convention),
 * the world should be mirrored.
 */
TEST(Matrix, ViewTransformLookingInPositiveZDirection) {
    // Given position is Point(0, 0, 0)
    Point position(0, 0, 0);

    // And lookAt is Point(0, 0, 1)
    Point lookAt(0, 0, 1);

    // And up is Vector(0, 1, 0)
    Vector up(0, 1, 0);

    // When transform is viewTransform(position, lookAt, up)
    Matrix transform = Matrix::viewTransform(position, lookAt, up);

    // And expected is a scaling matrix that mirrors the world
    Matrix expected = Matrix::scale(-1, 1, -1);

    // Then transform == expected
    EXPECT_TRUE(transform == expected);
}

/**
 * Scenario: View transformation moves the world
 * The view transformation moves the world, not the camera.
 * When the camera is at z=8, the world is moved by -8.
 */
TEST(Matrix, ViewTransformMovesTheWorld) {
    // Given position is Point(0, 0, 8)
    Point position(0, 0, 8);

    // And lookAt is Point(0, 0, 0)
    Point lookAt(0, 0, 0);

    // And up is Vector(0, 1, 0)
    Vector up(0, 1, 0);

    // When transform is viewTransform(position, lookAt, up)
    Matrix transform = Matrix::viewTransform(position, lookAt, up);

    // And expected is a translation matrix
    Matrix expected = Matrix::translate(0, 0, -8);

    // Then transform == expected
    EXPECT_TRUE(transform == expected);
}

/**
 * Scenario: Arbitrary view transformation
 * Tests a complex view transformation with non-trivial values.
 */
TEST(Matrix, ArbitraryViewTransformation) {
    // Given position is Point(1, 3, 2)
    Point position(1, 3, 2);

    // And lookAt is Point(4, -2, 8)
    Point lookAt(4, -2, 8);

    // And up is Vector(1, 1, 0)
    Vector up(1, 1, 0);

    // When transform is viewTransform(position, lookAt, up)
    Matrix transform = Matrix::viewTransform(position, lookAt, up);

    // And expected is the expected transformation matrix
    Matrix expected(4, {
        -0.50709,  0.50709,  0.67612, -2.36643,
         0.76772,  0.60609,  0.12122, -2.82843,
        -0.35857,  0.59761, -0.71714,  0.00000,
         0.00000,  0.00000,  0.00000,  1.00000
    });

    // Then each element matches with tolerance
    const double epsilon = 0.05;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_NEAR(transform.at(i, j), expected.at(i, j), epsilon);
        }
    }
}

/**
 * Scenario: View transformation with different up vectors
 * The up vector doesn't need to be exactly perpendicular to the look direction.
 * Up vectors that only differ in the look-direction component lead to the same result.
 */
TEST(Matrix, ViewTransformWithDifferentUpVectors) {
    // Given position is Point(0, 0, -10)
    Point position(0, 0, -10);

    // And lookAt is Point(0, 0, 0)
    Point lookAt(0, 0, 0);

    // And up1 is Vector(0, 1, 0) (standard: y-axis)
    Vector up1(0, 1, 0);

    // And up2 is Vector(1, 1, 0) (diagonal in xy-plane)
    Vector up2(1, 1, 0);

    // And up3 is Vector(1, 0, 0) (x-axis, perpendicular to up1)
    Vector up3(1, 0, 0);

    // When creating view transformations with different up vectors
    Matrix transform1 = Matrix::viewTransform(position, lookAt, up1);
    Matrix transform2 = Matrix::viewTransform(position, lookAt, up2);
    Matrix transform3 = Matrix::viewTransform(position, lookAt, up3);

    // Then all transformations should be valid (invertible)
    EXPECT_TRUE(transform1.isInvertible());
    EXPECT_TRUE(transform2.isInvertible());
    EXPECT_TRUE(transform3.isInvertible());

    // And up1 and up2 produce different transformations
    EXPECT_FALSE(transform1 == transform2);

    // And up1 and up3 produce different transformations
    EXPECT_FALSE(transform1 == transform3);

    // And up2 and up3 produce different transformations
    EXPECT_FALSE(transform2 == transform3);
}

/**
 * Scenario: View transformation produces orthonormal basis
 * The basis produced by the view transformation should be orthonormal.
 */
TEST(Matrix, ViewTransformProducesOrthonormalBasis) {
    // Given position is Point(5, 5, 5)
    Point position(5, 5, 5);

    // And lookAt is Point(0, 0, 0)
    Point lookAt(0, 0, 0);

    // And up is Vector(0, 1, 0)
    Vector up(0, 1, 0);

    // When transform is viewTransform(position, lookAt, up)
    Matrix transform = Matrix::viewTransform(position, lookAt, up);

    // And extracting the basis vectors (first 3 rows)
    Vector right(transform.at(0, 0), transform.at(0, 1), transform.at(0, 2));
    Vector trueUp(transform.at(1, 0), transform.at(1, 1), transform.at(1, 2));
    Vector vpn(transform.at(2, 0), transform.at(2, 1), transform.at(2, 2));

    // Then all should be unit vectors
    const double epsilon = 1e-5;
    EXPECT_NEAR(right.magnitude(), 1.0, epsilon);
    EXPECT_NEAR(trueUp.magnitude(), 1.0, epsilon);
    EXPECT_NEAR(vpn.magnitude(), 1.0, epsilon);

    // And all should be orthogonal (dot product = 0)
    EXPECT_NEAR(Vector::dot(right, trueUp), 0.0, epsilon);
    EXPECT_NEAR(Vector::dot(right, vpn), 0.0, epsilon);
    EXPECT_NEAR(Vector::dot(trueUp, vpn), 0.0, epsilon);
}

/**
 * Scenario: View transformation at different camera positions
 */
TEST(Matrix, ViewTransformAtDifferentPositions) {
    // Given lookAt is Point(0, 0, 0)
    Point lookAt(0, 0, 0);

    // And up is Vector(0, 1, 0)
    Vector up(0, 1, 0);

    // When camera is on the left at Point(-10, 0, 0)
    Matrix transform1 = Matrix::viewTransform(Point(-10, 0, 0), lookAt, up);
    // Then transform1 is invertible
    EXPECT_TRUE(transform1.isInvertible());

    // When camera is on the right at Point(10, 0, 0)
    Matrix transform2 = Matrix::viewTransform(Point(10, 0, 0), lookAt, up);
    // Then transform2 is invertible
    EXPECT_TRUE(transform2.isInvertible());

    // When camera is above at Point(0, 10, 0)
    Matrix transform3 = Matrix::viewTransform(Point(0, 10, 0), lookAt, Vector(0, 0, -1));
    // Then transform3 is invertible
    EXPECT_TRUE(transform3.isInvertible());

    // When camera is below at Point(0, -10, 0)
    Matrix transform4 = Matrix::viewTransform(Point(0, -10, 0), lookAt, Vector(0, 0, 1));
    // Then transform4 is invertible
    EXPECT_TRUE(transform4.isInvertible());
}
