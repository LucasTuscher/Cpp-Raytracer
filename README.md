# C++ Raytracer

Ein C++ Raytracer basierend auf dem Universitätsskript "Mathematische Datenstrukturen – Punkte, Vektoren, Farben".

## Übersicht

Dieses Projekt implementiert die mathematischen Grundlagen für einen Raytracer:
- **Point-Klasse**: Repräsentation von Orten im 3D-Raum mit homogenen Koordinaten
- **Vector-Klasse**: Richtungen und Verschiebungen mit Skalar- und Kreuzprodukt
- **Color-Klasse**: RGB-Farbmodell mit Hadamard-Produkt und Clamping

## Features

### Implementierte Datenstrukturen

**Point (Punkt)**
- Homogene Koordinaten (x, y, z, 1.0)
- Operationen: Addition mit Vektoren, Subtraktion, Skalierung
- Min/Max-Funktionen für Bounding Boxes
- Epsilon-basierte Vergleiche

**Vector (Vektor)**
- Homogene Koordinaten (x, y, z, 0.0)
- Operationen: Addition, Subtraktion, Negation, Skalierung
- Längenberechnung (magnitude, sqrMagnitude)
- Normalisierung
- Skalarprodukt (dot product)
- Kreuzprodukt (cross product)

**Color (Farbe)**
- RGB-Komponenten (r, g, b)
- Farbaddition und Skalierung
- Hadamard-Produkt (komponentenweise Multiplikation)
- Clamping auf [0, 1]
- Vordefinierte Farben (BLACK, WHITE, RED, GREEN, BLUE)

### Test-Abdeckung

- 39 Unit Tests mit Google Test
- 100% Abdeckung aller Operationen
- Exception-Handling Tests
- Epsilon-Vergleiche für Gleitkomma-Genauigkeit

## Schnellstart

### Voraussetzungen

**Windows:**
- CMake 3.14 oder höher
- Visual Studio 2019/2022 (MSVC-Compiler) oder MinGW
- PowerShell

**macOS:**
- CMake 3.14 oder höher
- Xcode Command Line Tools: `xcode-select --install`
- C++17-kompatibler Compiler (Clang)

### Build & Test (Windows)

#### Automatischer Build & Test

```powershell
# Alles in einem Schritt
.\build_and_test.ps1
```

#### Manueller Build

```powershell
# CMake konfigurieren
cmake -B build -S .

# Projekt kompilieren (Release)
cmake --build build --config Release

# Oder Debug-Build
cmake --build build --config Debug
```

#### Programm ausführen

```powershell
# Tests ausführen
.\build\Release\raytracer_tests.exe

# Oder mit ctest
cd build
ctest -C Release --output-on-failure

# Hauptprogramm ausführen
.\build\Release\raytracer.exe
```

### Build & Test (macOS)

#### Build-Prozess

```bash
# CMake konfigurieren
cmake -B build -S .

# Projekt kompilieren (Release)
cmake --build build --config Release

# Oder Debug-Build
cmake --build build --config Debug
```

#### Programm ausführen

```bash
# Tests ausführen
./build/raytracer_tests

# Oder mit ctest
cd build
ctest --output-on-failure

# Hauptprogramm ausführen
./build/raytracer
```

**Hinweis für macOS:** Auf macOS werden die Executables direkt im `build/`-Ordner erstellt (nicht in `build/Release/`), da Xcode/Make eine andere Ordnerstruktur als MSVC verwendet.

**Beispielausgabe:**
```
diff: (-2, 0, 2)
moved: (2, 0, 6)
cross: (-4, -8, -4)
bright: (0.75, 1, 1)
```

## Code-Beispiele

### Punkte und Vektoren

```cpp
#include "Point/Point.h"
#include "Vector/Vector.h"

// Punkte erstellen
Point camera(0, 0, -5);
Point target(0, 0, 0);

// Richtungsvektor berechnen
Vector direction = target - camera;  // Punkt - Punkt = Vektor
Vector normalized = direction.normalized();

// Punkt verschieben
Point newPos = camera + direction * 0.5;

// Skalarprodukt
double dot = Vector::dot(direction, Vector(0, 1, 0));

// Kreuzprodukt
Vector up(0, 1, 0);
Vector right = Vector::cross(direction, up);
```

### Farben

```cpp
#include "Color/Color.h"

// Farben erstellen
Color red = Color::RED();
Color custom(0.5, 0.3, 0.8);

// Farboperationen
Color mixed = red * custom;           // Hadamard-Produkt
Color bright = (custom * 2.5).clamped();  // Skalierung + Clamping
Color sum = red + custom;
```

## Projekt-Struktur

```
Cpp-Raytracer/
├── src/                          # Quellcode
│   ├── Point/
│   │   ├── Point.h              # Point-Klasse
│   │   └── Point.cpp
│   ├── Vector/
│   │   ├── Vector.h             # Vector-Klasse
│   │   └── Vector.cpp
│   ├── Color/
│   │   ├── Color.h              # Color-Klasse
│   │   └── Color.cpp
│   └── Main.cpp                 # Demo-Programm
│
├── tests/                        # Test-Suite
│   ├── PointTests.cpp           # 13 Point-Tests
│   ├── VectorTests.cpp          # 18 Vector-Tests
│   ├── ColorTests.cpp           # 8 Color-Tests
│   └── README.md                # Test-Dokumentation
│
├── build/                        # Build-Artefakte (generiert)
│   └── Release/
│       ├── raytracer.exe        # Hauptprogramm
│       └── raytracer_tests.exe  # Tests
│
├── CMakeLists.txt               # CMake-Konfiguration
├── build_and_test.ps1           # Build-Script
├── README.md                    # Diese Datei
├── IMPLEMENTATION.md            # Implementierungs-Details
├── TESTING_SUMMARY.md           # Test-Ergebnisse
├── PROJECT_STRUCTURE.md         # Detaillierte Struktur
└── LICENSE                      # MIT Lizenz
```

## Nach Änderungen neu kompilieren

**Windows:**
```powershell
# Nur geänderte Dateien neu kompilieren
cmake --build build --config Release
```

**macOS:**
```bash
# Nur geänderte Dateien neu kompilieren
cmake --build build --config Release
```

CMake erkennt automatisch, welche Dateien geändert wurden.

## Projekt komplett neu bauen

**Windows:**
```powershell
# Build-Ordner löschen
Remove-Item -Recurse -Force build

# Neu konfigurieren und bauen
cmake -B build -S .
cmake --build build --config Release
```

**macOS:**
```bash
# Build-Ordner löschen
rm -rf build

# Neu konfigurieren und bauen
cmake -B build -S .
cmake --build build --config Release
```

## Koordinatensystem

Das Projekt verwendet ein **linkshändiges Koordinatensystem**:
- Positive x-Achse: nach rechts
- Positive y-Achse: nach oben
- Positive z-Achse: vom Betrachter weg

Dies entspricht der Konvention von Unity und RenderMan.

## Dokumentation

- **IMPLEMENTATION.md**: Detaillierte Implementierungs-Dokumentation mit mathematischen Grundlagen
- **TESTING_SUMMARY.md**: Vollständige Test-Ergebnisse und Feature-Übersicht
- **PROJECT_STRUCTURE.md**: Detaillierte Projektstruktur und Code-Statistiken
- **tests/README.md**: Test-Spezifikationen und Ausführungsanleitung

## Technische Details

- **C++ Standard**: C++17
- **Build-System**: CMake 3.14+
- **Test-Framework**: Google Test 1.14.0
- **Compiler**: MSVC 19.38+ / GCC / Clang
- **Gleitkomma-Genauigkeit**: double (1e-6 Epsilon)

## Lizenz

MIT License - Siehe LICENSE-Datei
