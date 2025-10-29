# Implementierung: Mathematische Datenstrukturen

## Übersicht

Diese Implementierung basiert auf Kapitel 1 des Raytracing-Skripts: "Mathematische Datenstrukturen – Punkte, Vektoren, Farben".

## Koordinatensystem

Wir verwenden ein **linkshändiges Koordinatensystem**:
- Positive x-Achse: nach rechts
- Positive y-Achse: nach oben  
- Positive z-Achse: vom Betrachter weg

Dies entspricht der Konvention von Unity und RenderMan.

## Homogene Koordinaten

Punkte und Vektoren werden mit homogenen Koordinaten (4D) repräsentiert:

- **Punkt**: `(x, y, z, 1)` - w-Komponente = 1.0
- **Vektor**: `(x, y, z, 0)` - w-Komponente = 0.0

Dies ermöglicht einheitliche Transformationen (Translation, Rotation, Skalierung) durch Matrixoperationen.

## Klassen-Implementierung

### Point (src/Point/Point.h)

**Eigenschaften:**
- `double x, y, z, w` (w ist immer 1.0)

**Operationen:**
- `Point - Point → Vector` (Differenz zweier Punkte ergibt Richtungsvektor)
- `Point ± Vector → Point` (Verschiebung eines Punktes)
- `Point * double → Point` (Skalierung)
- `Point / double → Point` (Division, wirft Exception bei 0)
- `Point::min(Point, Point) → Point` (komponentenweises Minimum)
- `Point::max(Point, Point) → Point` (komponentenweises Maximum)

**Vergleiche:**
- Epsilon-basiert (1e-6) wegen Gleitkomma-Ungenauigkeit

### Vector (src/Vector/Vector.h, Vector.cpp)

**Eigenschaften:**
- `double x, y, z, w` (w ist immer 0.0)

**Operationen:**
- `Vector ± Vector → Vector` (Addition/Subtraktion)
- `-Vector → Vector` (Negation)
- `Vector * double → Vector` (Skalierung)
- `Vector / double → Vector` (Division, wirft Exception bei 0)
- `magnitude()` - Länge des Vektors: √(x² + y² + z²)
- `sqrMagnitude()` - Quadrierte Länge (effizienter für Vergleiche)
- `normalized()` - Normalisierter Einheitsvektor
- `dot(Vector, Vector)` - Skalarprodukt
- `cross(Vector, Vector)` - Kreuzprodukt (nur 3D)

**Wichtige Funktionen:**

#### Skalarprodukt (Dot Product)
```cpp
dot(a, b) = a.x*b.x + a.y*b.y + a.z*b.z
```
Geometrische Interpretation: `dot(a,b) = |a| * |b| * cos(θ)`

#### Kreuzprodukt (Cross Product)
```cpp
cross(a, b) = (a.y*b.z - a.z*b.y,
               a.z*b.x - a.x*b.z,
               a.x*b.y - a.y*b.x)
```
Ergebnis: Vektor senkrecht zu a und b (Rechte-Hand-Regel im linkshändigen System)

### Color (src/Color/Color.h, Color.cpp)

**Eigenschaften:**
- `double r, g, b` (RGB-Komponenten, normalerweise [0,1])

**Operationen:**
- `Color + Color → Color` (Addition)
- `Color * double → Color` (Skalarmultiplikation)
- `Color * Color → Color` (Hadamard-Produkt, komponentenweise)
- `clamped()` - Beschränkt Werte auf [0,1]

**Vordefinierte Farben:**
- `Color::BLACK()` - (0, 0, 0)
- `Color::WHITE()` - (1, 1, 1)
- `Color::RED()` - (1, 0, 0)
- `Color::GREEN()` - (0, 1, 0)
- `Color::BLUE()` - (0, 0, 1)

## Design-Entscheidungen

### 1. Separate Klassen für Point und Vector
Obwohl beide 4D-Koordinaten verwenden, sind sie semantisch unterschiedlich:
- **Punkt**: Ort im Raum
- **Vektor**: Richtung/Verschiebung

Nicht alle Operationen sind für beide sinnvoll (z.B. Punkt + Punkt ist mathematisch nicht definiert).

### 2. Inline-Implementierung
Die meisten Operationen sind in Header-Dateien inline implementiert für bessere Performance, da sie sehr häufig aufgerufen werden.

### 3. Epsilon-Vergleiche
Gleitkommazahlen werden mit Toleranz verglichen:
```cpp
a == b ⟺ |a - b| ≤ ε (ε = 1e-6)
```

### 4. Exception-Handling
Division durch Null wirft `std::runtime_error` statt undefiniertes Verhalten.

### 5. Immutability
Alle Operationen geben neue Instanzen zurück, ändern nicht das Original:
```cpp
Point a(1, 2, 3);
Point b = a * 2;  // a bleibt unverändert
```

## Performance-Überlegungen

### Warum sqrMagnitude()?
```cpp
// Langsam (benötigt sqrt)
if (v.magnitude() < threshold) { ... }

// Schnell (nur Multiplikationen)
if (v.sqrMagnitude() < threshold*threshold) { ... }
```

Quadratwurzel ist teuer - für Vergleiche genügt oft die quadrierte Länge.

## Kontrollfragen (Antworten)

1. **Unterschied Punkt/Vektor**: Punkt = Ort im Raum, Vektor = Richtung/Verschiebung
2. **Homogene Koordinaten**: Translation ist keine lineare Transformation in 3D, aber in 4D
3. **Punkt + Punkt**: Mathematisch nicht definiert (was wäre die Bedeutung?)
4. **Ortsvektor**: Vektor vom Ursprung zum Punkt
5. **Skalarprodukt**: Projektion eines Vektors auf den anderen, `|a|*|b|*cos(θ)`
6. **Kreuzprodukt**: Vektor senkrecht zu beiden Eingangsvektoren
7. **sqrMagnitude**: Vermeidet teure sqrt-Operation bei Vergleichen
8. **Clamping-Problem**: Kann Farbverhältnisse verfälschen (Tone Mapping ist besser)
9. **RGB ↔ CMY**: Komplementär, CMY = (1-R, 1-G, 1-B)

## Nächste Schritte

- Matrix-Klasse für Transformationen
- Ray-Klasse für Strahlverfolgung
- Intersection-Tests
- Kamera-Implementierung
- Bildausgabe (PPM/PNG)
