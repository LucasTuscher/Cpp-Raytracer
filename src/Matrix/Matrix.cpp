#include "Matrix.h"
#include <stdexcept>
#include <cmath>
#include <sstream>

// ========================
// Konstruktoren & Destruktor
// ========================

Matrix::Matrix(int size)
    : size_(size), inverseCalculated_(false), cachedInverse_(nullptr) {
    if (size <= 0) {
        throw std::runtime_error("Matrix-Größe muss positiv sein");
    }
    data_.resize(size, std::vector<double>(size, 0.0));
}

Matrix::Matrix(int size, const std::vector<double>& values)
    : size_(size), inverseCalculated_(false), cachedInverse_(nullptr) {
    if (size <= 0) {
        throw std::runtime_error("Matrix-Größe muss positiv sein");
    }
    if (values.size() != static_cast<size_t>(size * size)) {
        std::ostringstream oss;
        oss << "Falsche Anzahl Werte: erwartet " << (size * size)
            << ", erhalten " << values.size();
        throw std::runtime_error(oss.str());
    }

    data_.resize(size, std::vector<double>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            data_[i][j] = values[i * size + j];
        }
    }
}

Matrix::Matrix(const Matrix& other)
    : size_(other.size_), data_(other.data_),
      inverseCalculated_(false), cachedInverse_(nullptr) {
}

Matrix::~Matrix() {
    if (cachedInverse_ != nullptr) {
        delete cachedInverse_;
    }
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        size_ = other.size_;
        data_ = other.data_;
        invalidateCache();
    }
    return *this;
}

// ========================
// Elementzugriff
// ========================

double Matrix::at(int row, int col) const {
    if (row < 0 || row >= size_ || col < 0 || col >= size_) {
        throw std::out_of_range("Matrix-Index außerhalb des Bereichs");
    }
    return data_[row][col];
}

void Matrix::set(int row, int col, double value) {
    if (row < 0 || row >= size_ || col < 0 || col >= size_) {
        throw std::out_of_range("Matrix-Index außerhalb des Bereichs");
    }
    data_[row][col] = value;
    invalidateCache();
}

void Matrix::invalidateCache() {
    if (cachedInverse_ != nullptr) {
        delete cachedInverse_;
        cachedInverse_ = nullptr;
    }
    inverseCalculated_ = false;
}

// ========================
// Vergleichsoperatoren
// ========================

bool Matrix::operator==(const Matrix& m) const {
    if (size_ != m.size_) {
        return false;
    }

    const double eps = 1e-5;
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
            if (std::fabs(data_[i][j] - m.data_[i][j]) > eps) {
                return false;
            }
        }
    }
    return true;
}

// ========================
// Matrixmultiplikation
// ========================

Matrix Matrix::operator*(const Matrix& m) const {
    if (size_ != m.size_) {
        throw std::runtime_error("Matrizen müssen gleiche Größe haben für Multiplikation");
    }

    Matrix result(size_);
    for (int i = 0; i < size_; i++) {
        for (int k = 0; k < size_; k++) {
            double sum = 0.0;
            for (int j = 0; j < size_; j++) {
                sum += data_[i][j] * m.data_[j][k];
            }
            result.data_[i][k] = sum;
        }
    }
    return result;
}

Vector Matrix::operator*(const Vector& v) const {
    if (size_ != 4) {
        throw std::runtime_error("Matrix muss 4×4 sein für Vektor-Multiplikation");
    }

    // Vektor hat w = 0
    double x = data_[0][0] * v.x + data_[0][1] * v.y + data_[0][2] * v.z + data_[0][3] * 0.0;
    double y = data_[1][0] * v.x + data_[1][1] * v.y + data_[1][2] * v.z + data_[1][3] * 0.0;
    double z = data_[2][0] * v.x + data_[2][1] * v.y + data_[2][2] * v.z + data_[2][3] * 0.0;
    // w-Komponente ignorieren für Vektor

    return Vector(x, y, z);
}

Point Matrix::operator*(const Point& p) const {
    if (size_ != 4) {
        throw std::runtime_error("Matrix muss 4×4 sein für Punkt-Multiplikation");
    }

    // Punkt hat w = 1
    double x = data_[0][0] * p.x + data_[0][1] * p.y + data_[0][2] * p.z + data_[0][3] * 1.0;
    double y = data_[1][0] * p.x + data_[1][1] * p.y + data_[1][2] * p.z + data_[1][3] * 1.0;
    double z = data_[2][0] * p.x + data_[2][1] * p.y + data_[2][2] * p.z + data_[2][3] * 1.0;
    double w = data_[3][0] * p.x + data_[3][1] * p.y + data_[3][2] * p.z + data_[3][3] * 1.0;

    // Normalisierung bei w != 1 (für perspektivische Projektionen)
    // In unserem Fall sollte w immer 1 bleiben, aber sicherheitshalber:
    if (std::fabs(w - 1.0) > 1e-6) {
        return Point(x / w, y / w, z / w);
    }

    return Point(x, y, z);
}

// ========================
// Transponierung
// ========================

Matrix Matrix::transpose() const {
    Matrix result(size_);
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
            result.data_[j][i] = data_[i][j];
        }
    }
    return result;
}

// ========================
// Determinanten-Berechnung
// ========================

Matrix Matrix::submatrix(int removeRow, int removeCol) const {
    if (size_ <= 1) {
        throw std::runtime_error("Untermatrix nicht definiert für 1×1 Matrix");
    }

    Matrix result(size_ - 1);
    int targetRow = 0;
    for (int i = 0; i < size_; i++) {
        if (i == removeRow) continue;

        int targetCol = 0;
        for (int j = 0; j < size_; j++) {
            if (j == removeCol) continue;

            result.data_[targetRow][targetCol] = data_[i][j];
            targetCol++;
        }
        targetRow++;
    }
    return result;
}

double Matrix::minor(int row, int col) const {
    Matrix sub = submatrix(row, col);
    return sub.determinant();
}

double Matrix::cofactor(int row, int col) const {
    double m = minor(row, col);
    // (-1)^(row+col)
    if ((row + col) % 2 == 1) {
        return -m;
    }
    return m;
}

double Matrix::determinant() const {
    // Basisfall: 2×2 Matrix
    if (size_ == 2) {
        return data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];
    }

    // Rekursion: Entwicklung nach erster Zeile
    double det = 0.0;
    for (int j = 0; j < size_; j++) {
        det += data_[0][j] * cofactor(0, j);
    }
    return det;
}

// ========================
// Invertierung
// ========================

bool Matrix::isInvertible() const {
    return std::fabs(determinant()) > 1e-12;
}

Matrix Matrix::inverse() const {
    // Lazy Evaluation: Cache verwenden
    if (inverseCalculated_) {
        return *cachedInverse_;
    }

    if (!isInvertible()) {
        throw std::runtime_error("Matrix ist nicht invertierbar (Determinante = 0)");
    }

    double det = determinant();
    Matrix result(size_);

    // Berechne Adjunkte (transponierte Kofaktorenmatrix)
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
            // Achtung: Transponierung durch Vertauschen der Indizes
            result.data_[j][i] = cofactor(i, j) / det;
        }
    }

    // Cache speichern (const_cast für Lazy Evaluation)
    Matrix* thisNonConst = const_cast<Matrix*>(this);
    if (thisNonConst->cachedInverse_ != nullptr) {
        delete thisNonConst->cachedInverse_;
    }
    thisNonConst->cachedInverse_ = new Matrix(result);
    thisNonConst->inverseCalculated_ = true;

    return result;
}

// ========================
// Statische Methoden
// ========================

Matrix Matrix::identity(int size) {
    Matrix result(size);
    for (int i = 0; i < size; i++) {
        result.data_[i][i] = 1.0;
    }
    return result;
}
