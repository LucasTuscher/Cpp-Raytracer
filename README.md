# C++ Raytracer

> Ein vollständiger Raytracer in C++17 — von den mathematischen Grundlagen bis hin zu Reflexionen, Schatten und Textur-Mapping.

---

## Highlights

| Feature | Details |
|---|---|
| **Sprache** | C++17 |
| **Build-System** | CMake 3.14+ |
| **Test-Frameworks** | Google Test 1.14.0 + AZTest-Cpp |
| **Ausgabeformat** | PPM (Portable Pixmap) |
| **Koordinatensystem** | Linkshändig (Unity / RenderMan-Konvention) |

---

## Features

### Geometrische Primitive

| Shape | Beschreibung |
|---|---|
| **Sphere** | Einheitskugel, beliebig transformierbar |
| **Plane** | Unendliche Ebene (z. B. als Boden) |
| **Cube** | Achsenparalleler Würfel (AABB-Schnitt) |
| **Cylinder** | Offener oder geschlossener Zylinder |
| **Cone** | Offener oder geschlossener Kegel |

### Beleuchtung & Shading

- **Phong-Beleuchtungsmodell** — Ambient, Diffuse, Specular
- **Direktionale Lichtquellen** — parallele Lichtstrahlen (Sonne)
- **Punkt-Lichtquellen** — mit Abstandsdämpfung (Attenuation)
- **Spot-Lichtquellen** — kegelförmiger Lichtkegel
- **Schatten (Shadows)** — Shadow Rays für harte Schatten
- **Reflexionen** — rekursive Ray-Verfolgung mit konfigurierbarer Tiefe

### Materialien & Texturen

- **Material-System** — Farbe, Ambient, Diffuse, Specular, Shininess, Reflexivität
- **Image-Texturen** — PPM-Bilder auf Kugeln mappen (UV-Mapping)
- **Hadamard-Produkt** — komponentenweise Farbmultiplikation

### Kamera & Szene

- **Freie Kamera** — Position, Look-At-Punkt, Field of View (FOV)
- **Scene-Graph** — Objekte und Lichtquellen per Name zugreifbar
- **4×4 Transformationsmatrizen** — Scale, Rotate, Translate für alle Objekte

### Test-Abdeckung

- **Google Test Suite** — 14 Testdateien (Point, Vector, Color, Matrix, Ray, Shape, Sphere, Plane, Cube, Cylinder, Cone, Intersection, Scene, Camera)
- **AZTest Suite** — 22 Testdateien inkl. Lighting, Shadows, Reflections, Textures, Advanced Lighting
- Epsilon-basierte Gleitkomma-Vergleiche (`1e-6`)

---

## Architektur

```
┌─────────────────────────────────────────────────────────────┐
│                        RayTracer                            │
│   Für jedes Pixel → Ray erzeugen → Szene traceRay()         │
│                   → shadeHit() → Farbe schreiben            │
└────────────────────────┬────────────────────────────────────┘
                         │
          ┌──────────────┼──────────────┐
          │              │              │
       Camera          Scene          Canvas
       (FOV,         (Shapes,        (PPM-
       ViewTF)       Lights)         Ausgabe)
                         │
         ┌───────────────┼───────────────┐
         │               │               │
      Shapes          Lights          Material
   ┌────┴────┐    ┌────┴────┐
   Sphere   Plane  Point  Directional
   Cube   Cylinder  Spot
   Cone
         │
       Texture
    (ImageTexture
      + UV-Mapping)
```

---

## Render-Galerie

Alle erzeugten Bilder liegen als `.ppm` im `archiv/`-Ordner:

| Kategorie | Pfad |
|---|---|
| Kamera & Scene | `archiv/scene-camera/` |
| Beleuchtung | `archiv/lighting/` |
| Lichtquellen | `archiv/lightingsource/` |
| Schatten | `archiv/shadows/` |
| Reflexionen | `archiv/reflection/` |
| Transformationen | `archiv/transformations/` |
| Alle weiteren | `archiv/weitere-images/` |

`.ppm`-Dateien lassen sich z. B. mit [IrfanView](https://www.irfanview.com/), GIMP oder dem macOS Preview öffnen.

---

## Schnellstart

### Voraussetzungen

**Windows**
- CMake 3.14+
- Visual Studio 2019/2022 (MSVC) oder MinGW
- PowerShell

**macOS**
- CMake 3.14+
- Xcode Command Line Tools: `xcode-select --install`

### Build

```bash
# CMake konfigurieren
cmake -B build -S .

# Kompilieren (Release)
cmake --build build --config Release
```

### Demo-Programme ausführen

Das Projekt enthält mehrere eigenständige Demo-Executables:

| Executable | Beschreibung |
|---|---|
| `raytracer` | Haupt-Demo |
| `raytracer_lighting` | Phong-Beleuchtung |
| `raytracer_lightsources` | Punkt-, Direktional- & Spot-Licht |
| `raytracer_shadows` | Schattenberechnung |
| `raytracer_reflection` | Rekursive Reflexionen |
| `raytracer_textures` | Textur-Mapping auf Kugel |
| `raytracer_primitives` | Alle Primitive (Cube, Cylinder, Cone, Plane) |
| `transformation_demo` | Transformationsmatrix-Demo |

**Windows:**
```powershell
.\build\Release\raytracer.exe
.\build\Release\raytracer_lighting.exe
.\build\Release\raytracer_shadows.exe
.\build\Release\raytracer_reflection.exe
# ... usw.
```

**macOS:**
```bash
./build/raytracer
./build/raytracer_lighting
./build/raytracer_shadows
./build/raytracer_reflection
```

### Tests ausführen

#### Google Test (optional, erfordert Netzwerk beim ersten Build)

```bash
cmake -B build -S . -DENABLE_TESTS=ON
cmake --build build --config Release
cmake -E chdir build ctest -C Release --output-on-failure
```

#### AZTest (immer verfügbar, kein Download nötig)

```bash
cmake -B build -S .
cmake --build build --config Release
cmake -E chdir build ctest --output-on-failure
```

---

## Code-Beispiele

### Szene aufbauen & rendern

```cpp
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Shape/Sphere.h"
#include "LightSource/PointLightSource.h"
#include "Canvas/Canvas.h"

// Kamera konfigurieren
Camera camera(400, 300, M_PI / 3.0);
camera.setTransform(Matrix::viewTransform(
    Point(0, 1.5, -5),   // Position
    Point(0, 1, 0),       // Look-At
    Vector(0, 1, 0)       // Up-Vektor
));

// Szene aufbauen
Scene scene;

Sphere floor;
floor.setTransform(Matrix::scaling(10, 0.01, 10));
floor.material().color = Color(1, 0.9, 0.9);
scene.addObject(&floor);

Sphere sphere;
sphere.setTransform(Matrix::translation(0, 1, 0));
sphere.material().reflectivity = 0.5;
scene.addObject(&sphere);

PointLightSource light(Point(-10, 10, -10), Color::WHITE());
scene.addLight(&light);

// Rendern
RayTracer rt(&scene, &camera);
rt.render();
rt.getRenderTarget().writePPM("output.ppm");
```

### Punkte, Vektoren & Transformationen

```cpp
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Matrix/Matrix.h"

Point origin(0, 0, 0);
Vector dir(1, 0, 0);

// Kreuz- und Skalarprodukt
Vector up(0, 1, 0);
Vector right = Vector::cross(dir, up);
double d = Vector::dot(dir, up);   // 0.0

// Transformation
Matrix tf = Matrix::rotationY(M_PI / 4.0) * Matrix::scaling(2, 2, 2);
Point transformed = tf * origin;
```

### Farben & Beleuchtung

```cpp
#include "Color/Color.h"
#include "Material/Material.h"

Material mat;
mat.color       = Color(0.8, 0.2, 0.1);
mat.ambient     = 0.1;
mat.diffuse     = 0.9;
mat.specular    = 0.9;
mat.shininess   = 200.0;
mat.reflectivity = 0.3;

// Hadamard-Produkt + Clamping
Color a(0.5, 1.5, 0.2);
Color b(0.9, 0.3, 0.8);
Color result = (a * b).clamped();   // bleibt im Bereich [0, 1]
```

---

## Projektstruktur

```
CppRaytracer/
├── src/
│   ├── Camera/          # Kamera (FOV, View Transform)
│   ├── Canvas/          # Bildspeicher + PPM-Export
│   ├── Color/           # RGB-Farbe, Clamping, Hadamard
│   ├── HitInfo/         # Schnittpunkt-Metadaten
│   ├── Intersection/    # Ray-Objekt-Schnittpunkte
│   ├── LightSource/     # Direktional, Punkt, Spot
│   ├── Material/        # Phong-Material
│   ├── Matrix/          # 4×4 Transformationsmatrix
│   ├── Point/           # Homogener Punkt (w=1)
│   ├── Ray/             # Strahl (Ursprung + Richtung)
│   ├── RayTracer/       # Haupt-Render-Loop
│   ├── Scene/           # Szene (Shapes + Lights + Shading)
│   ├── Shape/           # Sphere, Plane, Cube, Cylinder, Cone
│   ├── Texture/         # Image- & UV-Textur-Mapping
│   ├── UV/              # UV-Koordinaten
│   └── Vector/          # Homogener Vektor (w=0)
├── tests/               # Google Test Suite
├── tests_aztest/        # AZTest Suite
├── libs/
│   └── AZTest-Cpp/      # Eingebettetes Test-Framework
├── archiv/              # Gespeicherte PPM-Render-Ergebnisse
└── CMakeLists.txt
```

---

## Technische Details

| Aspekt | Wert |
|---|---|
| C++ Standard | C++17 |
| Gleitkomma-Typ | `double` |
| Epsilon | `1e-6` |
| Koordinatensystem | Linkshändig |
| Max. Reflexions-Tiefe | 5 (konfigurierbar) |
| Build-System | CMake 3.14+ |
| Compiler | MSVC 19.38+ / GCC / Clang |
| Test-Frameworks | Google Test 1.14.0, AZTest-Cpp |

---

## Lizenz

MIT License — siehe [LICENSE](LICENSE)
