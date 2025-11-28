# Ray Tracer - Kapitel 7: Beleuchtung

Dieser Ordner enthält die Implementierung des Phong-Beleuchtungsmodells für den Ray Tracer.

## Implementierte Features

### 1. Korrigierte Normalen-Transformation
- **Dateien**: `src/Shape/Shape.cpp`, `src/Shape/Sphere.cpp`
- Die Normalen werden jetzt korrekt mit der **Transponierten der inversen Transformationsmatrix** transformiert
- Dies ist wichtig für korrekte Beleuchtung bei transformierten Objekten (skaliert, rotiert, etc.)

### 2. Vektor-Reflexion
- **Datei**: `src/Vector/Vector.h`
- Statische Methode `Vector::reflect()` für die Reflexion eines Vektors an einer Normalen
- Formel: `r = e - 2(e · n)n`
- Wird für die spekulare Komponente des Phong-Modells benötigt

### 3. Lichtquellen-System
- **Dateien**: `src/LightSource/LightSource.h`, `src/LightSource/PointLightSource.h`
- Basisklasse `LightSource` für alle Lichtquellen
- `PointLightSource`: Punktlichtquelle mit Position, Farbe und Intensität
- Szenen können mehrere Lichtquellen enthalten

### 4. Material-System
- **Dateien**: `src/Material/Material.h`, `src/Material/Material.cpp`
- Oberflächeneigenschaften:
  - `color`: Farbe der Oberfläche
  - `ambient`: Ambienter Reflexionskoeffizient (Standard: 0.1)
  - `diffuse`: Diffuser Reflexionskoeffizient (Standard: 0.9)
  - `specular`: Spekularer Reflexionskoeffizient (Standard: 0.9)
  - `shininess`: Glanz (Standard: 200.0)
- Methode `phongLighting()`: Berechnet Beleuchtung nach Phong

### 5. Phong-Beleuchtungsmodell
Das Phong-Modell besteht aus drei Komponenten:

**Formel**: `I = ka*O + IL*O*kd*(n·l) + IL*ks*(v·r)^shininess`

1. **Ambient**: Grundhelligkeit (unabhängig von Lichtquellen)
   - Simuliert indirektes Licht in der Szene

2. **Diffuse**: Matte Reflexion (abhängig vom Einfallswinkel)
   - Je steiler das Licht einfällt, desto heller
   - Lambertsche Reflexion: `kd * (n · l)`

3. **Specular**: Glänzende Reflexion (Highlights)
   - Abhängig vom Winkel zwischen Betrachter und reflektiertem Licht
   - `ks * (v · r)^shininess`

### 6. HitInfo-Struktur
- **Datei**: `src/HitInfo/HitInfo.h`
- Speichert alle Informationen über einen Schnittpunkt:
  - t-Wert, getroffenes Objekt, Schnittpunkt
  - Eye-Vektor (Richtung zur Kamera)
  - Normalenvektor

### 7. Integration in Scene und RayTracer
- **Scene::colorAt()**: Kombiniert Strahlverfolgung und Beleuchtung
- **Scene::shadeHit()**: Berechnet Farbe mit Beleuchtung
- **RayTracer**: Nutzt automatisch das Beleuchtungssystem

## Generierte Test-Bilder

### lighting1-simple-sphere.ppm
**Einfache beleuchtete Kugel**
- Einzelne Kugel im Ursprung
- Pink-violettes Material
- Lichtquelle bei (-10, 10, -10)
- Zeigt das vollständige Phong-Modell

### lighting2a-ambient-only.ppm
**Nur ambiente Komponente**
- Zeigt nur die Grundhelligkeit
- Keine diffuse oder spekulare Reflexion
- Gleichmäßige Farbe über die gesamte Kugel

### lighting2b-diffuse-only.ppm
**Nur diffuse Komponente**
- Zeigt nur die matte Reflexion
- Heller zur Lichtquelle hin, dunkler auf der Rückseite
- Keine Highlights

### lighting2c-specular-only.ppm
**Nur spekulare Komponente**
- Zeigt nur die glänzende Reflexion
- Helles Highlight wo das reflektierte Licht zur Kamera zeigt
- Rest der Kugel ist schwarz

### lighting2d-all-combined.ppm
**Alle Komponenten zusammen**
- Ambient + Diffuse + Specular
- Realistischste Darstellung
- Kombination aller drei Effekte

### lighting3-three-spheres.ppm
**Drei Kugeln mit verschiedenen Materialien**
- **Links (Rot)**: Sehr glänzend (shininess=300)
- **Mitte (Grün)**: Weniger glänzend (shininess=50, specular=0.3)
- **Rechts (Blau)**: Matt (shininess=10, specular=0.1)

### lighting4-diffuse-variation.ppm
**Variation des diffusen Reflexionskoeffizienten**
- 4 Kugeln mit diffuse-Werten von 0.3 bis 0.9
- Von links nach rechts: 0.3, 0.5, 0.7, 0.9
- Zeigt den Einfluss der matten Reflexion

### lighting5-specular-variation.ppm
**Variation des spekularen Reflexionskoeffizienten**
- 4 Kugeln mit specular-Werten von 0.3 bis 0.9
- Von links nach rechts: 0.3, 0.5, 0.7, 0.9
- Zeigt den Einfluss der Highlight-Stärke

### lighting6-shininess-variation.ppm
**Variation der Shininess**
- 7 Kugeln mit shininess-Werten von 10 bis 190
- Von links nach rechts: 10, 40, 70, 100, 130, 160, 190
- Niedrige Werte = große, weiche Highlights
- Hohe Werte = kleine, scharfe Highlights

### lighting7-default-scene.ppm
**Standard-Szene mit Beleuchtung**
- Die defaultScene() aus dem Skript
- Zwei Kugeln mit unterschiedlichen Materialien
- Zeigt das Zusammenspiel von Transformation und Beleuchtung

## Kompilierung und Ausführung

### Kompilieren
```bash
cd /Users/lucastuscher/Desktop/Cpp-Raytracer
rm -rf build && mkdir build && cd build
cmake ..
make raytracer_lighting
```

### Ausführen
```bash
./raytracer_lighting
```

Die PPM-Dateien werden im Hauptverzeichnis erstellt.

### PPM-Dateien anzeigen
PPM-Dateien können mit verschiedenen Programmen geöffnet werden:
- **macOS**: Preview, GIMP
- **Windows**: IrfanView, GIMP, Paint.NET
- **Linux**: GIMP, ImageMagick (display), eog

Online-Konverter: https://convertio.co/ppm-png/

## Tests

Alle Tests laufen erfolgreich:
```bash
./raytracer_tests
```

126 Tests bestanden ✓

## Nächste Schritte

Mögliche Erweiterungen:
1. **Schatten**: Objekte werfen Schatten auf andere Objekte
2. **Reflexionen**: Spiegelung anderer Objekte
3. **Refraktion**: Lichtbrechung in transparenten Objekten
4. **Texturen**: Oberflächenmuster
5. **Weitere Lichtquellen**: Gerichtete Lichter, Spotlights, Flächenlichter
6. **Soft Shadows**: Weiche Schatten mit Area Lights
7. **Global Illumination**: Realistische indirekte Beleuchtung

## Referenzen

- **Universitätsskript**: Kapitel 7 - Beleuchtung
- **Phong Reflection Model**: Bui Tuong Phong (1975)
- **The Ray Tracer Challenge**: Jamis Buck
- **Scratchapixel**: https://www.scratchapixel.com/
