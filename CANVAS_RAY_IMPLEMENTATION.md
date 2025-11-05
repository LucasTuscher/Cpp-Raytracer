# Canvas und Ray Implementation - Zusammenfassung

## Implementierte Features

### 1. Color-Klasse Erweiterung
**Dateien:**
- `src/Color/Color.h`
- `src/Color/Color.cpp`

**Neue Methoden:**
- `int toIntRGB() const` - Konvertiert Farbe in INT_RGB Format (0xRRGGBB)
- `static Color fromIntRGB(int rgb)` - Erstellt Farbe aus INT_RGB Format

Diese Methoden ermöglichen die Umwandlung zwischen unserem internen Farbformat (double 0.0-1.0) und 8-bit Integer-Werten (0-255) für die Bildausgabe.

### 2. Canvas-Klasse
**Dateien:**
- `src/Canvas/Canvas.h`
- `src/Canvas/Canvas.cpp`

**Funktionalität:**
- Verwaltet ein Pixelgitter mit angegebener Breite und Höhe
- Speichert Pixel als Color-Objekte im Speicher
- Bietet Methoden zum Setzen und Lesen einzelner Pixel
- Exportiert Bilder im PPM-Format (Portable Pixmap)
- Unterstützt automatische Timestamp-Generierung für Dateinamen

**PPM-Format:**
- Verwendet wurde das PPM-Format, da es einfach zu implementieren ist
- Keine externen Bibliotheken erforderlich
- Wird von den meisten Bildbetrachtungsprogrammen unterstützt
- Kann leicht in andere Formate konvertiert werden (z.B. mit GIMP, IrfanView)

**Koordinatensystem:**
- x-Achse: von links nach rechts (0 bis width-1)
- y-Achse: von oben nach unten (0 bis height-1)
- Ursprung (0,0) ist oben links

### 3. Ray-Klasse
**Dateien:**
- `src/Ray/Ray.h`
- `src/Ray/Ray.cpp`

**Funktionalität:**
- Repräsentiert einen Strahl mit Ursprung und Richtung
- Zwei Konstruktionsmethoden:
  1. `Ray(origin, direction)` - Übernimmt Richtung wie angegeben
  2. `Ray::fromPoints(origin, target)` - Berechnet und normalisiert Richtung
- `pointAt(t)` - Berechnet Punkt auf dem Strahl für Parameter t
- Parameterform: P(t) = O + t * d

### 4. Ray Tracing Test-Programm
**Datei:** `src/Main.cpp`

**Erzeugt drei Test-Bilder:**

#### Test 1: Pixel-Koordinaten (`test-pixel-coordinates.ppm`)
- Visualisiert die Pixel-Koordinaten im Bildraum
- Rot-Kanal: normalisierte x-Koordinate (0 bis 1)
- Grün-Kanal: normalisierte y-Koordinate (0 bis 1)
- Blau-Kanal: 0

#### Test 2: Strahl-Richtungen (`test-ray-directions.ppm`)
- Visualisiert die Richtung jedes Strahls vom Betrachter zum Pixel
- Rot-Kanal: |x| der normalisierten Richtung
- Grün-Kanal: |y| der normalisierten Richtung
- Blau-Kanal: |z| der normalisierten Richtung

#### Test 3: Strahl-Längen (`test-ray-lengths.ppm`)
- Visualisiert die Distanz vom Betrachter zu jedem Pixel
- Graustufen-Bild (alle Kanäle gleich)
- Normalisiert durch maximale Strahl-Länge (zur Ecke)

## Koordinatensysteme

Das Programm arbeitet mit zwei Koordinatensystemen:

### Bild-Koordinatensystem
- 2D, Integer-Koordinaten
- Pixel-Koordinaten (x, y)
- Verwendet für Canvas-Indizierung

### Welt-Koordinatensystem
- 3D, double-Koordinaten
- Linkshändig (Unity-Konvention)
- Bildebene liegt bei z = 0
- Bildmitte liegt im Ursprung (0, 0, 0)
- Betrachter bei (0, 0, -100)

## Umrechnung zwischen Koordinatensystemen

```cpp
// Pixel-Koordinaten (x, y) → Welt-Koordinaten (px, py, pz)
double px = (x + 0.5) - width / 2.0;   // +0.5 für Pixel-Mitte
double py = (y + 0.5) - height / 2.0;
double pz = 0.0;                        // Bildebene bei z=0
```

## Build-System

**CMakeLists.txt Änderungen:**
- `src/Canvas/Canvas.cpp` hinzugefügt
- `src/Ray/Ray.cpp` hinzugefügt

## Test-Ergebnisse

### Unit Tests
✅ Alle 40 Tests bestanden:
- 1 BasicTest
- 13 PointTests
- 18 VectorTests
- 8 ColorTests

### Funktionale Tests
✅ Alle drei Test-Bilder erfolgreich generiert:
- `test-pixel-coordinates.ppm` (400x300 Pixel)
- `test-ray-directions.ppm` (400x300 Pixel)
- `test-ray-lengths.ppm` (400x300 Pixel)

## Verwendung

### Projekt builden:
```powershell
cmake -B build -S .
cmake --build build --config Release
```

### Test-Bilder generieren:
```powershell
.\build\Release\raytracer.exe
```

### Unit Tests ausführen:
```powershell
.\build\Release\raytracer_tests.exe
```

## Beantwortung der Kontrollfragen

### Frage 1
**Gegeben sei ein Strahl, der vom Punkt O ausgeht und in Richtung d zeigt. Ein Punkt auf diesem Strahl hat den Parameterwert t = -1.2. Was bedeutet das geometrisch?**

**Antwort:** Der Punkt liegt auf der Verlängerung des Strahls **entgegen** der Richtung d, in einem Abstand von 1.2 * |d| vom Ursprung O. Da t negativ ist, befindet sich der Punkt "hinter" dem Ursprung des Strahls.

### Frage 2
**Welche Auswirkungen könnte es haben, wenn die Berechnungen bei einem Ray Tracer nicht mit den Pixelmittelpunkten (x + 0.5, y + 0.5) sondern lediglich mit (x, y) durchgeführt werden?**

**Antwort:** 
- **Aliasing-Probleme:** Die Strahlen würden durch die Pixel-Ecken statt durch die Mitten gehen
- **Verschiebung:** Das gesamte Bild würde um einen halben Pixel verschoben erscheinen
- **Asymmetrie:** Bei symmetrischen Szenen würde die Symmetrie verloren gehen
- **Sampling-Fehler:** Die Farbe würde nicht den repräsentativsten Punkt des Pixels darstellen

## Nächste Schritte

Für die weitere Entwicklung des Ray Tracers:
1. ✅ Canvas-Klasse implementiert
2. ✅ Ray-Klasse implementiert
3. ✅ Grundlegender Ray-Tracing-Loop funktioniert
4. ⏭️ Kamera-Klasse implementieren
5. ⏭️ Geometrie-Klassen (Sphere, Plane, etc.)
6. ⏭️ Intersection-Tests
7. ⏭️ Beleuchtungsmodelle (Phong, etc.)
8. ⏭️ Schatten und Reflexionen

## Hinweise

- **PPM-Format:** Die Bilder können mit jedem Bildbetrachtungsprogramm geöffnet werden, das PPM unterstützt (z.B. GIMP, IrfanView, oder Windows Photos)
- **Konvertierung:** PPM-Dateien können einfach in PNG/JPEG konvertiert werden
- **Performance:** PPM ist unkomprimiert, daher sind die Dateien relativ groß
- **Sicherheit:** Der Code verwendet sichere Bounds-Checking beim Pixel-Zugriff
