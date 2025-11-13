#pragma once
#include "../Vector/Vector.h"
#include "../Point/Point.h"
#include <vector>
#include <stdexcept>
#include <cmath>

/**
 * Matrix-Klasse
 *
 * Repräsentiert quadratische n×n Matrizen für 3D-Transformationen.
 * Hauptsächlich werden 4×4 Matrizen für homogene Koordinaten verwendet.
 *
 * Operationen:
 * - Matrixmultiplikation (Matrix × Matrix, Matrix × Vektor, Matrix × Punkt)
 * - Transponieren (Spiegelung an Hauptdiagonale)
 * - Invertieren (mit Determinanten und Adjunkte)
 *
 * Indizierung: Zeile zuerst, beginnend bei 0
 * Beispiel: M[2][3] ist das Element in Zeile 2, Spalte 3
 */
class Matrix {
private:
    int size_;                          // Dimension der Matrix (n für n×n)
    std::vector<std::vector<double>> data_;  // Speicherung als 2D-Vektor

    mutable bool inverseCalculated_;     // Flag für Lazy Evaluation
    mutable Matrix* cachedInverse_;      // Gecachte inverse Matrix

public:
    /**
     * Konstruktor für n×n Matrix
     * @param size Dimension der Matrix (Standard: 4)
     *
     * Initialisiert alle Elemente mit 0.
     */
    explicit Matrix(int size = 4);

    /**
     * Konstruktor für Matrix mit Initialisierungsliste
     * @param size Dimension der Matrix
     * @param values Flache Liste der Werte (zeilenweise)
     *
     * Beispiel für 2×2 Matrix:
     * Matrix(2, {1, 2, 3, 4}) ergibt:
     * | 1  2 |
     * | 3  4 |
     */
    Matrix(int size, const std::vector<double>& values);

    /**
     * Copy-Konstruktor
     */
    Matrix(const Matrix& other);

    /**
     * Destruktor
     */
    ~Matrix();

    /**
     * Zuweisungsoperator
     */
    Matrix& operator=(const Matrix& other);

    /**
     * Gibt die Dimension der Matrix zurück
     * @return Größe n der n×n Matrix
     */
    int getSize() const { return size_; }

    /**
     * Elementzugriff (lesend)
     * @param row Zeilenindex (0-basiert)
     * @param col Spaltenindex (0-basiert)
     * @return Wert an Position [row][col]
     */
    double at(int row, int col) const;

    /**
     * Elementzugriff (schreibend)
     * @param row Zeilenindex (0-basiert)
     * @param col Spaltenindex (0-basiert)
     * @param value Neuer Wert
     */
    void set(int row, int col, double value);

    /**
     * Operator für Elementzugriff [row][col]
     * @param row Zeilenindex
     * @return Zeiger auf die Zeile für weiteren Zugriff
     */
    std::vector<double>& operator[](int row) { return data_[row]; }
    const std::vector<double>& operator[](int row) const { return data_[row]; }

    /**
     * Gleichheitsvergleich mit Epsilon-Toleranz
     *
     * Vergleicht zwei Matrizen elementweise unter Berücksichtigung
     * von Gleitkomma-Ungenauigkeiten (eps = 1e-6).
     *
     * @param m Die zu vergleichende Matrix
     * @return true wenn die Matrizen (nahezu) gleich sind
     */
    bool operator==(const Matrix& m) const;

    /**
     * Ungleichheitsvergleich
     * @param m Die zu vergleichende Matrix
     * @return true wenn die Matrizen unterschiedlich sind
     */
    bool operator!=(const Matrix& m) const { return !(*this == m); }

    /**
     * Matrixmultiplikation: Matrix × Matrix
     *
     * Berechnet C = A × B, wobei this = A.
     * Beide Matrizen müssen die gleiche Größe haben.
     *
     * Algorithmus:
     * C[i][k] = Σ(j=0 bis n-1) A[i][j] × B[j][k]
     *
     * @param m Die rechte Matrix
     * @return Produktmatrix
     * @throws std::runtime_error bei unterschiedlichen Größen
     */
    Matrix operator*(const Matrix& m) const;

    /**
     * Matrix × Vektor
     *
     * Multipliziert eine 4×4 Matrix mit einem Vektor (Spaltenvektor).
     * Der Vektor wird als homogene Koordinaten mit w=0 behandelt.
     *
     * @param v Der zu transformierende Vektor
     * @return Transformierter Vektor
     * @throws std::runtime_error wenn Matrix nicht 4×4 ist
     */
    Vector operator*(const Vector& v) const;

    /**
     * Matrix × Punkt
     *
     * Multipliziert eine 4×4 Matrix mit einem Punkt (Spaltenvektor).
     * Der Punkt wird als homogene Koordinaten mit w=1 behandelt.
     *
     * @param p Der zu transformierende Punkt
     * @return Transformierter Punkt
     * @throws std::runtime_error wenn Matrix nicht 4×4 ist
     */
    Point operator*(const Point& p) const;

    /**
     * Transponierte der Matrix
     *
     * Spiegelt die Matrix an ihrer Hauptdiagonale.
     * Das Element M[i][j] wird zu M^T[j][i].
     *
     * @return Transponierte Matrix
     */
    Matrix transpose() const;

    /**
     * Untermatrix (für Determinantenberechnung)
     *
     * Erzeugt eine (n-1)×(n-1) Matrix durch Entfernen
     * der angegebenen Zeile und Spalte.
     *
     * @param removeRow Zu entfernende Zeile
     * @param removeCol Zu entfernende Spalte
     * @return Untermatrix
     */
    Matrix submatrix(int removeRow, int removeCol) const;

    /**
     * Minor (Unterdeterminante)
     *
     * Berechnet die Determinante der Untermatrix,
     * die entsteht, wenn Zeile row und Spalte col entfernt werden.
     *
     * @param row Zeilenindex
     * @param col Spaltenindex
     * @return Unterdeterminante
     */
    double minor(int row, int col) const;

    /**
     * Kofaktor
     *
     * Berechnet den Kofaktor für Position [row][col]:
     * cofactor = (-1)^(row+col) × minor(row, col)
     *
     * @param row Zeilenindex
     * @param col Spaltenindex
     * @return Kofaktor
     */
    double cofactor(int row, int col) const;

    /**
     * Determinante
     *
     * Berechnet die Determinante rekursiv mittels
     * Entwicklung nach der ersten Zeile.
     *
     * Rekursionsbasis: 2×2 Matrix
     * det(A) = a11×a22 - a12×a21
     *
     * Rekursion:
     * det(A) = Σ(j=0 bis n-1) a[0][j] × cofactor(0, j)
     *
     * @return Determinante
     */
    double determinant() const;

    /**
     * Prüft ob Matrix invertierbar ist
     *
     * Eine Matrix ist invertierbar gdw. ihre Determinante != 0.
     *
     * @return true wenn invertierbar
     */
    bool isInvertible() const;

    /**
     * Inverse Matrix (mit Lazy Evaluation)
     *
     * Berechnet die inverse Matrix mit der Formel:
     * A^(-1) = (1/det(A)) × adj(A)
     *
     * wobei adj(A) die Adjunkte (transponierte Kofaktorenmatrix) ist.
     *
     * Die Inverse wird gecacht und nur bei Bedarf neu berechnet.
     *
     * @return Inverse Matrix
     * @throws std::runtime_error wenn Matrix nicht invertierbar
     */
    Matrix inverse() const;

    /**
     * Markiert die gecachte Inverse als ungültig
     *
     * Wird aufgerufen wenn sich die Matrix ändert.
     */
    void invalidateCache();

    /**
     * Erzeugt eine Einheitsmatrix
     * @param size Dimension (Standard: 4)
     * @return n×n Einheitsmatrix
     */
    static Matrix identity(int size = 4);
};
