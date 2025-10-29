# C++ Raytracer - Projektstruktur

## Verzeichnisübersicht

```
Cpp-Raytracer/
├── src/                          # Quellcode
│   ├── Point/
│   │   ├── Point.h              # Point-Klasse (inline implementiert)
│   │   └── Point.cpp            # (leer - alles inline)
│   ├── Vector/
│   │   ├── Vector.h             # Vector-Klasse (inline implementiert)
│   │   └── Vector.cpp           # (leer - alles inline)
│   ├── Color/
│   │   ├── Color.h              # Color-Klasse Header
│   │   └── Color.cpp            # Color-Klasse Implementierung
│   └── Main.cpp                 # Hauptprogramm (Demo)
│
├── tests/                        # Test-Suite
│   ├── TestMain.cpp             # Google Test Basis-Test
│   ├── PointTests.cpp           # 13 Tests für Point-Klasse
│   ├── VectorTests.cpp          # 18 Tests für Vector-Klasse
│   ├── ColorTests.cpp           # 8 Tests für Color-Klasse
│   └── README.md                # Test-Dokumentation
│
├── build/                        # Build-Artefakte (generiert)
│   ├── Release/
│   │   ├── raytracer.exe        # Hauptprogramm
│   │   ├── raytracer_tests.exe  # Test-Executable
│   │   └── raytracer_lib.lib    # Shared Library
│   └── ...
│
├── CMakeLists.txt               # CMake Build-Konfiguration
├── build_and_test.ps1           # PowerShell Build-Script
├── README.md                    # Projekt-README
├── IMPLEMENTATION.md            # Implementierungs-Details
├── TESTING_SUMMARY.md           # Test-Ergebnisse
├── PROJECT_STRUCTURE.md         # Diese Datei
├── LICENSE                      # MIT Lizenz
└── .gitignore                   # Git Ignore-Regeln
```

## Datei-Beschreibungen

### Quellcode (src/)

#### Point/Point.h
- **Zweck:** Repräsentation von Punkten im 3D-Raum
- **Homogene Koordinaten:** (x, y, z, 1.0)
- **Operationen:** Addition mit Vektoren, Subtraktion, Skalierung, Min/Max
- **Zeilen:** ~48 Zeilen
- **Implementierung:** Vollständig inline

#### Vector/Vector.h
- **Zweck:** Repräsentation von Richtungen/Verschiebungen
- **Homogene Koordinaten:** (x, y, z, 0.0)
- **Operationen:** Addition, Subtraktion, Negation, Skalar-/Kreuzprodukt
- **Zeilen:** ~49 Zeilen
- **Implementierung:** Vollständig inline

#### Color/Color.h & Color.cpp
- **Zweck:** RGB-Farbrepräsentation
- **Komponenten:** (r, g, b) im Bereich [0, 1]
- **Operationen:** Addition, Skalierung, Hadamard-Produkt, Clamping
- **Zeilen:** 27 (Header) + 62 (Implementation)
- **Vordefinierte Farben:** BLACK, WHITE, RED, GREEN, BLUE

#### Main.cpp
- **Zweck:** Demo-Programm
- **Funktionalität:** Zeigt Verwendung der Klassen
- **Zeilen:** ~26 Zeilen

### Tests (tests/)

#### PointTests.cpp
- **Tests:** 13 Szenarien
- **Abdeckung:**
  - Konstruktoren (3 Tests)
  - Arithmetische Operationen (5 Tests)
  - Vergleiche (2 Tests)
  - Min/Max (2 Tests)
  - Exception-Handling (1 Test)

#### VectorTests.cpp
- **Tests:** 18 Szenarien
- **Abdeckung:**
  - Konstruktoren (3 Tests)
  - Arithmetische Operationen (5 Tests)
  - Längenberechnungen (3 Tests)
  - Normalisierung (2 Tests)
  - Produkte (2 Tests)
  - Vergleiche (2 Tests)
  - Exception-Handling (1 Test)

#### ColorTests.cpp
- **Tests:** 8 Szenarien
- **Abdeckung:**
  - Konstruktoren (2 Tests)
  - Operationen (3 Tests)
  - Clamping (1 Test)
  - Vordefinierte Farben (1 Test)
  - Vergleiche (1 Test)

### Build-System

#### CMakeLists.txt
- **CMake Version:** 3.14+
- **C++ Standard:** C++17
- **Targets:**
  - `raytracer` - Hauptprogramm
  - `raytracer_lib` - Shared Library
  - `raytracer_tests` - Test-Executable
- **Dependencies:** Google Test 1.14.0 (automatisch heruntergeladen)

#### build_and_test.ps1
- **Zweck:** Automatisiertes Build & Test Script
- **Funktionen:**
  - CMake-Konfiguration
  - Build (Release-Modus)
  - Test-Ausführung mit detaillierter Ausgabe
  - Fehlerbehandlung

### Dokumentation

#### README.md
- Projekt-Übersicht
- Installation & Build-Anleitung
- Verwendungsbeispiele

#### IMPLEMENTATION.md
- Detaillierte Implementierungs-Dokumentation
- Mathematische Grundlagen
- Design-Entscheidungen
- Antworten auf Kontrollfragen

#### TESTING_SUMMARY.md
- Test-Ergebnisse
- Feature-Übersicht
- Build-Informationen
- Nächste Schritte

## Abhängigkeiten

### Build-Zeit
- **CMake:** ≥ 3.14
- **Compiler:** MSVC 19.38+ (oder GCC/Clang mit C++17)
- **Google Test:** 1.14.0 (automatisch via FetchContent)

### Laufzeit
- Keine externen Abhängigkeiten
- Nur C++ Standard Library

## Build-Artefakte

### Release-Build
```
build/Release/
├── raytracer.exe          # ~50 KB
├── raytracer_tests.exe    # ~2 MB (inkl. Google Test)
└── raytracer_lib.lib      # ~10 KB
```

### Debug-Build (optional)
```powershell
cmake --build build --config Debug
```

## Code-Statistiken

| Komponente | Header | Implementation | Tests | Gesamt |
|------------|--------|----------------|-------|--------|
| Point      | 48     | 3              | 120   | 171    |
| Vector     | 49     | 3              | 180   | 232    |
| Color      | 27     | 62             | 90    | 179    |
| Main       | -      | 26             | -     | 26     |
| **Total**  | **124**| **94**         | **390**| **608**|

## Verwendung

### Schnellstart
```powershell
# Alles in einem Schritt
.\build_and_test.ps1

# Oder manuell
cmake -B build -S .
cmake --build build --config Release
.\build\Release\raytracer_tests.exe
.\build\Release\raytracer.exe
```

### Integration in eigenen Code
```cpp
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Color/Color.h"

Point camera(0, 0, -5);
Vector direction(0, 0, 1);
Color background = Color::BLACK();
```

## Nächste Entwicklungsschritte

1. **Matrix4x4-Klasse** - Transformationen
2. **Ray-Klasse** - Strahlverfolgung
3. **Camera-Klasse** - Bildprojektion
4. **Geometrie-Primitiven** - Sphere, Plane, Triangle
5. **Intersection-Tests** - Ray-Object Schnitte
6. **Material-System** - Phong/PBR Beleuchtung
7. **Scene-Graph** - Objekthierarchie
8. **Image-Output** - PPM/PNG Export

## Lizenz

MIT License - Siehe LICENSE-Datei
