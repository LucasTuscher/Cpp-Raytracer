# Raytracer Tests

Diese Tests basieren auf den Anforderungen aus dem Skript "Mathematische Datenstrukturen – Punkte, Vektoren, Farben".

## Test-Struktur

### PointTests.cpp
Tests für die `Point`-Klasse:
- Konstruktoren und Initialisierung
- Arithmetische Operationen (Addition mit Vektoren, Subtraktion)
- Skalarmultiplikation und -division
- Vergleichsoperatoren
- Min/Max-Funktionen
- Exception-Handling bei Division durch Null

### VectorTests.cpp
Tests für die `Vector`-Klasse:
- Konstruktoren und Initialisierung
- Vektoroperationen (Addition, Subtraktion, Negation)
- Skalarmultiplikation und -division
- Längenberechnungen (magnitude, sqrMagnitude)
- Normalisierung
- Skalarprodukt (dot product)
- Kreuzprodukt (cross product)
- Vergleichsoperatoren
- Exception-Handling bei Division durch Null

### ColorTests.cpp
Tests für die `Color`-Klasse:
- Konstruktoren und Initialisierung
- Farbaddition
- Skalarmultiplikation
- Hadamard-Produkt (komponentenweise Multiplikation)
- Clamping (Beschränkung auf [0,1])
- Vordefinierte Farben (BLACK, WHITE, RED, GREEN, BLUE)
- Vergleichsoperatoren

## Tests ausführen

### Windows (PowerShell)
```powershell
# Build
cmake -B build -S .
cmake --build build --config Release

# Tests ausführen
cd build
ctest -C Release --output-on-failure
```

### Alternative: Direkt das Test-Executable ausführen
```powershell
.\build\Release\raytracer_tests.exe
```

## Wichtige Hinweise

- Alle Gleitkomma-Vergleiche verwenden Epsilon-Toleranz (1e-6)
- Die w-Komponente ist 1.0 für Punkte und 0.0 für Vektoren (homogene Koordinaten)
- Division durch Null wirft eine `std::runtime_error` Exception
- Farben können Werte außerhalb [0,1] haben, müssen aber mit `clamped()` normalisiert werden
